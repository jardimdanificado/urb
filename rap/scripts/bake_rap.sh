#!/bin/bash
mkdir -p build
rm -rf build/urb_tar
mkdir -p build/urb_tar/
mkdir -p build/urb_tar/build/
mkdir -p build/urb_tar/rap/
mkdir -p build/urb_tar/rap/src/
mkdir -p build/urb_tar/rap/scripts/

./rap/scripts/gen_interpreter.sh

cp rapper build/urb_tar/
cp beatmaker build/urb_tar/
cp urb.h build/urb_tar/
cp -r libs build/urb_tar/
cp build/urb_dictionary.h build/urb_tar/build/

cp rap/scripts/preprocess.sh build/urb_tar/rap/scripts/
cp rap/scripts/assemble.sh build/urb_tar/rap/scripts/
cp rap/src/reverse.h build/urb_tar/rap/src/
cp rap/src/parser.pl build/urb_tar/rap/src/
cp rap/src/syntax.m4 build/urb_tar/rap/src/

makeself ./build/urb_tar rap __URB_AND_RAP_GENERIC_DELIMITER_FOR_MAKESELF__ ./rap/scripts/rap_frontend.sh