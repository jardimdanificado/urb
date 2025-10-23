#include "urb.h"

void print_string(List* stack)
{
    List *ls = urb_pop(stack).p;
    if (ls->capacity == 0)
    {
        printf("NULL");
        return;
    }
    char *str = (char*)ls->data;
    printf("%s", str);
}

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

void scan(List* stack)
{
    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        // Remove newline character if present
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
        }
        urb_push(stack, (Value){.p = strdup(buffer)});
    }
    else
    {
        urb_push(stack, (Value){.p = NULL});
    }
}