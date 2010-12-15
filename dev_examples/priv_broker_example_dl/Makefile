# enabling SystemC2.1 automatically changes 
# includes and compiler flags in Makefile.defs
#SYSTEMC21V1 = yes

CC     = g++
OPT    = -g -Wall 
OTHER  = -pedantic -Wno-long-long# -std=c++0x
CFLAGS = $(OPT) $(OTHER)

MODULE = testbench

SRCS = testbench.cpp \
       ModuleA.cpp \
       ModuleB.cpp \
       ModuleC.cpp \
       ObserverModule.cpp

OBJS = testbench.o \
       ModuleA.o \
       ModuleB.o \
       ModuleC.o \
       ObserverModule.o
 
OBJS_W_PATHS = $(SRCS:.cpp=.o)

EXTRA_LIBS = -lccibrokerimpl -lcciparamimpl -lcciapi

include ../../Makefile.defs

all: $(EXE)

