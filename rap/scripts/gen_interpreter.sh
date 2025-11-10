#!/bin/bash
mkdir -p build

LIBS="libs/*/*"

if [[ -n "$1" ]]; then
    LIBS="$1"
fi

./rap/scripts/embed.sh "rap/src/interpreter.rap" "$LIBS" > rapper
chmod +x rapper