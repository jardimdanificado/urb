#include "returb.h"

#include "returb_merged_libs.h"

int main()
{
    // header[0] = capacity
    // header[1] = size
    Half header[2];

    List* code;
    List* context = returb_new(RETURB_DEFAULT_SIZE);

    INIT_RETURB(context);
    
    if (fread(header, sizeof(Half), 2, stdin) != 2)
    {
        fprintf(stderr, "error: this file is smaller than a returb header\n");
        return 1;
    };

    code = returb_new(header[0]);
    code->size = header[1];

    // we read *size* elements
    if (fread(code->data, sizeof(Int), code->size, stdin) != code->size)
    {
        // if we have a header claiming it has more elements -
        // - than it does, then the file is corrupted
        // or was not generated properly
        // or is not a returb binary
        fprintf(stderr, "error: this file is probably corrupted\n");
        return 1;
    }

    returb_interpret(context, code, NULL);

    returb_free(context);
    returb_free(code);
};