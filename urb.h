// urb
// urb use no other libraries beside the standard C99 libraries
// if urb does not work on a platform, it is a bug, and should be reported.

#ifndef URB_H
#define URB_H 1

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <ctype.h>

// version
#define URB_VERSION "0.9.4"

typedef intptr_t Int;
typedef uintptr_t UInt;

// URB use Int and Float instead of int and float because we need to use always the pointer size for any type that might share the fundamental union type;
// URB use a union as universal type, and URB is able to manipulate and use pointers direcly so we need to use the pointer size;
#if INTPTR_MAX == INT64_MAX
    typedef double Float;
    typedef uint32_t UHalf;
    typedef int32_t Half;
    #define INT_MAX INT64_MAX
    #define INT_MIN INT64_MIN
    #define UINT_MAX UINT64_MAX
    #define UHALF_MAX UINT32_MAX
    #define HALF_MAX INT32_MAX
    #define HALF_MIN INT32_MIN
#else
    typedef float Float;
    typedef uint16_t UHalf;
    typedef int16_t Half;
    #define INT_MAX INT32_MAX
    #define INT_MIN INT32_MIN
    #define UINT_MAX UINT32_MAX
    #define UHALF_MAX UINT16_MAX
    #define HALF_MAX INT16_MAX
    #define HALF_MIN INT16_MIN
#endif

#ifndef URB_DEFAULT_SIZE
    #define URB_DEFAULT_SIZE 0
#endif

typedef struct List List;
typedef union Value Value;
typedef void (*Function)(List *stack);

union Value
{
    // all types depend on the size of the pointer
    Int i;
    Float f;
    UInt u;
    void* p;
    uint8_t b[sizeof(void*)];
    // in C99 you cant cast a function pointer from a void*, 
    // so we need the function pointer in the union as well
    Function fn;
};

struct List
{
    // the real size
    UHalf capacity;
    // the current size
    UHalf size;
    // the data, it is a pointer to a Value array
    Value *data;
};

// List functions   
// create a new list with the given size, if size is 0, it will be initialized with NULL data and then allocated when needed
static inline List*              urb_new(Int size);
// free the list    
static inline void               urb_free(List *list);
// double the list capacity   
static inline void               urb_double(List *list);
// halve the list capacity   
static inline void               urb_half(List *list);
// push a value to the end of the list
static inline void               urb_push(List *list, Value value);
// unshift a value to the start of the list
static inline void               urb_unshift(List *list, Value value);
// insert a value at index i in the list
static inline void               urb_insert(List *list, Int i, Value value);
// pop a value from the end of the list
static inline Value              urb_pop(List *list);
// shift a value from the start of the list
static inline Value              urb_shift(List *list);
// remove and return the value at index i in the list, shifting the rest of the list
static inline Value              urb_remove(List *list, Int i);
// fast remove a value at index i in the list, swapping it with the last element and popping it
static inline Value              urb_fast_remove(List *list, Int i);
// swap two values in the list at indices i1 and i2
static inline void               urb_swap(List *list, Int i1, Int i2);
// find the index of a value in the list, returns -1 if not found
static inline Int                urb_find(const List *list, Value value);
// reverse the list in place   
static inline void               urb_reverse(List *list);
// create a copy of the list, with the same capacity and size, but new data array
static inline List*              urb_copy(const List *list);
// get a value at index i in the list, returns a value with i set to -1 if index is out of range
static inline Value              urb_get(const List *list, Int i);
// set a value at index i in the list, if index is out of range, it will print an error and exit
static inline void               urb_set(List *list, Int i, Value value);
// get the ub version   
static inline const char*        urb_get_version(void);
// arena   
static inline void*              urb_alloc(List *arena, size_t size);
// preprocess string into urb code
static inline List*              urb_preprocess(char* input_str);
// ub representation
static inline void               urb_interpret(List *context, List* code, List* stack);

// functions implementations
// functions implementations
// functions implementations
// functions implementations

static inline List *urb_new(Int size)
{
    List *list = (List*)malloc(sizeof(List));
    
    if (list == NULL)
    {
        printf("URB_ERROR: failed to allocate memory for List\n");
        exit(EXIT_FAILURE);
    }
    
    list->data = (size == 0) ? NULL : (Value*)malloc((size_t)size * sizeof(Value));
    
    if (size > 0 && list->data == NULL)
    {
        printf("URB_ERROR: failed to allocate memory for List data\n");
        exit(EXIT_FAILURE);
    }
    
    list->size = 0;
    list->capacity = size;

    return list;
}

static inline void urb_free(List *list)
{
    free(list->data);
    free(list);
}

static inline void urb_double(List *list)
{
    Value *new_data = NULL;
    list->capacity = list->capacity == 0 ? 1 : list->capacity * 2;
    new_data = (Value*)realloc(list->data, (size_t)list->capacity * sizeof(Value));
    if (!new_data)
    {
        printf("URB_ERROR: failed to reallocate list data\n");
        exit(EXIT_FAILURE);
    }
    list->data = new_data;
}

static inline void urb_half(List *list)
{
    Value *new_data = NULL;
    list->capacity /= 2;
    new_data = (Value*)realloc(list->data, (size_t)list->capacity * sizeof(Value));
    if (!new_data)
    {
        printf("URB_ERROR: failed to reallocate list data\n");
        exit(EXIT_FAILURE);
    }
    list->data = new_data;

    if (list->size > list->capacity)
    {
        list->size = list->capacity;
    }
}

static inline void urb_push(List *list, Value value)
{
    if (list->size == list->capacity)
    {
        urb_double(list);
    }
    list->data[list->size] = value;
    list->size++;
}

static inline void urb_unshift(List *list, Value value)
{
    if (list->size == list->capacity)
    {
        urb_double(list);
    }
    memmove(&(list->data[1]), &(list->data[0]), (size_t)list->size * sizeof(Value));
    list->data[0] = value;

    list->size++;
}

static inline void urb_insert(List *list, Int i, Value value)
{
    if (list->size == list->capacity)
    {
        urb_double(list);
    }

    if (i < 0 || i > list->size)
    {
        printf("URB_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
    else
    {
        memmove(&(list->data[i + 1]), &(list->data[i]), (size_t)(list->size - i) * sizeof(Value));
        list->data[i] = value;
        list->size++;
    }
}

static inline Value urb_pop(List *list)
{
    Value ret = list->data[list->size - 1]; 
    if (list->size == 0)
    {
        printf("URB_ERROR: cannot pop from empty list\n");
        exit(EXIT_FAILURE);
    }
    list->size--;
    return ret;
}

static inline Value urb_shift(List *list)
{
    Value ret = list->data[0];
    if (list->size == 0)
    {
        printf("URB_ERROR: cannot shift from empty list\n");
        exit(EXIT_FAILURE);
    }
    else if (list->size > 1) 
    { 
        memmove(&(list->data[0]), &(list->data[1]), (size_t)(list->size - 1) * sizeof(Value)); 
    } 
    list->size--; 
    return ret;
}

static inline Value urb_remove(List *list, Int i)
{
    Value ret = list->data[i]; 

    size_t elements_to_move = 0; 
    if (list->size == 0)
    {
        printf("URB_ERROR: cannot pop from empty list\n");
        exit(EXIT_FAILURE);
    }
    elements_to_move = (size_t)(list->size - (i) - 1); 
    
    /* Move data elements */ 
    memmove(&(list->data[i]), &(list->data[i + 1]), 
            elements_to_move * sizeof(Value)); 
    list->size--; 
    
    return ret;
}

static inline Value urb_fast_remove(List *list, Int i)
{
    urb_swap(list, i, list->size - 1);
    return urb_pop(list);
}

static inline void urb_swap(List *list, Int i1, Int i2)
{
    Value tmp = list->data[i1];
    list->data[i1] = list->data[i2];
    list->data[i2] = tmp;
}

static inline Int urb_find(const List *list, Value value) 
{
    for (Int i = list->size - 1; i >= 0; i--)
    {
        if (list->data[i].i == value.i)
        {
            return i;
        }
    }

    return -1;
}

static inline void urb_reverse(List *list)
{
    for (Int i = 0; i < list->size / 2; i++)
    {
        urb_swap(list, i, list->size - i - 1);
    }
}

static inline List* urb_copy(const List *list)
{
    List *copy = urb_new(list->capacity);
    
    if (copy == NULL)
    {
        printf("URB_ERROR: failed to allocate memory for List copy\n");
        exit(EXIT_FAILURE);
    }
    copy->size = list->size;
    memcpy(copy->data, list->data, (size_t)copy->size * sizeof(Value));
    
    copy->capacity = list->capacity;

    return copy;
}

static inline Value urb_get(const List *list, Int i)
{
    if (i < 0 || i >= list->size)
    {
        return (Value){.i=-1}; // return -1 if index is out of range
    }
    return list->data[i];
}

static inline void urb_set(List *list, Int i, Value value)
{
    if (i < 0 || i >= list->size)
    {
        printf("URB_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
    list->data[i] = value;
}

static inline const char* urb_get_version(void)
{
    return URB_VERSION;
}

static inline void* urb_alloc(List* arena, size_t size)
{
    size_t aligned_size = (size + sizeof(void*) - 1) & ~(sizeof(void*) - 1);
    // we use arena->size as offset
    if (arena->size + aligned_size > arena->capacity)
    {
        urb_double(arena);
    }
    void *ptr = arena->data + arena->size;
    arena->size += aligned_size;
    return ptr;
}

// interpreter special tokens
#define IFGO_TOK INT_MAX - 1
#define BANG_TOK INT_MAX - 2
#define GET_TOK  INT_MAX - 3

// pseudo positions to places we not know at preprocessing
#define CONTEXT_TOK INT_MIN + 1
#define STACK_TOK   INT_MIN + 2
#define CODE_TOK    INT_MIN + 3

// this modify the string;
static inline List* urb_preprocess(char* input_str)
{
    List* code = urb_new(URB_DEFAULT_SIZE);

    // Duplicate the input string to avoid modifying the original
    char* token = strtok(input_str, "\n\t \r");
    
    while (token != NULL)
    {
        // defining URB_FUNCTIONS_NAMES_NOT_AVAILABLE can boost performance of preprocessing
        // but also require one to turn all function names into their proper index
        // if so try calling "./urb macros" on the cli tool can help u
        // URB_FUNCTIONS_NAMES_NOT_AVAILABLE is very useful for realtime preprocessing
        #ifndef URB_FUNCTIONS_NAMES_NOT_AVAILABLE
            Int j = 0;
            Int found = false;
        
            while(urb_function_names[j] != 0)
            {
                if(strcmp(token, urb_function_names[j]) == 0)
                {
                    urb_push(code, (Value){.i = j});
                    found = true;
                    break;
                }
                j++;
            }

            if (found)
            {
                token = strtok(NULL, "\n\t \r");
                continue;
            }
        #endif

        switch (token[0])
        {
            // context
            case '@':
            {
                urb_push(code, (Value){.i = CONTEXT_TOK});
            }
            break;
            // stack
            case '%':
            {
                urb_push(code, (Value){.i = STACK_TOK});
            }
            break;
            // code
            case '$':
            {
                urb_push(code, (Value){.i = CODE_TOK});
            }
            break;
            // bang
            case '!':
            {
                urb_push(code, (Value){.i = BANG_TOK});
            }
            break;
            // ifgo
            case '?':
            {
                urb_push(code, (Value){.i = IFGO_TOK});
            }
            break;
            // get
            case ':':
            {
                urb_push(code, (Value){.i = GET_TOK});
            }
            break;
            case '0':
            {
                if (token[1] == 'b')
                {
                    // binary
                    // 0b0101...
                    // base 2
                    urb_push(code, (Value){.i = strtol(token + 2, NULL, 2)});
                    break;
                }
                else if (token[1] == 'o')
                {
                    // octal
                    // 0o8732...
                    // base 8
                    urb_push(code, (Value){.i = strtol(token + 2, NULL, 8)});
                    break;
                }
                else if (token[1] == 'x')
                {
                    // hex
                    // 0xFE98...
                    // base 16
                    urb_push(code, (Value){.i = strtol(token + 2, NULL, 16)});
                    break;
                }
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
                if (strchr(token, '.') != NULL)
                {
                    // if so, we have a float here
                    urb_push(code, (Value){.f = strtod(token, NULL)});
                }
                else
                {
                    // its a regular integer
                    urb_push(code, (Value){.i = strtol(token, NULL, 10)});
                }
                break;
            default:
                // we dont know wtf is this
                // as a last effort to not skip tokens nor crash
                // we gonna just push the pointer to this token as it is
                urb_push(code, (Value){.p = token});
                break;
        }
        token = strtok(NULL, "\n\t \r");
    }
    return code;
}

// if you want to return something, pass a stack, values will be there
// if you do not provide a stack, a new one will be created and freed at the end
static inline void urb_interpret(List *context, List* code, List* _stack)
{
    List *stack;

    if (_stack == NULL)
    {
        stack = urb_new(URB_DEFAULT_SIZE);
    }
    else
    {
        stack = _stack;
    }

    // interpreting
    for (Int i = 0; i < code->size; i++)
    {
        switch(code->data[i].i)
        {
            case GET_TOK:
            {
                Int index_toget = urb_pop(stack).i;
                if (index_toget < context->capacity)
                {
                    urb_push(stack, context->data[index_toget]);
                }
                else
                {
                    printf("cannot get variable %ld, curretly context capacity is %ls\n", index_toget, context->capacity);
                }
            }
            break;
            case BANG_TOK:
            {
                // in C99 you cant cast a function pointer from a void*
                Function bang_func = urb_pop(stack).fn;
                bang_func(stack);
            }
            break;
            case IFGO_TOK:
            {
                bool cond = urb_pop(stack).i;
                Int position = urb_pop(stack).i;
                if (cond)
                {
                    i = position;
                }
            }
            break;
            case CONTEXT_TOK:
            {
                urb_push(stack, (Value){.p = context});
            }
            break;
            case STACK_TOK:
            {
                urb_push(stack, (Value){.p = stack});
            }
            break;
            case CODE_TOK:
            {
                urb_push(stack, (Value){.p = code});
            }
            break;
            default:
            {
                urb_push(stack, code->data[i]);
            }
            break;
        }
    }

    if (_stack == NULL) 
    {
        urb_free(stack); // free stack only if it was created here
    }
}

#endif // ifndef URB_H macro


// CLI Tool
// this used to be a file apart
// but lets try put it here
#ifdef URB_ENABLE_CLI
    #ifndef URB_CLI_ENABLED
        #define URB_CLI_ENABLED 1

        const char* urb_help_message = 
        "urb v" URB_VERSION "\n"
        "usage:\n"
        "    urb [option / filename]\n"
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
    #endif
#endif