
CC     = g++
OPT    = -g -Wall 
OTHER  = -pedantic -Wno-long-long
CFLAGS = $(OPT) $(OTHER)

MODULE = testbench

SRCS = testbench.cpp
       
OBJS = testbench.o
 
OBJS_W_PATHS = $(SRCS:.cpp=.o)

EXTRA_LIBS = -lcciparamimpl -lcciapi -lccibrokerimpl

include ../../Makefile.defs

all: $(EXE)

