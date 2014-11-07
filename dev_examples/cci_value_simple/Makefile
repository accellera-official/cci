CC     = g++
OPT    = -g -Wall -Werror
OTHER  = -pedantic -Wno-long-long
CFLAGS = $(OPT) $(OTHER)

MODULE = cci_value_simple

SRCS = cci_value_simple.cpp
OBJS = $(SRCS:.cpp=.o) 
OBJS_W_PATHS = $(OBJS)
 
EXTRA_LIBS = -lcciapi

CCI_HOME ?= $(shell cd $(CURDIR)/../.. ; pwd)
include ../../Makefile.defs

all: $(EXE)

