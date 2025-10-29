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
// note that int_min is -int_max instead -int_max-1
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
// resize
static inline List*              urb_resize(List *list, Int new_size);
// copy
static inline List*              urb_copy(const List *list);
// ub representation
static inline void               urb_interpret(List *exec, List* code, List* _stack);

#define INDEX_CYCLE(index) ((index < 0) ? (list->size + index) : index)

// functions implementations
// functions implementations
// functions implementations
// functions implementations

static inline List *urb_new(Int size)
{
    List *list = (List*)malloc(sizeof(List));
    list->data = (size == 0) ? NULL : (Value*)malloc((size_t)size * sizeof(Value));
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
    list->capacity = list->capacity == 0 ? 1 : list->capacity * 2;
    Value *new_data = (Value*)realloc(list->data, (size_t)list->capacity * sizeof(Value));
    list->data = new_data;
}

static inline void urb_half(List *list)
{
    list->capacity /= 2;
    Value *new_data = (Value*)realloc(list->data, (size_t)list->capacity * sizeof(Value));
    list->data = new_data;

    if (list->size > list->capacity)
        list->size = list->capacity;
}

static inline void urb_push(List *list, Value value)
{
    if (list->size == list->capacity)
        urb_double(list);
    list->data[list->size] = value;
    list->size++;
}

static inline void urb_unshift(List *list, Value value)
{
    if (list->size == list->capacity)
        urb_double(list);
    memmove(&(list->data[1]), &(list->data[0]), (size_t)list->size * sizeof(Value));
    list->data[0] = value;

    list->size++;
}

static inline void urb_insert(List *list, Int index, Value value)
{
    Int original_index = index;
    if (list->size == list->capacity)
        urb_double(list);

    index = INDEX_CYCLE(index);
    
    switch(original_index)
    {
        case -1:
            urb_push(list, value);
        break;
        case 0:
            urb_unshift(list, value);
        break;
        default:
            memmove(&(list->data[index + 1]), &(list->data[index]), (size_t)(list->size - index) * sizeof(Value));
            list->data[index] = value;
            list->size++;
        break;
    }
}

static inline Value urb_pop(List *list)
{
    return list->data[--list->size];
}

static inline Value urb_shift(List *list)
{
    Value ret = list->data[0];
    memmove(&(list->data[0]), &(list->data[1]), (size_t)(list->size - 1) * sizeof(Value)); 
    list->size--; 
    return ret;
}

static inline Value urb_remove(List *list, Int i)
{
    Int original_index = i;
    i = INDEX_CYCLE(i);
    
    switch(original_index)
    {
        case -1:
            return urb_pop(list);
        break;
        case 0:
            return urb_shift(list);
        break;
        default:
        {
            Value ret = list->data[i];
            Int elements_to_move = list->size - (i) - 1;
            memmove(&(list->data[i]), &(list->data[i + 1]), elements_to_move * sizeof(Value)); 
            list->size--; 
            return ret;
        }
        break;
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

static inline List* urb_resize(List *list, Int new_size)
{
    if(new_size < 0)
    {
        urb_free(list);
        return NULL;
    }
    else if(new_size == 0)
    {
        free(list->data);
        list->data = NULL;
        list->capacity = 0;
    }
    else if(new_size < list->capacity/2)
    {
        while(new_size < list->capacity/2)
        {
            urb_half(list);
        }
    }
    else if(new_size > list->capacity)
    {
        while(new_size > list->capacity)
        {
            urb_double(list);
        }
    }
    list->size = new_size;
    return list;
}

// if you want to return something, pass a stack, values will be there
// if you do not provide a stack, a new one will be created and freed at the end
static inline void urb_interpret(List *exec, List* code, List* _stack)
{
    List *stack;
    stack = (_stack == NULL) ? urb_new(URB_DEFAULT_SIZE) : _stack;

    List* mem = urb_copy(code);
    urb_unshift(mem, (Value){.p = stack});
    urb_unshift(mem, (Value){.p = exec});
    urb_unshift(mem, (Value){.p = mem});

    // i might think about putting a pointer to the original code
    //urb_unshift(mem, (Value){.p = code});

    // interpreting
    for (Int i = 0; i < code->size; i++)
    {
        if(code->data[i].i < INT_MIN + exec->size)
        {
            // jif operator
            if (code->data[i].i == INT_MIN)
            {
                Int cond = urb_pop(stack).i;
                Int posit = urb_pop(stack).i - 1;
                i = cond ? posit : i;
            }
            else
            {
                // - 1 because we compensate the INT_MIN bein the jif operator
                exec->data[INT_MIN + code->data[i].i - 1].fn(stack);
            }
        }
        else if(code->data[i].i > INT_MAX - mem->size)
        {
            urb_push(stack, mem->data[INT_MAX - code->data[i].i]);
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
    
    urb_free(mem); // free mem only if it was created here
}

#endif // ifndef URB_H macro