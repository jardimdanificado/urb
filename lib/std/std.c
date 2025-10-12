#include "urb.h"

// conditions
void urb_and(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a && b ? 1 : 0});
}

void urb_or(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a || b ? 1 : 0});
}

void urb_not(List* stack)
{
    Int a = urb_pop(stack).i;
    urb_push(stack, (Value){.i = !a ? 1 : 0});
}

// drop
void urb_drop(List* stack)
{
    urb_pop(stack);
}

// preprocess
void preprocess(List* stack)
{
    char* code = (char*)urb_pop(stack).p;
    List* processed_code = urb_preprocess(code);
    urb_push(stack, (Value){.p = processed_code});
}

// interpret
void interpret(List* stack)
{
    List* context = urb_pop(stack).p;
    List* code = (List*)urb_pop(stack).p;
    List* splited = urb_pop(stack).p;
    List* prog_stack = urb_pop(stack).p;
    urb_interpret(context, code, prog_stack);
}