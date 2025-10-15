#include "urb.h"
#include "urb.c"

const char* urb_help_message = 
"urb v" URB_VERSION "\n"
"usage:\n"
"    urb [option || filename]\n"
"options:\n"
"    urb help\n"
"    urb version\n"
"    urb dictionary\n"
"    urb functions\n"
"    urb version\n"
"compile:\n"
"    urb filename.urb\n"
"interpret:\n"
"    urb filename.urbin\n";

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
        printf(urb_help_message);
        return 1;
    }
    else if (argc >= 2)
    {
        if (strcmp(argv[1], "dictionary") == 0)
        {
            Int j = 0;
            while(urb_function_names[j] != 0)
            {
                printf("#define %s %d\n", urb_function_names[j], j);
                j++;
            }
        }
        else if (strcmp(argv[1], "functions") == 0)
        {
            Int j = 0;
            while(urb_function_names[j] != 0)
            {
                printf("context[%d] = %s;\n", j, urb_function_names[j]);
                j++;
            }
        }
        else if (strcmp(argv[1], "version") == 0)
        {
            printf("urb v%s\n", URB_VERSION);
        }
        else if (strcmp(argv[1], "help") == 0)
        {
            printf(urb_help_message);
            return 1;
        }
        // a binary been passed, lets run it then
        else if(strstr(argv[1], ".urbin") != NULL)
        {
            // header[0] = capacity
            // header[1] = size
            Half header[2];

            List* code;
            List* context = urb_new(URB_DEFAULT_SIZE);
            FILE *file = fopen(argv[1], "rb");

            INIT_URB(context);
            
            if (fread(header, sizeof(Half), 2, file) != 2)
            {
                fprintf(stderr, "error: this file is smaller than a urb header\n");
                return 1;
            };
            
            code = urb_new(header[0]);
            code->size = header[1];

            // we read *size* elements
            if (fread(code->data, sizeof(Int), code->size, file) != code->size)
            {
                // if we have a header claiming it has more elements -
                // - than it does, then the file is corrupted
                // or was not generated properly
                // or is not a urb binary
                fprintf(stderr, "error: this file is probably corrupted\n");
                return 1;
            }
            
            fclose(file);

            urb_interpret(context, code, NULL);

            urb_free(context);
            urb_free(code);
        }
        else
        {
            // not .urbin
            // we treat everything else as a .urb source
            // so lets compile it then
            output_name = malloc(strlen(argv[1]) + 3);
            strcat(output_name, argv[1]);
            strcat(output_name, "in");

            char* file_content = slow_file_read(argv[1]);
            List* compiled = urb_preprocess(file_content);
            char* binary = malloc(sizeof(Int) * (compiled->size + 1));
            
            // we copy the metadata(.size and .capacity)
            memcpy(binary, compiled, sizeof(Int));
            
            // we copy the content(.data)
            memcpy(binary + sizeof(Int), compiled->data, compiled->size * sizeof(Int));

            FILE *file = fopen(output_name, "wb");
            if (file == NULL)
            {
                return 1;
            }
            
            fwrite(binary, 1, sizeof(Int) * (compiled->size + 1), file);
            fclose(file);

            free(output_name);
        }
    }
};