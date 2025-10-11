#include "returb.h"

void test_func(List *stack)
{
    Value v = returb_pop(stack);
    printf("value = %ld\n", v.i);
}

int main()
{
    List *context = returb_new(RETURB_DEFAULT_SIZE);

    returb_push(context, (Value){.fn = test_func});
    
    char* codestr = strdup("0123456789 0 : !");
    List* code = returb_preprocess(codestr);
    
    returb_interpret(context, code, NULL);

    returb_free(context);
    returb_free(code);
    free(codestr);
}