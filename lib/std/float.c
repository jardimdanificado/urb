#include "urb.h"
#include "math.h"

void urb_feq(List* stack)
{
    Int a = urb_pop(stack).f;
    Int b = urb_pop(stack).f;
    urb_push(stack, (Value){.f = a == b ? 1 : 0});
}

void urb_fneq(List* stack)
{
    Int a = urb_pop(stack).f;
    Int b = urb_pop(stack).f;
    urb_push(stack, (Value){.f = a != b ? 1 : 0});
}

void urb_fgt(List* stack)
{
    Int a = urb_pop(stack).f;
    Int b = urb_pop(stack).f;
    urb_push(stack, (Value){.f = a > b ? 1 : 0});
}

void urb_fge(List* stack)
{
    Int a = urb_pop(stack).f;
    Int b = urb_pop(stack).f;
    urb_push(stack, (Value){.f = a >= b ? 1 : 0});
}

void urb_flt(List* stack)
{
    Int a = urb_pop(stack).f;
    Int b = urb_pop(stack).f;
    urb_push(stack, (Value){.f = a < b ? 1 : 0});
}

void urb_fle(List* stack)
{
    Int a = urb_pop(stack).f;
    Int b = urb_pop(stack).f;
    urb_push(stack, (Value){.f = a <= b ? 1 : 0});
}

void urb_fadd(List *stack)
{
    urb_push(stack, (Value){.f = (urb_pop(stack).f + urb_pop(stack).f)});
}

void urb_fsub(List *stack)
{
    urb_push(stack, (Value){.f = (urb_pop(stack).f - urb_pop(stack).f)});
}

void urb_fmul(List *stack)
{
    urb_push(stack, (Value){.f = (urb_pop(stack).f * urb_pop(stack).f)});
}

void urb_fdiv(List *stack)
{
    urb_push(stack, (Value){.f = (urb_pop(stack).f / urb_pop(stack).f)});
}

void urb_fmod(List *stack)
{
    urb_push(stack, (Value){.f = fmod(urb_pop(stack).f, urb_pop(stack).f)});
}

// float also includes the rounding stuff
void urb_round(List *stack)
{
    urb_push(stack, (Value){.f = round(urb_pop(stack).f)});
}