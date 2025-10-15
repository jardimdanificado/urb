#include "urb.h"
#include "../build/urb.c"
int main()
{
    List *context = urb_new(URB_DEFAULT_SIZE);
    INIT_URB(context);
    
    char* codestr = strdup("0123456789 0 context get call");
    List* code = urb_preprocess(codestr);

    for (size_t i = 0; i < code->size; i++)
    {
        printf("index[%ld] = %ld\n", i, code->data[i].i);
    }
    
    
    urb_interpret(context, code, NULL);

    urb_free(context);
    urb_free(code);
    free(codestr);
}