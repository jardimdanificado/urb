#!/usr/bin/env bash
set -euo pipefail

# $@ -> arquivos .c
if [[ $# -lt 1 ]]; then
    echo "usage: $(basename "$0") <file1.c> [file2.c ...]" >&2
    exit 1
fi

tmp_c=$(mktemp)
declare -a funcs=()

regex='void[[:space:]]+[A-Za-z_][A-Za-z0-9_]*[[:space:]]*\([^)]*List[[:space:]]*\*?[[:space:]]*[A-Za-z_][A-Za-z0-9_]*[[:space:]]*\)'

for file in "$@"; do
    cat "$file" >> "$tmp_c"
    echo "" >> "$tmp_c"

    while IFS= read -r line; do
        name=$(echo "$line" | sed -E 's/.*void[[:space:]]+([A-Za-z_][A-Za-z0-9_]*).*/\1/')
        reg_name=${name#urb_}
        funcs+=("$reg_name:$name")
    done < <(grep -Poz "$regex" "$file" | tr '\0' '\n' || true)
done

# output
# primeira linha: arquivo temporário concatenado
# resto: funções
printf "%s\n" "$tmp_c"
printf "%s\n" "${funcs[@]}"
