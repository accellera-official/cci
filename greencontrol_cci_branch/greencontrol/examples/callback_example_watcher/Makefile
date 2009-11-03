# enabling SystemC2.1 automatically changes 
# includes and compiler flags in Makefile.defs
#SYSTEMC21V1 = yes

CC     = g++
#OPT    = -O3 -funroll-loops -mtune=pentium4
OPT  = -g -Wall -lpthread -D_REENTRANT
#OTHER  = -Wno-deprecated
CFLAGS = $(OPT) $(OTHER)
# CFLAGS = $(DEBUG) $(OTHER)
# EXTRA_LIBS = -lboost_program_options

MODULE = testbench_callback
SRCS = testbench_callback.cpp ParameterOwnerModule.cpp ObserverModule.cpp ParamManipulateModule.cpp
OBJS = $(SRCS:.cpp=.o)

include ../Makefile.gspm
include ../Makefile.defs

all: $(EXE)

