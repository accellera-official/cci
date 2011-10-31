# define -DENABLE_CCI_VALUE_SUPPORT_FOR_TEST_DATATYPE for cci_value support
# for the data type test_datatype_without_parameter_implementation

CC     = g++
OPT    = -g -Wall 
OTHER  = -pedantic -Wno-long-long  # -DENABLE_CCI_VALUE_SUPPORT_FOR_TEST_DATATYPE
CFLAGS = $(OPT) $(OTHER)

MODULE = testbench

SRCS = testbench.cpp \
       ParameterOwnerModule.cpp \
       gs_cci_symbols_c.cpp

OBJS = testbench.o \
       ParameterOwnerModule.o \
       gs_cci_symbols_c.o
 
OBJS_W_PATHS = $(SRCS:.cpp=.o)

EXTRA_LIBS = -lccibrokerimpl -lcciparamimpl -lcciapi

include ../../Makefile.defs

all: $(EXE)

