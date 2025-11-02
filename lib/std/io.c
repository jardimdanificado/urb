#include "urb.h"

void print(List* stack)
{
    Int value = urb_pop(stack).i;
    printf("%ld", value);
}

void println(List* stack)
{
    Int value = urb_pop(stack).i;
    printf("%ld\n", value);
}

void pring(List* stack)
{
    unsigned char* str = urb_pop(stack).p;
    printf("%s", str);
}

void pringln(List* stack)
{
    unsigned char* str = urb_pop(stack).p;
    printf("%s\n", str);
}

void ls(List* stack)
{
    // [index, type, "name"] = value;
    List* list = urb_pop(stack).p;
    for (Int i = 0; i < list->size; i++)
    {
        printf("[%ld] = %ld\n", i, list->data[i].i);
    }
}

// file i/o
List* load_urbin(const char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "error: cannot open file '%s'\n", filename);
        return NULL;
    }

    Int code_size;
    if (fread(&code_size, sizeof(Int), 1, file) != 1) {
        fprintf(stderr, "error: file '%s' is empty or invalid\n", filename);
        fclose(file);
        return NULL;
    }

    // calcula capacidade como a menor potência de 2 >= code_size
    Int cap = 1;
    while (cap < code_size) cap *= 2;

    List* code = urb_new(cap);
    code->size = code_size;

    if (fread(code->data, sizeof(Int), code->size, file) != code->size) {
        fprintf(stderr, "error: file '%s' is corrupted or not a valid URB binary\n", filename);
        urb_free(code);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return code;
}