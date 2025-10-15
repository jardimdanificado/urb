#include "urb.h"
#include "urb.c"

int main(int argc, char* argv[])
{
    // header[0] = capacity
    // header[1] = size
    Half header[2];

    List* code;
    List* context = urb_new(URB_DEFAULT_SIZE);
    FILE *file = fopen(argv[1], "rb");

    INIT_URB(context);
    if (fread(header, sizeof(Half), 2, file) != 2)
    {
        fprintf(stderr, "error: this file is smaller than a urb header\n");
        return 1;
    };
    
    code = urb_new(header[0]);
    code->size = header[1];
    
    // we read *size* elements
    if (fread(code->data, sizeof(Int), code->size, file) != code->size)
    {
        // if we have a header claiming it has more elements -
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