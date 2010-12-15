This example has two different Makefiles:

1/
$ make
compiles against the complete param lib

2/
$ make -f Makefile.only.needed.symbols
compiles an on-demand param lib
(also see comment within the Makefile!)