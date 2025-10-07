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

typedef intptr_t UbInt;
typedef uintptr_t UbUInt;

// UB use UbInt and UbFloat instead of int and float because we need to use always the pointer size for any type that might share the fundamental union type;
// UB use a union as universal type, and UB is able to manipulate and use pointers direcly so we need to use the pointer size;
#if INTPTR_MAX == INT64_MAX
    typedef double UbFloat;
    typedef uint32_t UbUHalf;
    typedef int32_t UbHalf;
#else
    typedef float UbFloat;
    typedef uint16_t UbUHalf;
    typedef int16_t UbHalf;
#endif

#ifndef UB_DEFAULT_SIZE
    #define UB_DEFAULT_SIZE 0
#endif

typedef union UbValue
{
    // all types depend on the size of the pointer
    UbInt i;
    UbFloat f;
    UbUInt u;
    void* p;
    uint8_t b[sizeof(void*)];
} UbValue;

typedef struct UbList
{
    // the real size
    UbUHalf capacity;
    // the current size
    UbUHalf size;
    // the data, it is a pointer to a UbValue array
    UbValue *data;
} UbList;

typedef void (*Function)(UbList *stack);
   
// UbList functions   
// create a new list with the given size, if size is 0, it will be initialized with NULL data and then allocated when needed
static inline UbList*            ub_new(UbInt size);
// free the list    
static inline void               ub_free(UbList *list);
// double the list capacity   
static inline void               ub_double(UbList *list);
// halve the list capacity   
static inline void               ub_half(UbList *list);
// push a value to the end of the list
static inline void               ub_push(UbList *list, UbValue value);
// unshift a value to the start of the list
static inline void               ub_unshift(UbList *list, UbValue value);
// insert a value at index i in the list
static inline void               ub_insert(UbList *list, UbInt i, UbValue value);
// pop a value from the end of the list
static inline UbValue            ub_pop(UbList *list);
// shift a value from the start of the list
static inline UbValue            ub_shift(UbList *list);
// remove and return the value at index i in the list, shifting the rest of the list
static inline UbValue            ub_remove(UbList *list, UbInt i);
// fast remove a value at index i in the list, swapping it with the last element and popping it
static inline UbValue            ub_fast_remove(UbList *list, UbInt i);
// swap two values in the list at indices i1 and i2
static inline void               ub_swap(UbList *list, UbInt i1, UbInt i2);
// find the index of a value in the list, returns -1 if not found
static inline UbInt              ub_find(const UbList *list, UbValue value);
// reverse the list in place   
static inline void               ub_reverse(UbList *list);
// create a copy of the list, with the same capacity and size, but new data array
static inline UbList*            ub_copy(const UbList *list);
// get a value at index i in the list, returns a value with i set to -1 if index is out of range
static inline UbValue            ub_get(const UbList *list, UbInt i);
// set a value at index i in the list, if index is out of range, it will print an error and exit
static inline void               ub_set(UbList *list, UbInt i, UbValue value);
// get the ub version   
static inline const char*        ub_get_version(void);
// arena   
static inline void*              ub_alloc(UbList *arena, size_t size);
// ub representation
static inline void               ub_interpret(UbList *context, const char* input_str, UbList* code, UbList* stack);

// functions implementations
// functions implementations
// functions implementations
// functions implementations

static inline UbList *ub_new(UbInt size)
{
    UbList *list = (UbList*)malloc(sizeof(UbList));
    
    if (list == NULL)
    {
        printf("UB_ERROR: failed to allocate memory for UbList\n");
        exit(EXIT_FAILURE);
    }
    
    list->data = (size == 0) ? NULL : (UbValue*)malloc((size_t)size * sizeof(UbValue));
    
    if (size > 0 && list->data == NULL)
    {
        printf("UB_ERROR: failed to allocate memory for UbList data\n");
        exit(EXIT_FAILURE);
    }
    
    list->size = 0;
    list->capacity = size;

    return list;
}

static inline void ub_free(UbList *list)
{
    free(list->data);
    free(list);
}

static inline void ub_double(UbList *list)
{
    UbValue *new_data = NULL;
    list->capacity = list->capacity == 0 ? 1 : list->capacity * 2;
    new_data = (UbValue*)realloc(list->data, (size_t)list->capacity * sizeof(UbValue));
    if (!new_data)
    {
        printf("UB_ERROR: failed to reallocate list data\n");
        exit(EXIT_FAILURE);
    }
    list->data = new_data;
}

static inline void ub_half(UbList *list)
{
    UbValue *new_data = NULL;
    list->capacity /= 2;
    new_data = (UbValue*)realloc(list->data, (size_t)list->capacity * sizeof(UbValue));
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

static inline void ub_push(UbList *list, UbValue value)
{
    if (list->size == list->capacity)
    {
        ub_double(list);
    }
    list->data[list->size] = value;
    list->size++;
}

static inline void ub_unshift(UbList *list, UbValue value)
{
    if (list->size == list->capacity)
    {
        ub_double(list);
    }
    memmove(&(list->data[1]), &(list->data[0]), (size_t)list->size * sizeof(UbValue));
    list->data[0] = value;

    list->size++;
}

static inline void ub_insert(UbList *list, UbInt i, UbValue value)
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
        memmove(&(list->data[i + 1]), &(list->data[i]), (size_t)(list->size - i) * sizeof(UbValue));
        list->data[i] = value;
        list->size++;
    }
}

static inline UbValue ub_pop(UbList *list)
{
    UbValue ret = list->data[list->size - 1]; 
    if (list->size == 0)
    {
        printf("UB_ERROR: cannot pop from empty list\n");
        exit(EXIT_FAILURE);
    }
    list->size--;
    return ret;
}

static inline UbValue ub_shift(UbList *list)
{
    UbValue ret = list->data[0];
    if (list->size == 0)
    {
        printf("UB_ERROR: cannot shift from empty list\n");
        exit(EXIT_FAILURE);
    }
    else if (list->size > 1) 
    { 
        memmove(&(list->data[0]), &(list->data[1]), (size_t)(list->size - 1) * sizeof(UbValue)); 
    } 
    list->size--; 
    return ret;
}

static inline UbValue ub_remove(UbList *list, UbInt i)
{
    UbValue ret = list->data[i]; 

    size_t elements_to_move = 0; 
    if (list->size == 0)
    {
        printf("UB_ERROR: cannot pop from empty list\n");
        exit(EXIT_FAILURE);
    }
    elements_to_move = (size_t)(list->size - (i) - 1); 
    
    /* Move data elements */ 
    memmove(&(list->data[i]), &(list->data[i + 1]), 
            elements_to_move * sizeof(UbValue)); 
    list->size--; 
    
    return ret;
}

static inline UbValue ub_fast_remove(UbList *list, UbInt i)
{
    ub_swap(list, i, list->size - 1);
    return ub_pop(list);
}

static inline void ub_swap(UbList *list, UbInt i1, UbInt i2)
{
    UbValue tmp = list->data[i1];
    list->data[i1] = list->data[i2];
    list->data[i2] = tmp;
}

static inline UbInt ub_find(const UbList *list, UbValue value) 
{
    for (UbInt i = list->size - 1; i >= 0; i--)
    {
        if (list->data[i].i == value.i)
        {
            return i;
        }
    }

    return -1;
}

static inline void ub_reverse(UbList *list)
{
    for (UbInt i = 0; i < list->size / 2; i++)
    {
        ub_swap(list, i, list->size - i - 1);
    }
}

static inline UbList* ub_copy(const UbList *list)
{
    UbList *copy = ub_new(list->capacity);
    
    if (copy == NULL)
    {
        printf("UB_ERROR: failed to allocate memory for UbList copy\n");
        exit(EXIT_FAILURE);
    }
    copy->size = list->size;
    memcpy(copy->data, list->data, (size_t)copy->size * sizeof(UbValue));
    
    copy->capacity = list->capacity;

    return copy;
}

static inline UbValue ub_get(const UbList *list, UbInt i)
{
    if (i < 0 || i >= list->size)
    {
        return (UbValue){.i=-1}; // return -1 if index is out of range
    }
    return list->data[i];
}

static inline void ub_set(UbList *list, UbInt i, UbValue value)
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

static inline void* ub_alloc(UbList* arena, size_t size)
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
/*
// if you want to return something, pass a stack, values will be there
// if you do not provide a stack, a new one will be created and freed at the end
static inline void ub_interpret(UbList *context, const char* input_str, UbList* _code, UbList* _stack)
{
    UbList *code;
    UbList *stack;
    UbInt i = 0;
    char* original_str = NULL;

    // this will increment every label
    int difference = 0;
    if (_code == NULL)
    {
        code = ub_new(UB_DEFAULT_SIZE);
        original_str = strdup(input_str); // Duplicate the input string to avoid modifying the original
        char* token = strtok(original_str, "\n\t \r");
        while (token != NULL)
        {
            // we will also process the labels here
            if (token[0] == ':') // label
            {
                // we remove the label from the token
                char* label = token + 1; // skip the first character
                // we push the label as an integer (its position in the code)
                ub_push_int(context, i - difference, label, UB_TYPE_ANY);
                difference++;
            }
            else 
            {
                ub_push_pointer(code, token, NULL, UB_TYPE_BUFFER);
            }
            token = strtok(NULL, "\n\t \r");
            i++;
        }
    }
    else
    {
        code = _code;
    }

    if (_stack == NULL)
    {
        stack = ub_new(UB_DEFAULT_SIZE);
    }
    else
    {
        stack = _stack;
    }

    // interpreting goes here

    if (_code == NULL) 
        ub_free(code); // free code only if it was created here
    if (_stack == NULL) 
        ub_free(stack); // free stack only if it was created here

    free(original_str); // free the original string
}
*/
#endif // ifndef UB_H macro