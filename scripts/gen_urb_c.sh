#!/usr/bin/env bash
set -euo pipefail

if [[ $# -lt 1 ]]; then
    echo "usage: $(basename "$0") <file1.c> [file2.c ...]" >&2
    exit 1
fi

mkdir -p build
interpreter_c="build/urb.c"

# captura arquivo temporário e lista de funções
mapfile -t output < <(scripts/extract_funcs.sh "$@")
tmp_c="${output[0]}"
funcs=("${output[@]:1}")

{
    echo "typedef struct List List;"
    echo "typedef union Value Value;"
    echo "typedef void (*Function)(List *stack);"
    echo
    for f in "${funcs[@]}"; do
        real_name="${f##*:}"
        echo "void $real_name(List* stack);"
    done
    echo
    echo "#define INIT_URB(context) \\"
    for f in "${funcs[@]}"; do
        real_name="${f##*:}"
        echo "    urb_push(context, (Value){.p = $real_name}); \\"
    done
    echo "    ((void)0)"
    echo
    cat "$tmp_c"
    echo
} > "$interpreter_c"

rm -f "$tmp_c"