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
    UInt u;
    Float f;
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
static inline List*              urb_from_pointer(void* ptr, Int size);
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

static inline List *urb_from_pointer(void* ptr, Int size)
{
    List *list = urb_new(size);
    memcpy(list->data, ptr, size * sizeof(Int));
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

// BASES
#define URB_INT_MAX (INT_MAX - 128)
#define URB_INT_MIN (INT_MIN + 128)  // INT32_MIN + 128, para segurança

// SPECIAL OPCODES
#define OP_CALL       (INT_MAX -  0)
#define OP_GOIF       (INT_MAX -  1)
#define OP_DROP       (INT_MAX -  2)

// LIST OPCODES
#define OP_NEW        (INT_MAX -  3)
#define OP_FREE       (INT_MAX -  4)
#define OP_PUSH       (INT_MAX -  5)
#define OP_POP        (INT_MAX -  6)
#define OP_UNSHIFT    (INT_MAX -  7)
#define OP_SHIFT      (INT_MAX -  8)
#define OP_INSERT     (INT_MAX -  9)
#define OP_REMOVE     (INT_MAX -  10)
#define OP_SWAP       (INT_MAX -  11)
#define OP_SET        (INT_MAX -  12)
#define OP_GET        (INT_MAX -  13)

// BASIC MATH OPCODES
#define OP_ADD        (INT_MAX -  14)
#define OP_SUB        (INT_MAX -  15)
#define OP_BIT_AND    (INT_MAX -  16)
#define OP_BIT_OR     (INT_MAX -  17)
#define OP_BIT_XOR    (INT_MAX -  18)
#define OP_BIT_LS     (INT_MAX -  19)
#define OP_BIT_RS     (INT_MAX -  20)
#define OP_BIT_NOT    (INT_MAX -  21)

// ALIASES
#define ALIAS_CONTEXT (INT_MAX -  22)
#define ALIAS_STACK   (INT_MAX -  23)
#define ALIAS_CODE    (INT_MAX -  24)
#define ALIAS_UINT    (INT_MAX -  25)
#define ALIAS_STRING  (INT_MAX -  26)

static inline List* urb_preprocess(char* input_str)
{
    List* code = urb_new(URB_DEFAULT_SIZE);

    // Duplicate the input string to avoid modifying the original
    char* token = strtok(input_str, "\n\t \r");
    
    while (token != NULL)
    {
        if(token[1] == 'b')
        {
            urb_push(code, (Value){.i = strtol(token + 2, NULL, 2)});
        }
        else if(token[1] == 'x')
        {
            urb_push(code, (Value){.i = strtol(token + 2, NULL, 16)});
        }
        else if(token[1] == 'o')
        {
            urb_push(code, (Value){.i = strtol(token + 2, NULL, 8)});
        }
        else if(strchr(token, 'f'))
        {
            urb_push(code, (Value){.f = strtod(token, NULL)});
        }
        else if(strchr(token, 'u'))
        {
            urb_push(code, (Value){.i = ALIAS_UINT});
            urb_push(code, (Value){.u = strtoul(token, NULL, 10)});
        }
        else
        {
            urb_push(code, (Value){.u = strtol(token, NULL, 10)});
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
        if(code->data[i].i >= INT_MAX - 127)
        {
            switch(code->data[i].i)
            {
                case ALIAS_CONTEXT:
                {
                    urb_push(stack, (Value){.p = context});
                }
                break;
                case ALIAS_STACK:
                {
                    urb_push(stack, (Value){.p = stack});
                }
                break;
                case ALIAS_CODE:
                {
                    urb_push(stack, (Value){.p = code});
                }
                break;
                case ALIAS_UINT:
                {
                    // next value will be interpreted as unsigned integer 
                    urb_push(stack, (Value){.u = code->data[i + 1].u});

                    // we must skip the next token as we already processed it
                    i++;
                }
                break;
                case ALIAS_STRING:
                {
                    void* str = &(code->data[0].i);
                    Int elements = strlen(str);
                    while (elements % 8 != 0)
                    {
                        elements++;
                    }
                    elements = elements/8;

                    List *list = urb_new(URB_DEFAULT_SIZE);
                    for (Int j = 0; j < elements; j++)
                    {
                        urb_push(list, code->data[i + j]);
                    }
                    urb_push(stack, (Value){.p = list});
                    i += elements;
                }
                break;
                case OP_GOIF:
                {
                    if (urb_pop(stack).i)
                    {
                        i = urb_pop(stack).i;
                    }
                }
                break;

                case OP_CALL:
                {
                    // in C99 you cant cast a function pointer from a void*
                    urb_pop(stack).fn(stack);
                }
                break;

                // list operators
                case OP_NEW:
                {
                    urb_push(stack, (Value){.p = urb_new(URB_DEFAULT_SIZE)});
                }
                break;

                case OP_FREE:
                {
                    urb_free(urb_pop(stack).p);
                }
                break;

                case OP_PUSH:
                {
                    urb_push(urb_pop(stack).p, urb_pop(stack));
                }
                break;

                case OP_POP:
                {
                    urb_push(stack, urb_pop(urb_pop(stack).p));
                }
                break;

                case OP_UNSHIFT:
                {
                    urb_unshift(urb_pop(stack).p, urb_pop(stack));
                }
                break;

                case OP_SHIFT:
                {
                    urb_push(stack, urb_shift(urb_pop(stack).p));
                }
                break;

                case OP_INSERT:
                {
                    urb_insert(urb_pop(stack).p, urb_pop(stack).i, urb_pop(stack));
                }
                break;

                case OP_REMOVE:
                {
                    urb_push(stack, urb_remove(urb_pop(stack).p, urb_pop(stack).i));
                }
                break;

                case OP_SWAP:
                {
                    urb_swap(urb_pop(stack).p, urb_pop(stack).i, urb_pop(stack).i);
                }
                break;

                case OP_SET:
                {
                    urb_set(urb_pop(stack).p, urb_pop(stack).i, urb_pop(stack));
                }
                break;

                case OP_GET:
                {
                    List* list = urb_pop(stack).p;
                    Int index = urb_pop(stack).i;
                    urb_push(stack, urb_get(list, index));
                }
                break;

                // basic math operators
                case OP_BIT_AND:
                {
                    urb_push(stack, (Value){.i = urb_pop(stack).i & urb_pop(stack).i});
                }
                break;

                case OP_BIT_OR:
                {
                    urb_push(stack, (Value){.i = urb_pop(stack).i | urb_pop(stack).i});
                }
                break;

                case OP_BIT_XOR:
                {
                    urb_push(stack, (Value){.i = urb_pop(stack).i ^ urb_pop(stack).i});
                }
                break;

                case OP_BIT_LS:
                {
                    urb_push(stack, (Value){.i = urb_pop(stack).i << urb_pop(stack).i});
                }
                break;

                case OP_BIT_RS:
                {
                    urb_push(stack, (Value){.i = urb_pop(stack).i >> urb_pop(stack).i});
                }
                break;

                case OP_BIT_NOT:
                {
                    urb_push(stack, (Value){.i = ~urb_pop(stack).i});
                }
                break;

                default:
                {
                    printf("'%ld' is either a unmapped opcode or a out-of-bounds integer.\n", code->data[i].i);
                }
                break;
            }
        }
        else 
        {
            urb_push(stack, code->data[i]);
        }
    }

    if (_stack == NULL) 
    {
        urb_free(stack); // free stack only if it was created here
    }
}

#endif // ifndef URB_H macro