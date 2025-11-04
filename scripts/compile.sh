#!/usr/bin/env bash

# cria arquivos temporários seguros
tmp_urb=$(mktemp /tmp/temp.urb.XXXXXX)
tmp_urb4=$(mktemp /tmp/temp.urb.XXXXXX)
tmp_urbin=$(mktemp /tmp/temp.urbin.XXXXXX)

# procesa com m4
m4 "$1" -I./build > "$tmp_urb4"

# processa com cpp
cpp -P "$tmp_urb4" -I./build > "$tmp_urb"

# gera binário
./urbc "$tmp_urb" > "$tmp_urbin"

cat "$tmp_urbin"

# opcional: remove os temporários ao sair
trap 'rm -f "$tmp_urb" "$tmp_urb4" "$tmp_urbin"' EXIT