#include "urb.h"

void println(List* stack)
{
    Int value = urb_pop(stack).i;
    printf("%ld\n", value);
}

void ls(List* stack)
{
    // [index, type, "name"] = value;
    List* list = urb_pop(stack).p;
    for (Int i = 0; i < list->size; i++)
    {
        printf("[%ld] = %ld\n", i, list->data[i].i);
    }
}