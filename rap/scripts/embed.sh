#!/bin/bash
SOURCE_FILE="$1"
LIBS="$2"

mkdir -p ./.tmp/

tmp_urb=$(mktemp ./.tmp/temp.urb.XXXXXX)
tmp_urb_exec=$(mktemp ./.tmp/temp.urb.exe.XXXXXX)
tmp_urbin=$(mktemp ./.tmp/temp.urbin.XXXXXX)
tmp_pre=$(mktemp ./.tmp/temp.urbin.XXXXXX)

./rap/scripts/compile_assembler.sh "$LIBS"
./rap/scripts/preprocess.sh "$SOURCE_FILE" > "$tmp_pre"
./rap/scripts/assemble.sh "$tmp_pre" > "$tmp_urbin"
./rap/scripts/gen_embedded_c.sh "$tmp_urbin" > build/embedded.c

gcc -o $tmp_urb build/embedded.c -g -I. -lm -O3

cat $tmp_urb
trap 'rm -f "$tmp_pre" "$tmp_urb" "$tmp_urb_exec" "$tmp_urbin"' EXIT
