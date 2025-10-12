#!/bin/bash
./urbuilder lib/example.c lib/std/std.c lib/std/io.c

gcc -o urb build/urb.c -g

./urb example/example.urb

valgrind \
          --leak-check=full \
          --show-leak-kinds=all \
          --track-origins=yes \
          --log-file=./valgrind-out.txt \
          --verbose ./urb example/example.urbin