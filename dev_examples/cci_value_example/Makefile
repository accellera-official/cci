# define -DENABLE_CCI_VALUE_SUPPORT_FOR_TEST_DATATYPE for cci_value support
# for the data type test_datatype_without_parameter_implementation

CC     = g++
OPT    = -g -Wall -Werror
OTHER  = -pedantic -Wno-long-long
       # -DENABLE_CCI_VALUE_SUPPORT_FOR_TEST_DATATYPE
CFLAGS = $(OPT) $(OTHER)

MODULE = testbench

SRCS = testbench.cpp \
       ParameterOwnerModule.cpp 

OBJS = testbench.o \
       ParameterOwnerModule.o 
 
OBJS_W_PATHS = $(SRCS:.cpp=.o)

EXTRA_LIBS = -lcciapi -lccibrokerimpl

include ../../Makefile.defs

INCDIR+=-I$(CCI_PARAM_IMPLEMENTATION_INC_DIR)
INCDIR+=-I$(GS_CONTROL_DIR)

all: $(EXE)

