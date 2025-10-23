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

char* str_sub(const char *str, Int start, Int end)
{
    char *sub = (char*)malloc(end - start + 1);
    for (Int i = start; i < end; i++)
    {
        sub[i - start] = str[i];
    }
    sub[end - start] = '\0';
    return sub;
}

char* str_format(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    Int size = vsnprintf(NULL, 0, format, args);
    va_end(args);
    char *str = (char*)malloc(size + 1);
    va_start(args, format);
    vsprintf(str, format, args);
    va_end(args);
    return str;
}

char* str_concat(const char *str1, const char *str2)
{
    char *str = (char*)malloc(strlen(str1) + strlen(str2) + 1);
    strcpy(str, str1);
    strcat(str, str2);
    return str;
}

char* str_replace(const char *str, const char *substr, const char *replacement)
{
    const char *pos = strstr(str, substr); // Localiza a primeira ocorrência de 'substr'
    if (!pos)
    {
        // Se não encontrou, retorna uma cópia da string original
        char *newstr = (char *)malloc(strlen(str) + 1);
        strcpy(newstr, str);
        return newstr;
    }

    // Calcula os tamanhos
    size_t len_before = pos - str;
    size_t substr_len = strlen(substr);
    size_t replacement_len = strlen(replacement);
    size_t new_len = strlen(str) - substr_len + replacement_len;

    // Aloca memória para a nova string
    char *newstr = (char *)malloc(new_len + 1);

    // Constrói a nova string
    strncpy(newstr, str, len_before); // Parte antes da substring
    strcpy(newstr + len_before, replacement); // Substituição
    strcpy(newstr + len_before + replacement_len, pos + substr_len); // Parte após a substring

    return newstr;
}


char* str_replace_all(const char *str, const char *substr, const char *replacement)
{
    size_t substr_len = strlen(substr);
    size_t replacement_len = strlen(replacement);

    // Contar substrings e calcular tamanho necessário em uma única passagem
    size_t new_len = 0;
    size_t count = 0;
    for (const char *p = strstr(str, substr); p; p = strstr(p + substr_len, substr))
    {
        count++;
        new_len += (p - str) - new_len; // Adiciona a parte antes da substring
        new_len += replacement_len;    // Adiciona o tamanho da substituição
        str = p + substr_len;          // Avança para o próximo trecho
    }
    new_len += strlen(str); // Adiciona o restante da string

    // Aloca memória para a nova string
    char *newstr = (char *)malloc(new_len + 1);
    char *dest = newstr;

    // Construir a nova string
    str = str - new_len + strlen(newstr); // Reiniciar ponteiro para o início da string original
    const char *p = strstr(str, substr);
    while (p)
    {
        size_t len_before = p - str;
        strncpy(dest, str, len_before);    // Copiar parte antes da substring
        dest += len_before;

        strcpy(dest, replacement);        // Copiar substituição
        dest += replacement_len;

        str = p + substr_len;             // Avançar na string original
        p = strstr(str, substr);
    }
    strcpy(dest, str);                    // Copiar o restante da string original

    return newstr;
}


Int str_find(const char *str, const char *substr)
{
    return strstr(str, substr) - str;
}

List* special_space_split(char *str)
{
    List *splited = urb_new(URB_DEFAULT_SIZE);
    
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '(')
        {
            int j = i;
            int count = 1;
            while (count != 0)
            {
                j++;
                if (str[j] == '(')
                {
                    count++;
                }
                else if (str[j] == ')')
                {
                    count--;
                }
            }
            char *tmp = str_nduplicate(str + i, j - i + 1);
            urb_push(splited, (Value){.p = tmp});
            i = j + 1;
        }
        else if (str[i] == '"')
        {
            int j = i;
            j++;  // Avança para depois da abertura de aspas duplas
            while (str[j] != '"' && str[j] != '\0')
            {
                j++;
            }
            char *tmp = str_nduplicate(str + i, j - i + 1);
            urb_push(splited, (Value){.p = tmp});
            i = j + 1;  // Avança para após o fechamento de aspas duplas
        }
        else if (str[i] == '\'')
        {
            int j = i;
            j++;  // Avança para depois da abertura de aspas simples
            while (str[j] != '\'' && str[j] != '\0')
            {
                j++;
            }
            char *tmp = str_nduplicate(str + i, j - i + 1);
            urb_push(splited, (Value){.p = tmp});
            i = j + 1;  // Avança para após o fechamento de aspas simples
        }
        else if (isspace(str[i]))
        {
            i++;
        }
        else
        {
            int j = i;
            while (!isspace(str[j]) && str[j] != '\0' && str[j] != '(' && str[j] != ')' && str[j] != '"' && str[j] != '\'')
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
                else if(strchr(token, 'u'))
                {
                    urb_push(code, (Value){.i = ALIAS_BYPASS});
                    urb_push(code, (Value){.u = strtoul(token, NULL, 10)});
                }
                else
                {
                    urb_push(code, (Value){.u = strtol(token, NULL, 10)});
                }
            break;
            case '\'':
            {
                urb_push(code, (Value){.u = token[1]});
            }
            break;

            default:
            {
                bool found = 0;
                for(UInt j = 0; j < 32; j++)
                {
                    if(strcmp(token, op_names[j]) == 0)
                    {
                        urb_push(code, (Value){.i = op_values[j]});
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    for(UInt j = 0; j < CUSTOM_FUNC_COUNT; j++)
                    {
                        if(strcmp(token, custom_func_names[j]) == 0)
                        {
                            urb_push(code, (Value){.i = custom_func_indexes[j]});
                            found = true;
                            break;
                        }
                    }
                }
                if (!found)
                {
                    for(UInt j = 0; j < label_names->size; j++)
                    {
                        if(strcmp(token, label_names->data[j].p) == 0)
                        {
                            urb_push(code, (Value){.i = label_values->data[j].i});
                            found = true;
                            break;
                        }
                    }
                }
            }
            break;
        }
        free(token);
    }
    urb_free(label_names);
    urb_free(label_values);
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