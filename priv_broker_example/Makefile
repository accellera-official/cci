# enabling SystemC2.1 automatically changes 
# includes and compiler flags in Makefile.defs
#SYSTEMC21V1 = yes

CC     = g++
OPT    = -g -Wall 
OTHER  = -pedantic -Wno-long-long
CFLAGS = $(OPT) $(OTHER)

MODULE = testbench

SRCS = testbench.cpp \
       ModuleA.cpp \
       ModuleB.cpp \
       ObserverModule.cpp \
       ../gs_api_implementation/cci_api.cpp \
       ../gs_api_implementation/gs_cci_cnf_broker.cpp 

OBJS = testbench.o \
       ModuleA.o \
       ModuleA.o \
       ObserverModule.o \
       cci_api.o \
       gs_cci_cnf_broker.o
 
OBJS_W_PATHS = $(SRCS:.cpp=.o)

#include ../Makefile.gspm
include ../Makefile.defs

all: $(EXE)

