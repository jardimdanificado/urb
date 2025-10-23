#!/bin/bash
tempfile=$(mktemp)
cpp $1 -P > "$tempfile"
./urbc "$tempfile"
rm "$tempfile"