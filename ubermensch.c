// libub
// libub use no other libraries beside the standard C99 libraries
// if libub does not work on a platform, it is a bug, and should be reported.

#ifndef UBERMENSCH_H
#define UBERMENSCH_H 1

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <ctype.h>

// version
#define UBERMENSCH_VERSION "0.9.4"

typedef intptr_t Int;
typedef uintptr_t UInt;

#if INTPTR_MAX == INT64_MAX
    typedef double Float;
    typedef int32_t Half;
    typedef uint32_t UHalf;
#else
    typedef float Float;
    typedef int16_t Half;
    typedef uint16_t UHalf;
#endif

#ifndef UBERMENSCH_DEFAULT_SIZE
    #define UBERMENSCH_DEFAULT_SIZE 0
#endif

typedef struct ListMeta
{
    UHalf capacity;
    UHalf size;
} ListMeta;

// Value
typedef union Value
{
    // all types depend on the size of the pointer
    Int i;
    Float f;
    UInt u;
    void* p;
    uint8_t b[sizeof(void*)];
    ListMeta meta;
} Value;

typedef Value* List;
typedef void (*Function)(List *stack);

#define list_size(list) list[0].meta.size
#define list_capacity(list) list[0].meta.capacity
#define list_size_check(list) list = (list_size(list) == list_capacity(list)) ? realloc(list, 1 + (list[0].meta.capacity*2)) : list

static inline List   create();
static inline List   push(List list, Value value);
static inline List   unshift(List list, Value value);
static inline List   insert(List list, UHalf index, Value value);
static inline Value  pop(List list);
static inline Value  shift(List list);
static inline Value  remove(List list, UHalf index);
static inline List   swap(List list, UHalf index_a, UHalf index_b);

static inline List create()
{
    List result = (List)malloc(1);
    result[0].meta.capacity = 0;
    result[0].meta.size = 0;
    return result;
}

static inline List push(List list, Value value)
{
    list_size_check(list);
    list[list[0].meta.size] = value;
    return list;
}

static inline Value pop(List list)
{
    list_size(list) -= 1;
    return list[list_size(list) + 1];
}

static inline List unshift(List list, Value value)
{
    list_size_check(list);
    memmove(&list[2], &list[1], list_size(list));
    return list;
}

static inline Value unshift(List list)
{
    Value result = list[1];
    memmove(&list[1], &list[2], list_size(list) - 1);
    return result;
}

//static inline List insert(List list, )

#endif // UBERMENSCH_H

int main()
{
    printf("sizze of meta: %d\n", sizeof(List));
    return 0;
}