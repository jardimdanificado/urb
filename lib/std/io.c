#include "urb.h"

void print(List* stack)
{
    Int value = urb_pop(stack).i;
    printf("%ld", value);
}

void println(List* stack)
{
    Int value = urb_pop(stack).i;
    printf("%ld\n", value);
}

void pring(List* stack)
{
    List* list = urb_pop(stack).p;
    Int index = urb_pop(stack).i;
    printf("%s", &list->data[index]);
}

void pringln(List* stack)
{
    List* list = urb_pop(stack).p;
    Int index = urb_pop(stack).i;
    printf("%s\n", &list->data[index]);
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