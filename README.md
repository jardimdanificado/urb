# urb
urb is a minimal footprint bytecode virtual machine, it is list-based and does only require a regular C99 compiler to get started;

# rap
rap is a frontend language that generates urb bytecode, check it at: https://github.com/jardimdanificado/rap 

# compiling
urb cannot be standalone compiled because it is just a 250 line header, no interpreter frontend, no opcodes, just the minimal the vm needs to get going.

# libs
in libs/std you can find some libs written for the VM, function pointers must be added in the exec list in order to be recognized as opcodes;

# the interpreter
the interpreter consists in a function that receives 3 arguments: exec, mem and stack.

## exec
exec is where we store our "opcodes", our native functions written in C, exec occupies the first values of a integer, from `INT_MIN + OP_CODES_OFFSET` to `INT_MIN + OP_CODES_OFFSET + mem.size`, if one of these values is read in the code it will automatically call the function;

## mem
mem is our code, and also our memory as the name suggests.

mem is maybe the most tricky part of urb, our memory is also our code... that means it can do the unsafe and "scary" *self-modifing code*, which is actually very cool... but i understand the concerns about this technique, the good news is that it is completely optional.

mem is where we usually store our variables and things we dont want the interpreter to interpret also, its not really required but is the fastest way to retrieve a value, it works like exec, but reversed, the values reserved for the mem are the ones from `INT_MAX - exec.size` to `INT_MAX`.
