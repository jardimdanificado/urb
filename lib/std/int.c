#include "urb.h"

void urb_equals(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a == b ? 1 : 0});
}

void urb_not_equals(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a != b ? 1 : 0});
}

void urb_greater(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a > b ? 1 : 0});
}

void urb_greater_equal(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a >= b ? 1 : 0});
}

void urb_less(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a < b ? 1 : 0});
}

void urb_less_equal(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a <= b ? 1 : 0});
}

