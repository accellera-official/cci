#!/bin/bash

echo; echo
echo -n "WARNING: THE FOLLOWING COMPILE MAY TAKE SOME TIME (potentially hours)"
echo
echo -n "         AND WILL CREATE LARGE OBJECT AND LIBRARY FILES!"
echo; echo

GLOBAL_UPPER_BOUND=1024
STEPS=64

for ((lower_bound=1, upper_bound=STEPS; upper_bound <= GLOBAL_UPPER_BOUND ; lower_bound=lower_bound+STEPS, upper_bound=upper_bound+STEPS))
do
  echo; echo
  echo -n "make bigsymbols with bounds $lower_bound - $upper_bound "
  echo
  echo make bigsymbols "CCI_SYMBOLS_LOWER_BOUND=$lower_bound" "CCI_SYMBOLS_UPPER_BOUND=$upper_bound" OBJNAME=big$lower_bound
  make bigsymbols "CCI_SYMBOLS_LOWER_BOUND=$lower_bound" "CCI_SYMBOLS_UPPER_BOUND=$upper_bound" OBJNAME=big$lower_bound
done

echo; echo
echo -n "make the rest (make all)"
make all

echo; echo
echo -n "Create table of contents"
ranlib libcciparamimpl.a

echo; echo
strip -x -S libcciparamimpl.a -o libcciparamimpl.stripped.a

echo; echo
echo -n "Compressing brings significant reduction in size!"