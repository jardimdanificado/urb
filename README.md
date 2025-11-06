# urb
urb is a minimal footprint bytecode virtual machine, it is list-based and does only require a regular C99 compiler to get started;

# rap
rap is the urb language, check its documentation in its proper readme.

# compiling
urb cannot be standalone compiled because it is just a 250 line header, no interpreter frontend, no opcodes, just the minimal the vm needs to get going.

in lib/std you can find some libs written for the VM, function pointers must be added in the exec list in order to be recognized as opcodes;

# the interpreter
the interpreter consists in a function that receives 3 arguments: exec, mem and stack.

## exec
exec is where we store our "opcodes", our native funcions written in C, exec occupies the first values of a integer, from `INT_MIN` to `INT_MIN + exec.size`, if one of these values is read in the code it will automatically call the function;

## mem
mem is our code, and also our memory as the name suggests.