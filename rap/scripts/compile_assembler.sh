# run scripts to generate urb.c
# pass your lib names here
./rap/scripts/gen_urb_c.sh $@

# compile the bytecode assembler(beatmaker)
gcc -o beatmaker rap/src/assembler.c -g -I. -lm -O3
