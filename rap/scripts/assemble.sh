#!/usr/bin/env bash

# cria arquivos temporários seguros
tmp_urb=$(mktemp /tmp/temp.urb.XXXXXX)
tmp_urb4=$(mktemp /tmp/temp.urb.XXXXXX)
tmp_urb_pl=$(mktemp /tmp/temp.urb.XXXXXX)
tmp_urbin=$(mktemp /tmp/temp.urbin.XXXXXX)

./rap/scripts/preprocess.sh "$1" > "$tmp_urb"
# gera binário
./beatmaker "$tmp_urb" > "$tmp_urbin"

cat "$tmp_urbin"

# opcional: remove os temporários ao sair
trap 'rm -f "$tmp_urb" "$tmp_urb4" "$tmp_urbin" "$tmp_pl"' EXIT