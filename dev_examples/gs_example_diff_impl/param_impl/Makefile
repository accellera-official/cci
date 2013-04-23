
CC       = gcc
CXX      = g++
OPT      = -g -Wall
OTHER    = -pedantic -Wno-long-long
CXXFLAGS = $(OPT) $(OTHER)

SRCS = ../../gs_param_implementation/json_spirit/json_spirit_reader.cpp \
       json_spirit/json_spirit_writer.cpp \
       gs_cci_symbols_b.cpp

OBJS = ../../../gs_param_implementation/json_spirit/json_spirit_reader.o \
       ../../../gs_param_implementation/json_spirit/json_spirit_writer.o \
       gs_cci_symbols_b.o
 

# Makefile.defs
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
GS_CCI_HOME ?= /Users/schroede/mydev/cci_git/cci

## set some directories 
GS_CONTROL_DIR = $(GS_CCI_HOME)/greencontrol_cci_branch
CCI_DIR = $(GS_CCI_HOME)/api
GS_CCI_PARAM_DIR = $(GS_CCI_HOME)/gs_param_implementation
GS_CCI_API_DIR = $(GS_CCI_HOME)/gs_broker_implementation

DEFFLAGS = -DSC_INCLUDE_DYNAMIC_PROCESSES

INCDIR = -I. -I$(CCI_DIR) -I$(SYSTEMC)/include -I$(BOOST) \
         -I$(GS_CCI_API_DIR) -I$(GS_CONTROL_DIR)  -I$(GS_CCI_PARAM_DIR)

## -----------------------------------------------------------------------------

LIB=libcciparamimpl_test_type.a

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) rcs $@ $^
	ranlib $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $(SCCFLAG) $(DEFFLAGS) $(INCDIR) -c $< -o $@

.cc.o:
	$(CXX) $(CXXFLAGS) $(SCCFLAG) $(DEFFLAGS) $(INCDIR) -c $< -o $@

clean:
	rm -f  $(OBJS) $(LIB)

