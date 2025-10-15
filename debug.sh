#!/bin/bash
./urbuilder lib/example.c

gcc -o urbc compiler.c -g -I. -Ibuild
gcc -o urb interpreter.c -g -I. -Ibuild

cpp example/example.urb -E -P > build/example.urb
./urbc build/example.urb

valgrind \
          --leak-check=full \
          --show-leak-kinds=all \
          --track-origins=yes \
          --log-file=./valgrind-out.txt \
          --verbose ./urb build/example.urbin