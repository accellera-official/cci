
CC     = g++
OPT    = -g -Wall 
OTHER  = -pedantic -Wno-long-long
CFLAGS = $(OPT) $(OTHER)

MODULE = testbench

SRCS = testbench.cpp \
       ParameterOwnerModule.cpp \
       Observer.cpp

OBJS = testbench.o \
       ParameterOwnerModule.o \
       Observer.o 
 
OBJS_W_PATHS = $(SRCS:.cpp=.o)

EXTRA_LIBS = -lccibrokerimpl -lcciparamimpl -lcciapi

include ../../Makefile.defs

all: $(EXE)

