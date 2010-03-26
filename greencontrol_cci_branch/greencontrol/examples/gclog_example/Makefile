TARGET_ARCH = macosx

CC     = g++
#OPT	= -O3 -funroll-loops
#OPT    = -O3 -funroll-loops -mtune=pentium4
OPT    = -g -Wall
#OPT	= -g -Wall -fprofile-arcs -ftest-coverage
#OPT	= -g -Wall -lpthread -D_REENTRANT
#OTHER  = -Wno-deprecated
CFLAGS = $(OPT) $(OTHER)

# CFLAGS = $(DEBUG) $(OTHER)

MODULE = gclog
SRCS = Testbench.cpp TestIP.cpp
OBJS = $(SRCS:.cpp=.o)
#OBJS = ...

include ../Makefile.defs

all: $(EXE)

