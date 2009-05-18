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
       ../dummy_implementation/gs_cci.cpp \
       ../dummy_implementation/gs_cci_cnf_api.cpp 

OBJS = testbench.o \
       ParameterOwnerModule.o \
       ParamManipulateModule.o \
       ObserverModule.o \
       gs_cci.o \
       gs_cci_cnf_api.o
 
OBJS_W_PATHS = $(SRCS:.cpp=.o)

include ../Makefile.defs

all: $(EXE)

