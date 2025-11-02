#include "urb.h"
#include "../build/urb.c"

//urb_content_replacement

int main(int argc, char* argv[])
{
    List* exec = urb_new(URB_DEFAULT_SIZE);
    INIT_URB(exec);
 
    Int code_size = (embedded_code_len/sizeof(Int))-1;
    Int code_capacity = 1;

    while(code_size > code_capacity) 
        code_capacity *= 2;
    
    List* code = urb_new(code_capacity);
    code->size = code_size;
    memcpy(code->data, embedded_code + sizeof(Int), embedded_code_len-sizeof(Int));
    urb_interpret(exec, code, NULL);

    urb_free(exec);
    urb_free(code);
};