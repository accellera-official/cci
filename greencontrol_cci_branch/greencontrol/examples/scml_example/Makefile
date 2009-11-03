# enabling SystemC2.1 automatically changes 
# includes and compiler flags in Makefile.defs
#SYSTEMC21V1 = yes

CC     = g++
#OPT    = -O3 -funroll-loops -mtune=pentium4
OPT  = -g -Wall
#OTHER  = -Wno-deprecated
CFLAGS = $(OPT) $(OTHER) -DNO_DEPRECATED_WARNINGS
# CFLAGS = $(DEBUG) $(OTHER)
# EXTRA_LIBS = -lboost_program_options #-lModelingObjects

MODULE = testbench
SRCS = testbench.cpp scml_test_ip.cpp scml_orig_ip.cpp
OBJS = $(SRCS:.cpp=.o)

include ../Makefile.gspm
include ../Makefile.defs

all: $(EXE)

