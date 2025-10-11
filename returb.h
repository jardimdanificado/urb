// returb
// returb use no other libraries beside the standard C99 libraries
// if returb does not work on a platform, it is a bug, and should be reported.

#ifndef RETURB_H
#define RETURB_H 1

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <ctype.h>

// version
#define RETURB_VERSION "0.9.4"

typedef intptr_t Int;
typedef uintptr_t UInt;

// RETURB use Int and Float instead of int and float because we need to use always the pointer size for any type that might share the fundamental union type;
// RETURB use a union as universal type, and RETURB is able to manipulate and use pointers direcly so we need to use the pointer size;
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

#ifndef RETURB_DEFAULT_SIZE
    #define RETURB_DEFAULT_SIZE 0
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
static inline List*              returb_new(Int size);
// free the list    
static inline void               returb_free(List *list);
// double the list capacity   
static inline void               returb_double(List *list);
// halve the list capacity   
static inline void               returb_half(List *list);
// push a value to the end of the list
static inline void               returb_push(List *list, Value value);
// unshift a value to the start of the list
static inline void               returb_unshift(List *list, Value value);
// insert a value at index i in the list
static inline void               returb_insert(List *list, Int i, Value value);
// pop a value from the end of the list
static inline Value              returb_pop(List *list);
// shift a value from the start of the list
static inline Value              returb_shift(List *list);
// remove and return the value at index i in the list, shifting the rest of the list
static inline Value              returb_remove(List *list, Int i);
// fast remove a value at index i in the list, swapping it with the last element and popping it
static inline Value              returb_fast_remove(List *list, Int i);
// swap two values in the list at indices i1 and i2
static inline void               returb_swap(List *list, Int i1, Int i2);
// find the index of a value in the list, returns -1 if not found
static inline Int                returb_find(const List *list, Value value);
// reverse the list in place   
static inline void               returb_reverse(List *list);
// create a copy of the list, with the same capacity and size, but new data array
static inline List*              returb_copy(const List *list);
// get a value at index i in the list, returns a value with i set to -1 if index is out of range
static inline Value              returb_get(const List *list, Int i);
// set a value at index i in the list, if index is out of range, it will print an error and exit
static inline void               returb_set(List *list, Int i, Value value);
// get the ub version   
static inline const char*        returb_get_version(void);
// arena   
static inline void*              returb_alloc(List *arena, size_t size);
// preprocess string into returb code
static inline List*              returb_preprocess(char* input_str);
// ub representation
static inline void               returb_interpret(List *context, List* code, List* stack);

// functions implementations
// functions implementations
// functions implementations
// functions implementations

static inline List *returb_new(Int size)
{
    List *list = (List*)malloc(sizeof(List));
    
    if (list == NULL)
    {
        printf("RETURB_ERROR: failed to allocate memory for List\n");
        exit(EXIT_FAILURE);
    }
    
    list->data = (size == 0) ? NULL : (Value*)malloc((size_t)size * sizeof(Value));
    
    if (size > 0 && list->data == NULL)
    {
        printf("RETURB_ERROR: failed to allocate memory for List data\n");
        exit(EXIT_FAILURE);
    }
    
    list->size = 0;
    list->capacity = size;

    return list;
}

static inline void returb_free(List *list)
{
    free(list->data);
    free(list);
}

static inline void returb_double(List *list)
{
    Value *new_data = NULL;
    list->capacity = list->capacity == 0 ? 1 : list->capacity * 2;
    new_data = (Value*)realloc(list->data, (size_t)list->capacity * sizeof(Value));
    if (!new_data)
    {
        printf("RETURB_ERROR: failed to reallocate list data\n");
        exit(EXIT_FAILURE);
    }
    list->data = new_data;
}

static inline void returb_half(List *list)
{
    Value *new_data = NULL;
    list->capacity /= 2;
    new_data = (Value*)realloc(list->data, (size_t)list->capacity * sizeof(Value));
    if (!new_data)
    {
        printf("RETURB_ERROR: failed to reallocate list data\n");
        exit(EXIT_FAILURE);
    }
    list->data = new_data;

    if (list->size > list->capacity)
    {
        list->size = list->capacity;
    }
}

static inline void returb_push(List *list, Value value)
{
    if (list->size == list->capacity)
    {
        returb_double(list);
    }
    list->data[list->size] = value;
    list->size++;
}

static inline void returb_unshift(List *list, Value value)
{
    if (list->size == list->capacity)
    {
        returb_double(list);
    }
    memmove(&(list->data[1]), &(list->data[0]), (size_t)list->size * sizeof(Value));
    list->data[0] = value;

    list->size++;
}

static inline void returb_insert(List *list, Int i, Value value)
{
    if (list->size == list->capacity)
    {
        returb_double(list);
    }

    if (i < 0 || i > list->size)
    {
        printf("RETURB_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
    else
    {
        memmove(&(list->data[i + 1]), &(list->data[i]), (size_t)(list->size - i) * sizeof(Value));
        list->data[i] = value;
        list->size++;
    }
}

static inline Value returb_pop(List *list)
{
    Value ret = list->data[list->size - 1]; 
    if (list->size == 0)
    {
        printf("RETURB_ERROR: cannot pop from empty list\n");
        exit(EXIT_FAILURE);
    }
    list->size--;
    return ret;
}

static inline Value returb_shift(List *list)
{
    Value ret = list->data[0];
    if (list->size == 0)
    {
        printf("RETURB_ERROR: cannot shift from empty list\n");
        exit(EXIT_FAILURE);
    }
    else if (list->size > 1) 
    { 
        memmove(&(list->data[0]), &(list->data[1]), (size_t)(list->size - 1) * sizeof(Value)); 
    } 
    list->size--; 
    return ret;
}

static inline Value returb_remove(List *list, Int i)
{
    Value ret = list->data[i]; 

    size_t elements_to_move = 0; 
    if (list->size == 0)
    {
        printf("RETURB_ERROR: cannot pop from empty list\n");
        exit(EXIT_FAILURE);
    }
    elements_to_move = (size_t)(list->size - (i) - 1); 
    
    /* Move data elements */ 
    memmove(&(list->data[i]), &(list->data[i + 1]), 
            elements_to_move * sizeof(Value)); 
    list->size--; 
    
    return ret;
}

static inline Value returb_fast_remove(List *list, Int i)
{
    returb_swap(list, i, list->size - 1);
    return returb_pop(list);
}

static inline void returb_swap(List *list, Int i1, Int i2)
{
    Value tmp = list->data[i1];
    list->data[i1] = list->data[i2];
    list->data[i2] = tmp;
}

static inline Int returb_find(const List *list, Value value) 
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

static inline void returb_reverse(List *list)
{
    for (Int i = 0; i < list->size / 2; i++)
    {
        returb_swap(list, i, list->size - i - 1);
    }
}

static inline List* returb_copy(const List *list)
{
    List *copy = returb_new(list->capacity);
    
    if (copy == NULL)
    {
        printf("RETURB_ERROR: failed to allocate memory for List copy\n");
        exit(EXIT_FAILURE);
    }
    copy->size = list->size;
    memcpy(copy->data, list->data, (size_t)copy->size * sizeof(Value));
    
    copy->capacity = list->capacity;

    return copy;
}

static inline Value returb_get(const List *list, Int i)
{
    if (i < 0 || i >= list->size)
    {
        return (Value){.i=-1}; // return -1 if index is out of range
    }
    return list->data[i];
}

static inline void returb_set(List *list, Int i, Value value)
{
    if (i < 0 || i >= list->size)
    {
        printf("RETURB_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
    list->data[i] = value;
}

static inline const char* returb_get_version(void)
{
    return RETURB_VERSION;
}

static inline void* returb_alloc(List* arena, size_t size)
{
    size_t aligned_size = (size + sizeof(void*) - 1) & ~(sizeof(void*) - 1);
    // we use arena->size as offset
    if (arena->size + aligned_size > arena->capacity)
    {
        returb_double(arena);
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
static inline List* returb_preprocess(char* input_str)
{
    List* code = returb_new(RETURB_DEFAULT_SIZE);

    // Duplicate the input string to avoid modifying the original
    char* token = strtok(input_str, "\n\t \r");
    
    while (token != NULL)
    {
        // if a returb_function_names is available use it as dictionary 
        #ifdef RETURB_FUNCTIONS_NAMES_AVAILABLE
            Int j = 0;
            Int found = false;
        
            while(returb_function_names[j] != 0)
            {
                if(strcmp(token, returb_function_names[j]) == 0)
                {
                    returb_push(code, (Value){.i = j});
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
                returb_push(code, (Value){.i = CONTEXT_TOK});
            }
            break;
            // stack
            case '%':
            {
                returb_push(code, (Value){.i = STACK_TOK});
            }
            break;
            // code
            case '$':
            {
                returb_push(code, (Value){.i = CODE_TOK});
            }
            break;
            // bang
            case '!':
            {
                returb_push(code, (Value){.i = BANG_TOK});
            }
            break;
            // ifgo
            case '?':
            {
                returb_push(code, (Value){.i = IFGO_TOK});
            }
            break;
            // get
            case ':':
            {
                returb_push(code, (Value){.i = GET_TOK});
            }
            break;
            case '0':
            {
                if (token[1] == 'b')
                {
                    // binary
                    // 0b0101...
                    // base 2
                    returb_push(code, (Value){.i = strtol(token + 2, NULL, 2)});
                    break;
                }
                else if (token[1] == 'o')
                {
                    // octal
                    // 0o8732...
                    // base 8
                    returb_push(code, (Value){.i = strtol(token + 2, NULL, 8)});
                    break;
                }
                else if (token[1] == 'x')
                {
                    // hex
                    // 0xFE98...
                    // base 16
                    returb_push(code, (Value){.i = strtol(token + 2, NULL, 16)});
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
                    returb_push(code, (Value){.f = strtod(token, NULL)});
                }
                else
                {
                    // its a regular integer
                    returb_push(code, (Value){.i = strtol(token, NULL, 10)});
                }
                break;
            default:
                // we dont know wtf is this
                // as a last effort to not skip tokens nor crash
                // we gonna just push the pointer to this token as it is
                returb_push(code, (Value){.p = token});
                break;
        }
        token = strtok(NULL, "\n\t \r");
    }
    return code;
}

// if you want to return something, pass a stack, values will be there
// if you do not provide a stack, a new one will be created and freed at the end
static inline void returb_interpret(List *context, List* code, List* _stack)
{
    List *stack;

    if (_stack == NULL)
    {
        stack = returb_new(RETURB_DEFAULT_SIZE);
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
                Int index_toget = returb_pop(stack).i;
                if (index_toget < context->capacity)
                {
                    returb_push(stack, context->data[index_toget]);
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
                Function bang_func = returb_pop(stack).fn;
                bang_func(stack);
            }
            break;
            case IFGO_TOK:
            {
                bool cond = returb_pop(stack).i;
                Int position = returb_pop(stack).i;
                if (cond)
                {
                    i = position;
                }
            }
            break;
            case CONTEXT_TOK:
            {
                returb_push(stack, (Value){.p = context});
            }
            break;
            case STACK_TOK:
            {
                returb_push(stack, (Value){.p = stack});
            }
            break;
            case CODE_TOK:
            {
                returb_push(stack, (Value){.p = code});
            }
            break;
            default:
            {
                returb_push(stack, code->data[i]);
            }
            break;
        }
    }

    if (_stack == NULL) 
    {
        returb_free(stack); // free stack only if it was created here
    }
}

#endif // ifndef RETURB_H macro