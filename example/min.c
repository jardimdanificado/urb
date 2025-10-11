#include "urb.h"

void test_func(List *stack)
{
    Value v = urb_pop(stack);
    printf("value = %ld\n", v.i);
}

int main()
{
    List *context = urb_new(URB_DEFAULT_SIZE);

    urb_push(context, (Value){.fn = test_func});
    
    char* codestr = strdup("0123456789 0 : !");
    List* code = urb_preprocess(codestr);
    
    urb_interpret(context, code, NULL);

    urb_free(context);
    urb_free(code);
    free(codestr);
}