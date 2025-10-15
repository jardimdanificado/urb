#include "urb.h"
#include "math.h"

void feq(List* stack)
{
    Int a = urb_pop(stack).f;
    Int b = urb_pop(stack).f;
    urb_push(stack, (Value){.f = a == b ? 1 : 0});
}

void fneq(List* stack)
{
    Int a = urb_pop(stack).f;
    Int b = urb_pop(stack).f;
    urb_push(stack, (Value){.f = a != b ? 1 : 0});
}

void fgt(List* stack)
{
    Int a = urb_pop(stack).f;
    Int b = urb_pop(stack).f;
    urb_push(stack, (Value){.f = a > b ? 1 : 0});
}

void fge(List* stack)
{
    Int a = urb_pop(stack).f;
    Int b = urb_pop(stack).f;
    urb_push(stack, (Value){.f = a >= b ? 1 : 0});
}

void flt(List* stack)
{
    Int a = urb_pop(stack).f;
    Int b = urb_pop(stack).f;
    urb_push(stack, (Value){.f = a < b ? 1 : 0});
}

void fle(List* stack)
{
    Int a = urb_pop(stack).f;
    Int b = urb_pop(stack).f;
    urb_push(stack, (Value){.f = a <= b ? 1 : 0});
}

void fadd(List *stack)
{
    urb_push(stack, (Value){.f = (urb_pop(stack).f + urb_pop(stack).f)});
}

void fsub(List *stack)
{
    urb_push(stack, (Value){.f = (urb_pop(stack).f - urb_pop(stack).f)});
}

void fmul(List *stack)
{
    urb_push(stack, (Value){.f = (urb_pop(stack).f * urb_pop(stack).f)});
}

void fdiv(List *stack)
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