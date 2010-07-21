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
       ParamManipulateModule.cpp \
       ObserverModule.cpp \
       ValueModule.cpp

OBJS = testbench.o \
       ParameterOwnerModule.o \
       ParamManipulateModule.o \
       ObserverModule.o \
       ValueModule.o
 
OBJS_W_PATHS = $(SRCS:.cpp=.o)

EXTRA_LIBS = -lcciapiimpl -lcciparamimpl -lcciapi

#include ../Makefile.gspm
include ../Makefile.defs

all: $(EXE)

