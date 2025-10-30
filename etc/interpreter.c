#include "urb.h"
#include "../build/urb.c"

int main(int argc, char* argv[])
{
    // code_size[0] = capacity
    // code_size[1] = size
    Int code_size;

    List* code;
    List* context = urb_new(URB_DEFAULT_SIZE);
    FILE *file = fopen(argv[1], "rb");

    INIT_URB(context);
    if (fread(&code_size, sizeof(Int), 1, file) != 1)
    {
        fprintf(stderr, "error: this file is empty\n");
        return 1;
    };

    Int cap = 1;
    while(cap < code_size) cap *= 2;
    
    code = urb_new(cap);
    code->size = code_size;
    
    // we read *size* elements
    if (fread(code->data, sizeof(Int), code->size, file) != code->size)
    {
        // if we have a code_size claiming it has more elements -
        // - than it does, then the file is corrupted
        // or was not generated properly
        // or is not a urb binary
        fprintf(stderr, "error: this file is probably corrupted\n");
        return 1;
    }
    
    fclose(file);

    urb_interpret(context, code, NULL);

    urb_free(context);
    urb_free(code);
};