// urb
// urb use no other libraries beside the standard C99 libraries
// if urb does not work on a platform, its propably a bug, and should be reported.

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

// we use unions here mainly for type punning
// its against the C standards, but almost every C compiler supports it
// the "correct" way of doing so would be using memcpy, but that also introduce some overhead
// under conditions of strict C standard i would not use unions at all, 
// instead i would store the values as uint64_t int64_t uint8_t[8] or such
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
// create a new list from a pointer
static inline List*              urb_from_raw(void* ptr, Int size);
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

// BASES
#define URB_INT_MAX (INT_MAX - 128)
#define URB_INT_MIN (INT_MIN + 128)

// if adding new opcodes and using the experimental debug system
// be sure to register the opcodes in scripts/opcodes.h too
enum {
    // code opcodes
    OP_JUMPIF = INT_MAX - 127,
    
    // list opcodes
    OP_CALL,
    OP_CALLIST,
    OP_NEW,
    OP_FREE,
    OP_PUSH,
    OP_POP,
    OP_UNSHIFT,
    OP_SHIFT,
    OP_INSERT,
    OP_REMOVE,
    OP_SWAP,
    OP_SET,
    OP_GET,
    OP_COPY,
    OP_LENGTH,
    OP_DOUBLE,
    OP_HALF,
    
    // stack ops
    OP_DUP,
    OP_DROP,

    // math opcodes
    OP_ADD,
    OP_SUB,
    OP_BIT_AND,
    OP_BIT_OR,
    OP_BIT_XOR,
    OP_BIT_LS,
    OP_BIT_RS,
    OP_BIT_NOT,
    
    // cmp opcoddes
    OP_EQUALS,
    OP_NOT_EQUALS,
    OP_GREATER,
    OP_GREATER_OR_EQUAL,
    OP_LESSER,
    OP_LESSER_OR_EQUAL,
    OP_AND,
    OP_OR,
    
    // buffer opcodes
    OP_WRITE,
    OP_READ,

    // alias
    ALIAS_CONTEXT,
    ALIAS_STACK,
    ALIAS_CODE,
    ALIAS_BYPASS,
    ALIAS_WORD_SIZE
};

#define INDEX_CYCLE(index) ((index < 0) ? (list->size + index) : index)

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

static inline List *urb_from_raw(void* ptr, Int size)
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

    i = INDEX_CYCLE(i);

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
    i = INDEX_CYCLE(i);
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
    i = INDEX_CYCLE(i);
    urb_swap(list, i, list->size - 1);
    return urb_pop(list);
}

static inline void urb_swap(List *list, Int i1, Int i2)
{
    i1 = INDEX_CYCLE(i1);
    i2 = INDEX_CYCLE(i2);
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
    i = INDEX_CYCLE(i);
    if (i < 0 || i >= list->size)
    {
        return (Value){.i=-1}; // return -1 if index is out of range
    }
    return list->data[i];
}

static inline void urb_set(List *list, Int i, Value value)
{
    i = INDEX_CYCLE(i);
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

static inline Value urb_token_preprocess(char* token)
{
    if(token[1] == 'b')
    {
        return((Value){.i = strtol(token + 2, NULL, 2)});
    }
    else if(token[1] == 'x')
    {
        return((Value){.i = strtol(token + 2, NULL, 16)});
    }
    else if(token[1] == 'o')
    {
        return((Value){.i = strtol(token + 2, NULL, 8)});
    }
    else if(strchr(token, 'f'))
    {
        return((Value){.f = strtod(token, NULL)});
    }
    else if(strchr(token, 'u'))
    {
        return((Value){.i = ALIAS_BYPASS});
        return((Value){.u = strtoul(token, NULL, 10)});
    }
    else
    {
        return((Value){.u = strtol(token, NULL, 10)});
    }
}

static inline List* urb_preprocess(char* input_str)
{
    List* code = urb_new(URB_DEFAULT_SIZE);

    // Duplicate the input string to avoid modifying the original
    char* token = strtok(input_str, "\n\t \r");
    
    while (token != NULL)
    {
        urb_push(code, urb_token_preprocess(token));
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
                // code ops
                case OP_JUMPIF:
                    i = (urb_pop(stack).i) ? urb_pop(stack).i : i;
                break;

                // list ops
                case OP_CALL:
                {
                    List* list = (List*)urb_pop(stack).p;
                    Int index = urb_pop(stack).i;
                    index = INDEX_CYCLE(index);
                    list->data[index].fn(stack);
                }
                break;
                case OP_CALLIST:
                    urb_interpret(context, urb_pop(stack).p, stack);
                break;
                case OP_NEW:
                    urb_push(stack, (Value){.p = urb_new(URB_DEFAULT_SIZE)});
                break;
                case OP_FREE:
                    urb_free(urb_pop(stack).p);
                break;
                case OP_PUSH:
                {
                    List* list = urb_pop(stack).p;
                    urb_push(list, urb_pop(stack));
                }
                break;
                case OP_POP:
                    urb_push(stack, urb_pop(urb_pop(stack).p));
                break;
                case OP_UNSHIFT:
                {
                    List* list = urb_pop(stack).p;
                    urb_unshift(list, urb_pop(stack));
                }
                break;
                case OP_SHIFT:
                    urb_push(stack, urb_shift(urb_pop(stack).p));
                break;
                case OP_INSERT:
                {
                    List* list = urb_pop(stack).p;
                    Int index = urb_pop(stack).i;
                    urb_insert(list, index, urb_pop(stack));
                }
                break;
                case OP_REMOVE:
                {
                    List* list = urb_pop(stack).p;
                    Int index = urb_pop(stack).i;
                    urb_push(stack, urb_remove(list, index));
                }
                break;
                case OP_SWAP:
                {
                    List* list = urb_pop(stack).p;
                    Int index1 = urb_pop(stack).i;
                    Int index2 = urb_pop(stack).i;
                    urb_swap(list, index1, index2);
                }
                break;
                case OP_SET:
                {
                    List* list = urb_pop(stack).p;
                    Int index = urb_pop(stack).i;
                    urb_set(list, index, urb_pop(stack));
                }
                break;
                case OP_GET:
                {
                    List* list = urb_pop(stack).p;
                    Int index = urb_pop(stack).i;
                    urb_push(stack, urb_get(list, index));
                }
                break;
                case OP_COPY:
                    urb_push(stack, (Value){.p = urb_copy(urb_pop(stack).p)});
                break;
                case OP_LENGTH:
                    urb_push(stack, (Value){.i = ((List*)urb_pop(stack).p)->size});
                break;
                case OP_DOUBLE:
                    urb_double((List*)urb_pop(stack).p);
                break;
                case OP_HALF:
                    urb_half((List*)urb_pop(stack).p);
                break;

                // stack ops
                case OP_DUP:
                {
                    Value v = urb_pop(stack);
                    urb_push(stack, v);
                    urb_push(stack, v);
                }
                break;
                case OP_DROP:
                    urb_pop(stack);
                break;

                // math ops
                case OP_ADD:
                {
                    Int a = urb_pop(stack).i;
                    Int b = urb_pop(stack).i;
                    urb_push(stack, (Value){.i = a + b});
                }
                break;
                case OP_SUB:
                {
                    Int a = urb_pop(stack).i;
                    Int b = urb_pop(stack).i;
                    urb_push(stack, (Value){.i = a - b});
                }
                break;
                case OP_BIT_AND:
                {
                    Int a = urb_pop(stack).i;
                    Int b = urb_pop(stack).i;
                    urb_push(stack, (Value){.i = a & b});
                }
                break;
                case OP_BIT_OR:
                {
                    Int a = urb_pop(stack).i;
                    Int b = urb_pop(stack).i;
                    urb_push(stack, (Value){.i = a | b});
                }
                break;
                case OP_BIT_XOR:
                {
                    Int a = urb_pop(stack).i;
                    Int b = urb_pop(stack).i;
                    urb_push(stack, (Value){.i = a ^ b});
                }
                break;
                case OP_BIT_LS:
                {
                    Int a = urb_pop(stack).i;
                    Int b = urb_pop(stack).i;
                    urb_push(stack, (Value){.i = a << b});
                }
                break;
                case OP_BIT_RS:
                {
                    Int a = urb_pop(stack).i;
                    Int b = urb_pop(stack).i;
                    urb_push(stack, (Value){.i = a >> b});
                }
                break;
                case OP_BIT_NOT:
                {
                    Int a = urb_pop(stack).i;
                    urb_push(stack, (Value){.i = ~a});
                }
                break;

                // cond ops
                case OP_EQUALS:
                {
                    Int a = urb_pop(stack).i;
                    Int b = urb_pop(stack).i;
                    urb_push(stack, (Value){.i = a == b});
                }
                break;
                case OP_NOT_EQUALS:
                {
                    Int a = urb_pop(stack).i;
                    Int b = urb_pop(stack).i;
                    urb_push(stack, (Value){.i = a != b});
                }
                break;
                case OP_GREATER:
                {
                    Int a = urb_pop(stack).i;
                    Int b = urb_pop(stack).i;
                    urb_push(stack, (Value){.i = a > b});
                }
                break;
                case OP_GREATER_OR_EQUAL:
                {
                    Int a = urb_pop(stack).i;
                    Int b = urb_pop(stack).i;
                    urb_push(stack, (Value){.i = a >= b});
                }
                break;
                case OP_LESSER:
                {
                    Int a = urb_pop(stack).i;
                    Int b = urb_pop(stack).i;
                    urb_push(stack, (Value){.i = a < b});
                }
                break;
                case OP_LESSER_OR_EQUAL:
                {
                    Int a = urb_pop(stack).i;
                    Int b = urb_pop(stack).i;
                    urb_push(stack, (Value){.i = a <= b});
                }
                break;
                case OP_AND:
                    {
                    Int a = urb_pop(stack).i;
                    Int b = urb_pop(stack).i;
                    urb_push(stack, (Value){.i = a && b});
                }
                break;
                case OP_OR:
                {
                    Int a = urb_pop(stack).i;
                    Int b = urb_pop(stack).i;
                    urb_push(stack, (Value){.i = a || b});
                }
                break;

                // byte ops
                case OP_WRITE:
                {
                    List* list = urb_pop(stack).p;
                    Int index = urb_pop(stack).i;
                    ((uint8_t*)list->data)[index] = urb_pop(stack).i;
                }
                break;
                case OP_READ:
                {
                    List* list = urb_pop(stack).p;
                    Int index = urb_pop(stack).i;
                    urb_push(stack, (Value){.i = ((uint8_t*)list->data)[index]});
                }
                break;

                // aliases
                case ALIAS_CONTEXT:
                    urb_push(stack, (Value){.p = context});
                break;
                case ALIAS_STACK:
                    urb_push(stack, (Value){.p = stack});
                break;
                case ALIAS_CODE:
                    urb_push(stack, (Value){.p = code});
                break;
                case ALIAS_BYPASS:
                    urb_push(stack, code->data[++i]);
                break;
                case ALIAS_WORD_SIZE:
                    urb_push(stack, (Value){.i = sizeof(Int)});
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