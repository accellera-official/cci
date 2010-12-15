# enabling SystemC2.1 automatically changes 
# includes and compiler flags in Makefile.defs
#SYSTEMC21V1 = yes

CC     = g++
OPT    = -g -Wall 
OTHER  = -pedantic -Wno-long-long
CFLAGS = $(OPT) $(OTHER)

#TODO: different links to different parameter implementations

MODULE = testbench

SRCS = testbench.cpp \
       OwnerModuleA.cpp \
       OwnerModuleB.cpp \

OBJS = testbench.o \
       OwnerModuleA.o \
       OwnerModuleB.o \
 
OBJS_W_PATHS = $(SRCS:.cpp=.o)

EXTRA_LIBS = -lccibrokerimpl -lcciparamimpl -lcciparamimpl_test_type -lcciapi
EXTRA_LIBS_DIRS = -L./param_impl

include ../../Makefile.defs

all: $(EXE)

