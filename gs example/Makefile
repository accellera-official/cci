# enabling SystemC2.1 automatically changes 
# includes and compiler flags in Makefile.defs
#SYSTEMC21V1 = yes

CC     = g++
OPT    = -g -Wall 
OTHER  = -pedantic -Wno-long-long
CFLAGS = $(OPT) $(OTHER)

MODULE = testbench

SRCS = testbench.cpp \
       ParameterOwnerModule.cpp \
       ../gs_implementation/gs_cci.cpp \
       ../gs_implementation/gs_cci_cnf_api.cpp 

OBJS = testbench.o \
       ParameterOwnerModule.o \
       gs_cci.o \
       gs_cci_cnf_api.o
 
OBJS = $(SRCS:.cpp=.o)

#include ../Makefile.gspm
include ../Makefile.defs

all: $(EXE)

