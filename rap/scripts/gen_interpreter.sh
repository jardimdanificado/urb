#!/bin/bash
mkdir -p build

./rap/scripts/embed.sh "rap/src/interpreter.urb" "lib/*/*" > rapper

chmod +x rapper