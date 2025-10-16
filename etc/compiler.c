#include "urb.h"
// urb.c is a generated file
// usually found at build/urb.c
#include "urb.c"

int main()
{
    List* compiled = urb_new(URB_DEFAULT_SIZE);

    char word[128];

    // we dont use urb_preprocess directly
    // bcause we want to take advantage of -
    // - scanf performance and some bash features
    // loading a file would require we to define a static buffer
    // or using a slow "realloc-hell" file loader, like it used to be
    while (scanf("%127s", word) == 1) 
    {
        urb_push(compiled, urb_token_preprocess(word));
    }
    
    char* binary = malloc(sizeof(Int) * (compiled->size + 1));
    
    // we copy the metadata(.size and .capacity)
    memcpy(binary, compiled, sizeof(Int));
    
    // we copy the content(.data)
    memcpy(binary + sizeof(Int), compiled->data, compiled->size * sizeof(Int));

    fwrite(binary, 8, compiled->size + 1, stdout);

    return 0;
};