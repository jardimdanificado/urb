#!/bin/bash
if (( $# < 1 )); then
    echo "filename missing..."
    exit
fi

mkdir -p build

./scripts/embed.sh "examples/interpreter.urb" "lib/*/*" > urb
./scripts/compile.sh "$1" > build/test.urbin

# check valgrind-out.txt for the debug output
valgrind \
          --leak-check=full \
          --show-leak-kinds=all \
          --track-origins=yes \
          --log-file=./valgrind-out.txt \
          --verbose ./urb build/test.urbin