# rap
rap is the urb language, it is **COMPLETELY EXPERIMENTAL AND UNSTABLE**.

while rap itself is meant to be very readable and easy, its source can a nightmare for some people because it is heavily based on m4 preprocessor.

beside m4, rap also uses perl for some syntax suggar before m4, there are also some bash files and a few lines of c preprocessor;

the preprocessing order is perl + scripts > m4 > cpp, i personally recommend using m4, because every macro except reverse stuff is written in m4;

rap has no versioning, i cant even decide which is its main file, probably syntax.m4? idk

## requirements

in case you using the urb language you will need:
- a **gcc** *compatible* C compiler
- GNU **m4**, i guess other m4 implementation might work as well, but i havent tested
- **perl** 5.14 or newer
- **make**, this one is optional if you call the scripts directly
- **makeself**, we use this one to create the standalone
- **bash**, **sed**, **xxd** etc...

any decent linux system will already have all that bundled in, beside makeself, but it repo is cloned during the compilation if nor found

## compiling

to compile the default interpreter:

``make libs=LIBS``

note libs argument is completely optional, it defaults to "libs/\*/\*"

if you wanna compile as embedded debug:

``make debug filename=FILENAME libs=LIBS``

you will find the exec at build/main