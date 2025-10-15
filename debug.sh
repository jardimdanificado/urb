#!/bin/bash
./urbuilder lib/example.c

gcc -o urb cli.c -g -I. -Ibuild

./urb example/example.urb

valgrind \
          --leak-check=full \
          --show-leak-kinds=all \
          --track-origins=yes \
          --log-file=./valgrind-out.txt \
          --verbose ./urb example/example.urbin