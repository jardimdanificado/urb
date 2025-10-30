#include "urb.h"

void URB_stack(List* stack)
{
    urb_push(stack, (Value){.p = stack});
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

void URB_insert(List* stack)
{
    List* list = urb_pop(stack).p;
    Int index = urb_pop(stack).i;
    Value value = urb_pop(stack);
    urb_insert(list, index, value);
}

void URB_remove(List* stack)
{
    List* list = urb_pop(stack).p;
    Int index = urb_pop(stack).i;
    urb_push(stack, urb_remove(list, index));
}

void URB_set(List* stack)
{
    List* list = urb_pop(stack).p;
    Int index = urb_pop(stack).i;
    Value value = urb_pop(stack);
    list->data[index] = value;
}

void URB_get(List* stack)
{
    List* list = urb_pop(stack).p;
    Int index = urb_pop(stack).i;
    urb_push(stack, list->data[index]);
}

void URB_drop(List* stack)
{
    urb_pop(stack);
}

void URB_dup(List* stack)
{
    urb_push(stack, stack->data[stack->size - 1]);
}

void URB_len(List* stack)
{
    List* list = urb_pop(stack).p;
    urb_push(stack, (Value){.i = list->size});
}