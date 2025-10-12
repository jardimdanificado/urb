#include "urb.h"

void print_string(List* stack)
{
    char *str = urb_pop(stack).p;
    if (str == NULL)
    {
        printf("NULL");
        return;
    }
    printf("%s", str);
}

void print_int(List* stack)
{
    Int value = urb_pop(stack).i;
    printf("%ld", value);
}

void print_float(List* stack)
{
    Float value = urb_pop(stack).f;
    printf("%f", value);
}

void print_bool(List* stack)
{
    Int value = urb_pop(stack).i;
    printf(value ? "true" : "false");
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
        printf("[%ld] = %ld\n", i);
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