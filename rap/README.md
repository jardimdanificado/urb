# rap
rap is the urb language, it is **COMPLETELY EXPERIMENTAL AND UNSTABLE**.

while rap itself is meant to be very readable and easy, its source can a nightmare for some people because it is heavily based on m4 preprocessor.

beside m4, rap also uses perl for some syntax suggar before m4, there are also some bash files and a few lines of c preprocessor;

the preprocessing order is perl + scripts > m4 > cpp, i personally recommend using m4, because every macro except reverse stuff is written in m4;

rap has no versioning, i cant even decide which is its main file, probably syntax.m4? idk

## requirements

in case you using the urb language you will need:
- a *gcc compatible* C compiler
- GNU m4, i guess other m4 implementation might work as well
- perl 5.14 or newer
- bash, sed, xxd etc...

any decent linux system will already have that bundled in;

## compiling

from the **urb** folder, call:

``./rap/scripts/embed.sh "$filename" "lib/*/*" > execfile``

it will get a executable called execfile.