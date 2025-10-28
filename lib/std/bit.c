#include "urb.h"

void URB_band(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a & b});
}

void URB_bor(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a | b});
}

void URB_bxor(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a ^ b});
}

void URB_bnot(List* stack)
{
    Int a = urb_pop(stack).i;
    urb_push(stack, (Value){.i = ~a});
}

void URB_lshift(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a << b});
}

void URB_rshift(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a >> b});
}