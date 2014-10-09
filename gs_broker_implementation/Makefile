# enable compiler flag -DCCI_CNF_IMPL_VERBOSE to get broker
#  implementation debug output (see CFLAGS)

CC       = gcc
CXX      = g++
OPT      = -g -Wall
OTHER    = -pedantic -Wno-long-long -Wno-unused-local-typedefs
CXXFLAGS = $(OPT) $(OTHER)

SRCS = gs_cci_broker.cpp \
       gs_cci_cnf_broker.cpp \
       gs_cci_cnf_broker_accessor.cpp \
       gs_cci_cnf_broker_accessor_handler.cpp \
       gs_cci_cnf_private_broker.cpp \
       gs_cci_cnf_private_broker_accessor.cpp \
       helpers.cpp

OBJS = gs_cci_broker.o \
       gs_cci_cnf_broker.o \
       gs_cci_cnf_broker_accessor.o \
       gs_cci_cnf_broker_accessor_handler.o \
       gs_cci_cnf_private_broker.o \
       gs_cci_cnf_private_broker_accessor.o \
       helpers.o
 
OBJS_W_PATHS = $(SRCS:.cpp=.o)


#include ../Makefile.defs
## -----------------------------------------------------------------------------

ifdef BOOST_HOME
BOOST = $(BOOST_HOME)
endif
BOOST ?= /cad/tools/boost

ifdef SYSTEMC_HOME
SYSTEMC = $(SYSTEMC_HOME)
endif
SYSTEMC ?= /cad/tools/systemc

ifdef CCI_HOME
GS_CCI_HOME = $(CCI_HOME)
endif
GS_CCI_HOME ?=/ Users/schroede/mydev/cci_git/cci

## set some directories 
GS_CONTROL_DIR = $(GS_CCI_HOME)/greencontrol_cci_branch
GREENSTARCORE_DIR = $(GS_CCI_HOME)
CCI_DIR = $(GS_CCI_HOME)/api
GS_CCI_BROKER_DIR = $(GS_CCI_HOME)/gs_broker_implementation

DEFFLAGS = -DSC_INCLUDE_DYNAMIC_PROCESSES

## muldefs should be avoided in the source code, not by the linker
LDFLAGS =

INCDIR = -I. -I$(CCI_DIR) -I$(GS_CCI_BROKER_DIR)\
	 -I$(GS_CONTROL_DIR) -I$(GREENSTARCORE_DIR) -I$(SYSTEMC)/include\
 	 -I$(BOOST) 

## -----------------------------------------------------------------------------

LIB=libccibrokerimpl.a

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) rcs $@ $^
	ranlib $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $(DEFFLAGS) $(INCDIR) -c $< -o $@

.cc.o:
	$(CXX) $(CXXFLAGS) $(DEFFLAGS) $(INCDIR) -c $< -o $@

clean:
	rm -f  $(OBJS) $(LIB)
