#include "returb_function_names.h"
#include "returb.h"

// this is slow af
char* slow_file_read(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return NULL;
    }

    char *code = (char*)malloc(1);
    if (code == NULL)
    {
        printf("ERROR: could not allocate memory for file\n");
        fclose(file);
        return NULL;
    }

    code[0] = '\0';

    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, file) != -1)
    {
        size_t new_size = strlen(code) + strlen(line) + 1;
        char *temp = realloc(code, new_size);
        if (temp == NULL)
        {
            printf("ERROR: could not reallocate memory while reading file\n");
            free(code);
            free(line);
            fclose(file);
            return NULL;
        }
        code = temp;
        strcat(code, line);
    }

    free(line);
    fclose(file);
    return code;
}

int main(int argc, char* argv[])
{
    char *output_name = NULL;
    if (argc <= 1)
    {
        printf("you havent passed a filename\n");
        return 1;
    }
    else if (argc <= 2)
    {
        output_name = malloc(strlen(argv[1]) + 7);
        strcat(output_name, argv[1]);
        strcat(output_name, ".urbin");
    }
    else 
    {
        output_name = strdup(argv[2]);
    }

    char* file_content = slow_file_read(argv[1]);
    List* compiled = returb_preprocess(file_content);
    char* binary = malloc(sizeof(Int) * (compiled->size + 1));
    
    // we copy the metadata(.size and .capacity)
    memcpy(binary, compiled, sizeof(Int));
    
    // we copy the content(.data)
    memcpy(binary + sizeof(Int), compiled->data, compiled->size * sizeof(Int));

    FILE *file = fopen(output_name, "wb");
    printf("output_name: %s\n", output_name);
    if (file == NULL)
    {
        return 1;
    }
    
    fwrite(binary, 1, sizeof(Int) * (compiled->size + 1), file);
    fclose(file);

    free(output_name);
};