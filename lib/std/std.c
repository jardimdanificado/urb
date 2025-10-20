#include "urb.h"

// preprocess
void URB_preprocess(List* stack)
{
    char* code = (char*)urb_pop(stack).p;
    List* processed_code = urb_preprocess(code);
    urb_push(stack, (Value){.p = processed_code});
}

// interpret
void URB_interpret(List* stack)
{
    List* context = urb_pop(stack).p;
    List* code = urb_pop(stack).p;
    urb_interpret(context, code, stack);
}

void URB_pack(List* stack)
{
    Int count = urb_pop(stack).i;
    List* new_list = urb_new(URB_DEFAULT_SIZE);
    for (Int i = 0; i < count; i++)
    {
        urb_push(new_list, urb_pop(stack));
    }
    urb_push(stack, (Value){.p = new_list});
}

void URB_unpack(List* stack)
{
    List* list = urb_pop(stack).p;
    while(list->size > 0)
    {
        urb_push(stack, urb_pop(list));
    }
}