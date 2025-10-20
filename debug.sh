#!/bin/bash

# urb experimental debug system
# its planned to make a build system based on this

# user is meant to edit this manually for now
LIBS="lib/example.c lib/std/*"
SOURCE_FILE=examples/callist_example.urb

echo $LIBS

# run scripts to generate urb.c and dictionary.h
# urb.c is required to compile both urbc and urb
# dictionary.h is used in textual urb
./scripts/gen_urb_c.sh $LIBS
./scripts/gen_dictionary.sh $LIBS

# compile the bytecode compiler(urbc)
# and the bytecode interpreter(urb)
gcc -o urbc etc/compiler.c -g -I. -Ibuild -lm
gcc -o urb etc/interpreter.c -g -I. -Ibuild -lm

# urbpp(urb preprocessor) essentially call to c preprocessor and
# the result is passed to urbc using pipe
# urbc deals with stdio only, receive its content from stdin, and output to stdout
# its important to note that, the target script must include the dictionary himself
./scripts/urbpp.sh $SOURCE_FILE > build/temp.urbin

# unlike urbc, the interpreter receive a filename
# check valgrind-out.txt for the debug output
valgrind \
          --leak-check=full \
          --show-leak-kinds=all \
          --track-origins=yes \
          --log-file=./valgrind-out.txt \
          --verbose ./urb build/temp.urbin