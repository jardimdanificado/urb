#!/bin/bash
if [[ ! -v makeself ]]; then
    if [ -e "./makeself" ]; then
        makeself=./makeself/makeself.sh 
    else
        echo "makeself was NOT FOUND! trying to obtain it..."
        git clone https://github.com/megastep/makeself.git
        makeself=./makeself/makeself.sh 
    fi
fi

mkdir -p build
rm -rf build/urb_tar
rm -rf build/rap
mkdir -p build/urb_tar/
mkdir -p build/urb_tar/build/
mkdir -p build/urb_tar/rap/
mkdir -p build/urb_tar/rap/src/
mkdir -p build/urb_tar/rap/scripts/

./rap/scripts/gen_interpreter.sh

cp rapper build/urb_tar/
cp beatmaker build/urb_tar/
cp urb.h build/urb_tar/
cp build/urb_dictionary.h build/urb_tar/build/

cp rap/scripts/preprocess.sh build/urb_tar/rap/scripts/
cp rap/scripts/assemble.sh build/urb_tar/rap/scripts/
cp rap/scripts/frontend.sh build/urb_tar/rap/scripts/
cp rap/src/reverse.h build/urb_tar/rap/src/
cp rap/src/parser.pl build/urb_tar/rap/src/
cp rap/src/syntax.m4 build/urb_tar/rap/src/

$makeself ./build/urb_tar build/rap rap_compiler_and_interpreter ./rap/scripts/frontend.sh