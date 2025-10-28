#include "urb.h"
// urb.c is a generated file
// usually found at build/urb.c
#include "urb.c"
#include "stdarg.h"

// file stuff
char* file_read(char *filename)
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

//string functions
char* str_duplicate(const char *str)
{
    char *dup = (char*)malloc(strlen(str) + 1);
    strcpy(dup, str);
    return dup;
}

char* str_nduplicate(const char *str, Int n)
{
    char *dup = (char*)malloc(n + 1);
    for (Int i = 0; i < n; i++)
    {
        dup[i] = str[i];
    }
    dup[n] = '\0';
    return dup;
}

List* special_space_split(char *str)
{
    List *splited = urb_new(URB_DEFAULT_SIZE);

    int i = 0;
    while (str[i] != '\0')
    {
        // --- Blocos delimitados: (), {}, [], <>
        if (str[i] == '(' || str[i] == '[' || str[i] == '{' || str[i] == '<')
        {
            int j = i;
            char stack[256];
            int top = 0;
            stack[top++] = str[i]; // empilha delimitador inicial

            j++;
            while (str[j] != '\0' && top > 0)
            {
                char c = str[j];
                char last = stack[top - 1];

                if (c == '(' || c == '[' || c == '{' || c == '<')
                {
                    if (top < 256)
                        stack[top++] = c;
                }
                else if ((c == ')' && last == '(') ||
                         (c == ']' && last == '[') ||
                         (c == '}' && last == '{') ||
                         (c == '>' && last == '<'))
                {
                    top--;
                }
                j++;
            }

            if (top == 0)
            {
                // delimitadores equilibrados
                char *tmp = str_nduplicate(str + i, j - i);
                urb_push(splited, (Value){.p = tmp});
                i = j;
                continue;
            }
            else
            {
                // erro de fechamento, apenas duplica o resto
                char *tmp = str_duplicate(str + i);
                urb_push(splited, (Value){.p = tmp});
                break;
            }
        }
        // --- string
        else if (str[i] == '"')
        {
            int j = i + 1;
            while (str[j] != '\0' && str[j] != '"')
                j++;
            if (str[j] == '"')
                j++;
            urb_push(splited, (Value){.p = str_nduplicate(str + i, j - i)});
            i = j;
        }
        // --- char
        else if (str[i] == '\'')
        {
            int j = i + 1;
            while (str[j] != '\0' && str[j] != '\'')
                j++;
            if (str[j] == '\'')
                j++;
            urb_push(splited, (Value){.p = str_nduplicate(str + i, j - i)});
            i = j;
        }
        // --- space
        else if (isspace((unsigned char)str[i]))
        {
            i++;
        }
        // --- regular tokens
        else
        {
            int j = i;
            while (str[j] != '\0' &&
                   !isspace((unsigned char)str[j]) &&
                   str[j] != '(' && str[j] != ')' &&
                   str[j] != '[' && str[j] != ']' &&
                   str[j] != '{' && str[j] != '}' &&
                   str[j] != '<' && str[j] != '>' &&
                   str[j] != '"' && str[j] != '\'')
            {
                j++;
            }
            urb_push(splited, (Value){.p = str_nduplicate(str + i, j - i)});
            i = j;
        }
    }

    return splited;
}

static inline List* urb_preprocess(char* input_str)
{
    List* code = urb_new(URB_DEFAULT_SIZE);
    List* label_values = urb_new(URB_DEFAULT_SIZE);
    List* label_names = urb_new(URB_DEFAULT_SIZE);
    List* tokens = special_space_split(input_str);

    for(UInt i = 0; i < tokens->size; i++)
    {
        char* token = tokens->data[i].p;
        if(strchr(token, ':'))
        {
            token[strlen(token)-1] = 0;
            urb_push(label_names, (Value){.p = token});
            urb_push(label_values, (Value){.u = i});
            urb_remove(tokens, i);
            i--;
        }
    }

    for(UInt i = 0; i < tokens->size; i++)
    {
        char* token = tokens->data[i].p;
        switch (token[0])
        {
            case '0':
                if(token[1] == 'b')
                {
                    urb_push(code, (Value){.i = strtol(token + 2, NULL, 2)});
                    break;
                }
                else if(token[1] == 'x')
                {
                    urb_push(code, (Value){.i = strtol(token + 2, NULL, 16)});
                    break;
                }
                else if(token[1] == 'o')
                {
                    urb_push(code, (Value){.i = strtol(token + 2, NULL, 8)});
                    break;
                }
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if(strchr(token, 'f'))
                {
                    urb_push(code, (Value){.f = strtod(token, NULL)});
                }
                else
                {
                    urb_push(code, (Value){.u = strtol(token, NULL, 10)});
                }
            break;
            
            // char
            case '\'':
            {
                urb_push(code, (Value){.u = token[1]});
            }
            break;

            default:
            {
                bool found = 0;
                for(UInt j = 0; j < CUSTOM_FUNC_COUNT; j++)
                {
                    if(strcmp(token, custom_func_names[j]) == 0)
                    {
                        urb_push(code, (Value){.i = custom_func_opcodes[j]});
                        found = true;
                        break;
                    }
                }
                if(found) break;

                if(strcmp(token, "jif") == 0)
                {
                    urb_push(code, (Value){.i = INT_MIN});
                    found = true;
                    break;
                }

                for(UInt j = 0; j < label_names->size; j++)
                {
                    if(strcmp(token, label_names->data[j].p) == 0)
                    {
                        urb_push(code, (Value){.i = label_values->data[j].i});
                        found = true;
                        break;
                    }
                }
                if(found) break;
            }
            break;
        }
        free(token);
    }
    return code;
}

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        printf("you need to pass a filename.\n");
    }
    
    char* file_content = file_read(argv[1]);
    
    List* compiled = urb_preprocess(file_content);
    
    char* binary = malloc(sizeof(Int) * (compiled->size + 1));

    // we copy the metadata(.size and .capacity)
    memcpy(binary, compiled, sizeof(Int));
    
    // we copy the content(.data)
    memcpy(binary + sizeof(Int), compiled->data, compiled->size * sizeof(Int));

    fwrite(binary, 8, compiled->size + 1, stdout);
    free(file_content);
    return 0;
};