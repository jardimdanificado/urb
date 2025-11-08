#!/usr/bin/env bash

# cria arquivos temporários seguros
tmp_urb4=$(mktemp /tmp/temp.urb.XXXXXX)
tmp_urb_pl=$(mktemp /tmp/temp.urb.XXXXXX)

echo "include(urb_dictionary.h)" > "$tmp_urb_pl"

./rap/src/parser.pl "$1" >> "$tmp_urb_pl"

# procesa com m4
m4 "$tmp_urb_pl" -I./build > "$tmp_urb4"

# processa com cpp
cpp -P "$tmp_urb4" -I./build

# opcional: remove os temporários ao sair
trap 'rm -f "$tmp_urb4" "$tmp_pl"' EXIT