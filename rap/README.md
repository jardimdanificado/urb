# rap
rap is the urb language, it is **COMPLETELY EXPERIMENTAL AND UNSTABLE**.

proper standards and documentation are currently WIP

## requirements

in case you using the urb language you will need:
- a **gcc** *compatible* C compiler
- **git**
- GNU **m4**, i guess other m4 implementation might work as well, but i havent tested
- **perl** 5.14 or newer
- **make**, this one is optional if you call the scripts directly
- **makeself**, we use this to create the standalone rap
- **bash**, **sed**, **xxd** etc...

most linux will already have that, except for makeself, but we do clone makeself repo during the compilation.

## compiling

from the repo root run ``make`` and you will find the rap toolkit in ``./build/rap``, then run ``rap help`` and you will probably find everything you need there, futher docs are bein written right now...

the toolkit is meant for portability and ease of use, it is only meant to support linux(might work well on other unixes too, but i havnt tested 'em, so i cant assure).

rap toolkit does not work on windows directly, but you can cross-compile with something like ``rap embed file.rap -o file.exe -cc $mingw`` from WSL or such...