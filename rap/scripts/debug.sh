#!/bin/bash
if (( $# < 1 )); then
    echo "filename missing..."
    exit
fi

mkdir -p build
./rap/scripts/embed.sh "$1" "$2" > ./build/main
chmod +x ./build/main

# check valgrind-out.txt for the debug output
valgrind \
          --leak-check=full \
          --show-leak-kinds=all \
          --track-origins=yes \
          --log-file=./valgrind-out.txt \
          --verbose ./build/main