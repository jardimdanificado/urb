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
#define list_size_check(list) \
    if (list_size(list) == list_capacity(list))\
    {\
        if (list_capacity(list) == 0)\
        {\
            list_capacity(list) = 1;\
        }\
        else \
        {\
            list_capacity(list) *= 2;\
        }\
        list = realloc(list, (list_capacity(list) + 1) * sizeof(Int));\
    }

#define VALUE(t, value) (Value){.t = value}

static inline List   ubermensch_create();
static inline List   ubermensch_push(List list, Value value);
static inline List   ubermensch_unshift(List list, Value value);
static inline List   ubermensch_attach(List list, UHalf index, Value value);
static inline Value  ubermensch_pop(List list);
static inline Value  ubermensch_shift(List list);
static inline Value  ubermensch_dettach(List list, UHalf index);
static inline void   ubermensch_swap(List list, UHalf index_a, UHalf index_b);

static inline List ubermensch_create()
{
    List result = (List)malloc(sizeof(Int));
    list_capacity(result) = 0;
    list_size(result) = 0;
    return result;
}

static inline List ubermensch_push(List list, Value value)
{
    list_size_check(list);
    list[list_size(list) + 1] = value;
    list_size(list)++;
    return list;
}

static inline List ubermensch_unshift(List list, Value value)
{
    list_size_check(list);
    memmove(&list[2], &list[1], list_size(list) * sizeof(Int));
    list[1] = value;
    list_size(list)++;
    return list;
}

static inline List ubermensch_attach(List list, UHalf index, Value value)
{
    list_size_check(list);
    memmove(&list[index + 2], &list[index + 1], (list_size(list) - (index + 1)) * sizeof(Int));
    list[index + 1] = value;
    list_size(list)++;
    return list;
}

static inline Value ubermensch_pop(List list)
{
    list_size(list) -= 1;
    return list[list_size(list) + 1];
}

static inline Value ubermensch_shift(List list)
{
    Value result = list[1];
    memmove(&list[1], &list[2], (list_size(list) - 1) * sizeof(Int));
    list_size(list)--;
    return result;
}

static inline Value ubermensch_dettach(List list, UHalf index)
{
    Value result = list[index + 1];
    memmove(&list[index + 1], &list[index + 2], (list_size(list) - (index + 1)) * sizeof(Int));
    list_size(list)--;
    return result;
}

static inline void ubermensch_swap(List list, UHalf index_a, UHalf index_b)
{
    Value a = list[index_a + 1];
    list[index_a + 1] = list[index_b + 1];
    list[index_b + 1] = a;
}

#define ub_new() ubermensch_create()
#define ub_push(list, value) list = ubermensch_push(list, value)
#define ub_unshift(list, value) list = ubermensch_unshift(list, value)
#define ub_insert(list, index, value) list = ubermensch_attach(list, index, value)
#define ub_pop(list) ubermensch_pop(list)
#define ub_shift(list) ubermensch_shift(list)
#define ub_remove(list, index) ubermensch_dettach(list, index)
#define ub_swap(list, index) ubermensch_swap(list, index_a, index_b)

#endif // UBERMENSCH_H

int main()
{
    List l = ub_new();
    ub_push(l, VALUE(i, 55));
    ub_push(l, VALUE(i, 57));
    ub_push(l, VALUE(i, 55));
    ub_push(l, VALUE(i, 57));
    ub_push(l, VALUE(i, 56));
    ub_push(l, VALUE(i, 54));
    ub_push(l, VALUE(i, 53));
    ub_push(l, VALUE(i, 52));

    
    printf("popped: %d\n", ub_pop(l).i); // 52
    printf("popped: %d\n", ub_pop(l).i); // 53
    printf("removed: %d\n", ub_remove(l, 1).i); // 57
    printf("shift: %d\n", ub_shift(l).i); // 55
    printf("list size: %d\n", list_size(l)); // 4
    
    // 55, 57, 56, 54
    
    ub_insert(l, 2, VALUE(i, 99));
    // 55, 57, 99, 56, 54

    ub_unshift(l, VALUE(i, 999));
    // 999, 55, 57, 99, 56, 54

    printf("popped: %d\n", ub_pop(l).i); // 54
    printf("shift: %d\n", ub_shift(l).i); // 999
    printf("popped: %d\n", ub_pop(l).i); // 56
    printf("popped: %d\n", ub_pop(l).i); // 99
    
    free(l);

    return 0;
}