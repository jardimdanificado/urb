#!/usr/bin/env bash
set -euo pipefail

if [[ $# -lt 1 ]]; then
    echo "usage: $(basename "$0") <file1.c> [file2.c ...]" >&2
    exit 1
fi

mkdir -p build
urb_names_h="build/dictionary.h"

# captura lista de funções (não precisa do arquivo temporário)
mapfile -t output < <(scripts/extract_funcs.sh "$@")
funcs=("${output[@]:1}")

{
    echo "#ifndef URB_DICTIONARY_H"
    echo "#define URB_DICTIONARY_H"
    echo
    source scripts/opcodes.sh || true
    index=0
    for f in "${funcs[@]}"; do
        reg_name="${f%%:*}"
        echo "#define $reg_name $index"
        index=$((index + 1))
    done
    echo
    echo "#endif // URB_DICTIONARY_H"
} > "$urb_names_h"
