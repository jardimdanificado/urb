#!/usr/bin/env bash

mkdir -p ./.tmp/

# cria arquivos temporários seguros
tmp_urb4=$(mktemp ./.tmp/temp.urb.XXXXXX)
tmp_urb_pl=$(mktemp ./.tmp/temp.urb.XXXXXX)

echo "#include \"urb_dictionary.h\"" > "$tmp_urb_pl"

./rap/src/parser.pl "$1" >> "$tmp_urb_pl"

# processa com cpp
cpp -P "$tmp_urb_pl" -I./build

# opcional: remove os temporários ao sair
trap 'rm -f "$tmp_urb4" "$tmp_pl"' EXIT