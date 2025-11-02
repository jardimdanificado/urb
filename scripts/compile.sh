#!/usr/bin/env bash

# cria arquivos temporários seguros
tmp_urb=$(mktemp /tmp/temp.urb.XXXXXX)
tmp_urbin=$(mktemp /tmp/temp.urbin.XXXXXX)

# processa com cpp
cpp -P "$1" -I./build > "$tmp_urb"

# gera binário
./urbc "$tmp_urb" > "$tmp_urbin"

cat "$tmp_urbin"

# opcional: remove os temporários ao sair
trap 'rm -f "$tmp_urb" "$tmp_urbin"' EXIT