#include "urb.h"

void URB_add(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a + b});
}

void URB_sub(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a - b});
}

void URB_mul(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a * b});
}

void URB_div(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    if (a == 0) {
        urb_push(stack, (Value){.i = 0}); // evitar divisão por zero
        return;
    }
    urb_push(stack, (Value){.i = a / b});
}

void URB_mod(List* stack)
{
    Int a = urb_pop(stack).i;
    Int b = urb_pop(stack).i;
    if (a == 0) {
        urb_push(stack, (Value){.i = 0});
        return;
    }
    urb_push(stack, (Value){.i = a % b});
}

void URB_neg(List* stack)
{
    Int a = urb_pop(stack).i;
    urb_push(stack, (Value){.i = -a});
}

void URB_inc(List* stack)
{
    Int a = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a + 1});
}

void URB_dec(List* stack)
{
    Int a = urb_pop(stack).i;
    urb_push(stack, (Value){.i = a - 1});
}

void URB_abs(List* stack)
{
    Int a = urb_pop(stack).i;
    urb_push(stack, (Value){.i = (a < 0) ? -a : a});
}

void URB_pow(List* stack)
{
    Int exp = urb_pop(stack).i;
    Int base = urb_pop(stack).i;
    Int res = 1;
    for (Int i = 0; i < exp; i++)
        res *= base;
    urb_push(stack, (Value){.i = res});
}
