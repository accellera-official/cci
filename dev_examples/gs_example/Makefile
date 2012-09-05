
CC     = g++
OPT    = -g -Wall 
OTHER  = -pedantic -Wno-long-long
CFLAGS = $(OPT) $(OTHER)

MODULE = testbench

SRCS = testbench.cpp \
       ParameterOwnerModule.cpp \
       ParameterOwnerMutabilityModule.cpp \
       ParamManipulateModule.cpp \
       ObserverModule.cpp \
       ValueModule.cpp

OBJS = testbench.o \
       ParameterOwnerModule.o \
       ParameterOwnerMutabilityModule.o \
       ParamManipulateModule.o \
       ObserverModule.o \
       ValueModule.o
 
OBJS_W_PATHS = $(SRCS:.cpp=.o)

EXTRA_LIBS = -lcciparamimpl -lcciapi -lccibrokerimpl 

include ../../Makefile.defs

all: $(EXE)

