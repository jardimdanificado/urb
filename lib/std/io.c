#include "urb.h"

void print(List* stack)
{
    Int value = urb_pop(stack).i;
    printf("%ld", value);
}

void println(List* stack)
{
    printf("\n");
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