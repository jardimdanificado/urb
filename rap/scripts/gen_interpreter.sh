#!/bin/bash
if (( $# < 1 )); then
    echo "filename missing..."
    exit
fi

mkdir -p build

./rap/scripts/embed.sh "rap/src/interpreter.urb" "lib/*/*" > rapper
./rap/scripts/compile.sh "$1" > build/test.urbin

chmod +x rapper