#include "urb.h"

void ieq(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a == b ? 1 : 0});
}

void ineq(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a != b ? 1 : 0});
}

void igt(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a > b ? 1 : 0});
}

void ige(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a >= b ? 1 : 0});
}

void ilt(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a < b ? 1 : 0});
}

void ile(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a <= b ? 1 : 0});
}

void iadd(List *stack)
{
    urb_push(stack, (Value){.i = (urb_pop(stack).i + urb_pop(stack).i)});
}

void isub(List *stack)
{
    urb_push(stack, (Value){.i = (urb_pop(stack).i - urb_pop(stack).i)});
}

void imul(List *stack)
{
    urb_push(stack, (Value){.i = (urb_pop(stack).i * urb_pop(stack).i)});
}

void idiv(List *stack)
{
    urb_push(stack, (Value){.i = (urb_pop(stack).i / urb_pop(stack).i)});
}

void imod(List *stack)
{
    urb_push(stack, (Value){.i = (urb_pop(stack).i % urb_pop(stack).i)});
}