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

if [[ -z LIBS ]]; then 
    LIBS="${2:-libs/*/*}"
fi


mkdir -p build
rm -rf build/urb_tar
rm -rf build/rap
mkdir -p build/urb_tar/
mkdir -p build/urb_tar/build/
mkdir -p build/urb_tar/rap/
mkdir -p build/urb_tar/rap/src/
mkdir -p build/urb_tar/rap/scripts/

./rap/scripts/gen_interpreter.sh "$LIBS"

cp -r libs build/urb_tar/

cp Makefile build/urb_tar/

cp rapper build/urb_tar/
cp beatmaker build/urb_tar/
cp urb.h build/urb_tar/
cp build/urb_dictionary.h build/urb_tar/build/
cp build/urb.c build/urb_tar/build/

cp -r rap/scripts build/urb_tar/rap/
cp -r rap/src build/urb_tar/rap/
cp -r makeself build/urb_tar/

$makeself ./build/urb_tar build/rap rap_compiler_and_interpreter ./rap/scripts/frontend.sh

# we need to force it to be quiet, otherwise we would need to pass --quiet every call
# we also turn on the "nodiskspace"
sed -i 's/quiet="n"/quiet=y/' build/rap
sed -i 's/noprogress=n/noprogress=y/' build/rap
sed -i 's/nodiskspace="n"/nodiskspace=y/' build/rap