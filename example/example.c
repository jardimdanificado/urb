#include "returb.h"

void test_func(List* stack)
{
    Value v = returb_pop(stack);
    printf("value = %ld\n", v.i);
}