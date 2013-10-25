# -----------------------------------------------------------------------------
#
# This is a 'make' wrapper for the user examples which are built with the 
# regression script, verify.pl.  It is convenient for IDE (e.g. Eclipse) 
# integration.  It resides in the examples top-level directory but should be 
# invoked from within an examples directory, as in:
#
#   cd $CCI_HOME/examples/ex01_Simple_Int_Param
#   gmake -f ../Makefile
#
# build results, including systemc.exe, will be placed directly in the example
# subdirectory.
#
# -----------------------------------------------------------------------------
all: systemc.exe

systemc.exe: *.h *.cpp
	cd ../..; scripts/verify.pl -v -g -I ${CCI_HOME}/examples/common/inc examples/$(notdir $(shell pwd))

