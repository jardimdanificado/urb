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

typedef union Value Value;
typedef struct List List;
// typedef void (*Function)(Value *stack);

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
    // Function fn;
};

struct List
{
    UInt capacity;
    UInt size;
    Value data[0];
};

List* urb_new()
{
    List* result = malloc(sizeof(Int)*2);
    result->capacity = 0;
    result->size = 0;
    return result;
}

List* urb_push(List* list, Value value)
{
    if(list->capacity == list->size)
    {
        list = list->size == 0 ? 1 : realloc(list, list->capacity *= 2);
    }
    list->data[list->size++] = value;
    return list;
}

Value urb_pop(List* list)
{
    return list->data[--list->size];
}

#endif

int main()
{
    List *context = urb_new();


    
}