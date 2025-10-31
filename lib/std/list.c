#include "urb.h"

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

void URB_push(List* stack)
{
    List* list = urb_pop(stack).p;
    Value value = urb_pop(stack);
    urb_push(list, value);
}

void URB_pop(List* stack)
{
    List* list = urb_pop(stack).p;
    urb_push(stack, urb_pop(list));
}

void URB_shift(List* stack)
{
    List* list = urb_pop(stack).p;
    urb_push(stack, urb_remove(list, 0));
}

void URB_unshift(List* stack)
{
    List* list = urb_pop(stack).p;
    Value value = urb_pop(stack);
    urb_insert(list, 0, value);
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

void URB_clear(List* stack)
{
    List* list = urb_pop(stack).p;
    list->size = 0;
}

void URB_peek(List* stack)
{
    List* list = urb_pop(stack).p;
    urb_push(stack, list->data[list->size - 1]);
}

void URB_swap(List* stack)
{
    List* list = urb_pop(stack).p;
    Int index_b = urb_pop(stack).i;
    Int index_a = urb_pop(stack).i;

    if (index_a < 0 || index_a >= list->size ||
        index_b < 0 || index_b >= list->size)
    {
        PANIC("cannot swap out-of-bounds elements.");
        return;
    }

    Value tmp = list->data[index_a];
    list->data[index_a] = list->data[index_b];
    list->data[index_b] = tmp;

    // mantém consistência: devolve a lista no topo da pilha
    urb_push(stack, (Value){.p = list});
}
