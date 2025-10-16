#!/bin/bash
./scripts/gen_urb_c.sh lib/example.c
./scripts/gen_dictionary.sh lib/example.c

gcc -o urbc etc/compiler.c -g -I. -Ibuild
gcc -o urb etc/interpreter.c -g -I. -Ibuild

{
    urbc=./urbc ./scripts/urbpp.sh 
} > build/example.urbin
valgrind \
          --leak-check=full \
          --show-leak-kinds=all \
          --track-origins=yes \
          --log-file=./valgrind-out.txt \
          --verbose ./urb build/example.urbin