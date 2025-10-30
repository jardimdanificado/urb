#!/bin/bash

# urb experimental debug system
# its planned to make a build system based on this

# user is meant to edit this manually for now
LIBS="lib/example.c lib/std/*"
SOURCE_FILE=examples/loop.urb

echo $LIBS

# run scripts to generate urb.c
./scripts/gen_urb_c.sh $LIBS

# compile the bytecode compiler(urbc)
# and the bytecode interpreter(urb)
gcc -o urbc etc/compiler.c -g -I. -lm -O3
gcc -o urb etc/interpreter.c -g -I. -lm -O3

cpp -P $SOURCE_FILE -I./build > build/temp.urb

./urbc build/temp.urb > build/temp.urbin

# check valgrind-out.txt for the debug output
valgrind \
          --leak-check=full \
          --show-leak-kinds=all \
          --track-origins=yes \
          --log-file=./valgrind-out.txt \
          --verbose ./urb build/temp.urbin