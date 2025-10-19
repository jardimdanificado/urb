#include "urb.h"

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
    List* code = urb_pop(stack).p;
    urb_interpret(context, code, stack);
}