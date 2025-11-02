#!/bin/bash
SOURCE_FILE="$1"
LIBS="$2"

tmp_urb=$(mktemp /tmp/temp.urb.exe.XXXXXX)
tmp_urbin=$(mktemp /tmp/temp.urbin.XXXXXX)

# run scripts to generate urb.c
./scripts/gen_urb_c.sh $LIBS

# compile the bytecode compiler(urbc)
gcc -o urbc etc/compiler.c -g -I. -lm -O3

./scripts/compile.sh "$SOURCE_FILE" > "$tmp_urbin"
./scripts/gen_embedded_c.sh "$tmp_urbin" > build/embedded.c

gcc -o $tmp_urb build/embedded.c -g -I. -lm -O3

cat $tmp_urb
trap 'rm -f "$tmp_urb" "$tmp_urbin"' EXIT