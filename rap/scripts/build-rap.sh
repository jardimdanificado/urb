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

LIBS="${2:-libs/*/*}"

mkdir -p build
rm -rf build/urb_tar
rm -rf build/rap
mkdir -p build/urb_tar/
mkdir -p build/urb_tar/build/
mkdir -p build/urb_tar/rap/
mkdir -p build/urb_tar/rap/src/
mkdir -p build/urb_tar/rap/scripts/

./rap/scripts/gen_interpreter.sh "$LIBS"

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

$makeself --nocomp --nomd5 --nocrc ./build/urb_tar build/rap rap_compiler_and_interpreter ./rap/scripts/frontend.sh

# we need to force it to be quiet, otherwise we would need to pass --quiet every call
# we also turn on the "nodiskspace"
sed -i 's/quiet="n"/quiet=y/' build/rap
sed -i 's/noprogress=n/noprogress=y/' build/rap
sed -i 's/nodiskspace="n"/nodiskspace=y/' build/rap