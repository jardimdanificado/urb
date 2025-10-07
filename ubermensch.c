// libub
// libub use no other libraries beside the standard C99 libraries
// if libub does not work on a platform, it is a bug, and should be reported.

#ifndef UB_H
#define UB_H 1

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <ctype.h>

// version
#define UB_VERSION "0.9.4"

typedef intptr_t Int;
typedef uintptr_t UInt;

// UB use Int and Float instead of int and float because we need to use always the pointer size for any type that might share the fundamental union type;
// UB use a union as universal type, and UB is able to manipulate and use pointers direcly so we need to use the pointer size;
#if INTPTR_MAX == INT64_MAX
    typedef double Float;
    typedef uint32_t UHalf;
    typedef int32_t Half;
#else
    typedef float Float;
    typedef uint16_t UHalf;
    typedef int16_t Half;
#endif

#ifndef UB_DEFAULT_SIZE
    #define UB_DEFAULT_SIZE 0
#endif

typedef union Value
{
    // all types depend on the size of the pointer
    Int i;
    Float f;
    UInt u;
    void* p;
    uint8_t b[sizeof(void*)];
} Value;

typedef struct List
{
    // the real size
    UHalf capacity;
    // the current size
    UHalf size;
    // the data, it is a pointer to a Value array
    Value *data;
} List;

typedef void (*Function)(List *stack);
   
// List functions   
// create a new list with the given size, if size is 0, it will be initialized with NULL data and then allocated when needed
static inline List*            ub_new(Int size);
// free the list    
static inline void               ub_free(List *list);
// double the list capacity   
static inline void               ub_double(List *list);
// halve the list capacity   
static inline void               ub_half(List *list);
// push a value to the end of the list
static inline void               ub_push(List *list, Value value);
// unshift a value to the start of the list
static inline void               ub_unshift(List *list, Value value);
// insert a value at index i in the list
static inline void               ub_insert(List *list, Int i, Value value);
// pop a value from the end of the list
static inline Value            ub_pop(List *list);
// shift a value from the start of the list
static inline Value            ub_shift(List *list);
// remove and return the value at index i in the list, shifting the rest of the list
static inline Value            ub_remove(List *list, Int i);
// fast remove a value at index i in the list, swapping it with the last element and popping it
static inline Value            ub_fast_remove(List *list, Int i);
// swap two values in the list at indices i1 and i2
static inline void               ub_swap(List *list, Int i1, Int i2);
// find the index of a value in the list, returns -1 if not found
static inline Int              ub_find(const List *list, Value value);
// reverse the list in place   
static inline void               ub_reverse(List *list);
// create a copy of the list, with the same capacity and size, but new data array
static inline List*            ub_copy(const List *list);
// get a value at index i in the list, returns a value with i set to -1 if index is out of range
static inline Value            ub_get(const List *list, Int i);
// set a value at index i in the list, if index is out of range, it will print an error and exit
static inline void               ub_set(List *list, Int i, Value value);
// get the ub version   
static inline const char*        ub_get_version(void);
// arena   
static inline void*              ub_alloc(List *arena, size_t size);
// ub representation
static inline void               ub_interpret(List *context, const char* input_str, List* code, List* stack);

// functions implementations
// functions implementations
// functions implementations
// functions implementations

static inline List *ub_new(Int size)
{
    List *list = (List*)malloc(sizeof(List));
    
    if (list == NULL)
    {
        printf("UB_ERROR: failed to allocate memory for List\n");
        exit(EXIT_FAILURE);
    }
    
    list->data = (size == 0) ? NULL : (Value*)malloc((size_t)size * sizeof(Value));
    
    if (size > 0 && list->data == NULL)
    {
        printf("UB_ERROR: failed to allocate memory for List data\n");
        exit(EXIT_FAILURE);
    }
    
    list->size = 0;
    list->capacity = size;

    return list;
}

static inline void ub_free(List *list)
{
    free(list->data);
    free(list);
}

static inline void ub_double(List *list)
{
    Value *new_data = NULL;
    list->capacity = list->capacity == 0 ? 1 : list->capacity * 2;
    new_data = (Value*)realloc(list->data, (size_t)list->capacity * sizeof(Value));
    if (!new_data)
    {
        printf("UB_ERROR: failed to reallocate list data\n");
        exit(EXIT_FAILURE);
    }
    list->data = new_data;
}

static inline void ub_half(List *list)
{
    Value *new_data = NULL;
    list->capacity /= 2;
    new_data = (Value*)realloc(list->data, (size_t)list->capacity * sizeof(Value));
    if (!new_data)
    {
        printf("UB_ERROR: failed to reallocate list data\n");
        exit(EXIT_FAILURE);
    }
    list->data = new_data;

    if (list->size > list->capacity)
    {
        list->size = list->capacity;
    }
}

static inline void ub_push(List *list, Value value)
{
    if (list->size == list->capacity)
    {
        ub_double(list);
    }
    list->data[list->size] = value;
    list->size++;
}

static inline void ub_unshift(List *list, Value value)
{
    if (list->size == list->capacity)
    {
        ub_double(list);
    }
    memmove(&(list->data[1]), &(list->data[0]), (size_t)list->size * sizeof(Value));
    list->data[0] = value;

    list->size++;
}

static inline void ub_insert(List *list, Int i, Value value)
{
    if (list->size == list->capacity)
    {
        ub_double(list);
    }

    if (i < 0 || i > list->size)
    {
        printf("UB_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
    else
    {
        memmove(&(list->data[i + 1]), &(list->data[i]), (size_t)(list->size - i) * sizeof(Value));
        list->data[i] = value;
        list->size++;
    }
}

static inline Value ub_pop(List *list)
{
    Value ret = list->data[list->size - 1]; 
    if (list->size == 0)
    {
        printf("UB_ERROR: cannot pop from empty list\n");
        exit(EXIT_FAILURE);
    }
    list->size--;
    return ret;
}

static inline Value ub_shift(List *list)
{
    Value ret = list->data[0];
    if (list->size == 0)
    {
        printf("UB_ERROR: cannot shift from empty list\n");
        exit(EXIT_FAILURE);
    }
    else if (list->size > 1) 
    { 
        memmove(&(list->data[0]), &(list->data[1]), (size_t)(list->size - 1) * sizeof(Value)); 
    } 
    list->size--; 
    return ret;
}

static inline Value ub_remove(List *list, Int i)
{
    Value ret = list->data[i]; 

    size_t elements_to_move = 0; 
    if (list->size == 0)
    {
        printf("UB_ERROR: cannot pop from empty list\n");
        exit(EXIT_FAILURE);
    }
    elements_to_move = (size_t)(list->size - (i) - 1); 
    
    /* Move data elements */ 
    memmove(&(list->data[i]), &(list->data[i + 1]), 
            elements_to_move * sizeof(Value)); 
    list->size--; 
    
    return ret;
}

static inline Value ub_fast_remove(List *list, Int i)
{
    ub_swap(list, i, list->size - 1);
    return ub_pop(list);
}

static inline void ub_swap(List *list, Int i1, Int i2)
{
    Value tmp = list->data[i1];
    list->data[i1] = list->data[i2];
    list->data[i2] = tmp;
}

static inline Int ub_find(const List *list, Value value) 
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

static inline void ub_reverse(List *list)
{
    for (Int i = 0; i < list->size / 2; i++)
    {
        ub_swap(list, i, list->size - i - 1);
    }
}

static inline List* ub_copy(const List *list)
{
    List *copy = ub_new(list->capacity);
    
    if (copy == NULL)
    {
        printf("UB_ERROR: failed to allocate memory for List copy\n");
        exit(EXIT_FAILURE);
    }
    copy->size = list->size;
    memcpy(copy->data, list->data, (size_t)copy->size * sizeof(Value));
    
    copy->capacity = list->capacity;

    return copy;
}

static inline Value ub_get(const List *list, Int i)
{
    if (i < 0 || i >= list->size)
    {
        return (Value){.i=-1}; // return -1 if index is out of range
    }
    return list->data[i];
}

static inline void ub_set(List *list, Int i, Value value)
{
    if (i < 0 || i >= list->size)
    {
        printf("UB_ERROR: index %" PRIdPTR " out of range in list of size %" PRIdPTR " \n", i, list->size);
        exit(EXIT_FAILURE);
    }
    list->data[i] = value;
}

static inline const char* ub_get_version(void)
{
    return UB_VERSION;
}

static inline void* ub_alloc(List* arena, size_t size)
{
    size_t aligned_size = (size + sizeof(void*) - 1) & ~(sizeof(void*) - 1);
    // we use arena->size as offset
    if (arena->size + aligned_size > arena->capacity)
    {
        ub_double(arena);
    }
    void *ptr = arena->data + arena->size;
    arena->size += aligned_size;
    return ptr;
}

// if you want to return something, pass a stack, values will be there
// if you do not provide a stack, a new one will be created and freed at the end
static inline void ub_interpret(List *context, const char* input_str, List* _code, List* _stack)
{
    List *code;
    List *stack;
    Int i = 0;
    char* original_str = NULL;


    if (_stack == NULL)
    {
        stack = ub_new(UB_DEFAULT_SIZE);
    }
    else
    {
        stack = _stack;
    }

    if (_code == NULL)
    {
        code = ub_new(UB_DEFAULT_SIZE);
    }
    else
    {
        code = _code;
    }

    // interpreting goes here
    // interpreting goes here
    // interpreting goes here
    // interpreting goes here
    // interpreting goes here
    // interpreting goes here
    // interpreting goes here
    // interpreting goes here
    // interpreting goes here

    if (_code == NULL) 
        ub_free(code); // free code only if it was created here
    if (_stack == NULL) 
        ub_free(stack); // free stack only if it was created here

    free(original_str); // free the original string
}

#endif // ifndef UB_H macro