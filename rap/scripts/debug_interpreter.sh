#!/bin/bash
if (( $# < 1 )); then
    echo "filename missing..."
    exit
fi

mkdir -p build

./rap/scripts/embed.sh "rap/src/interpreter.urb" "lib/*/*" > rapper
./rap/scripts/compile.sh "$1" > build/test.urbin

chmod +x rapper

# check valgrind-out.txt for the debug output
valgrind \
          --leak-check=full \
          --show-leak-kinds=all \
          --track-origins=yes \
          --log-file=./valgrind-out.txt \
          --verbose ./rapper build/test.urbin
