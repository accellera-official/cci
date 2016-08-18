
CC       = gcc
CXX      = g++
OPT      = -g -Wall
OTHER    = -pedantic -Wno-long-long
CXXFLAGS = $(OPT) $(OTHER)


OBJS = $(SRCS:.cpp=.o)
 

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
GS_CONTROL_DIR = $(GS_CCI_HOME)/packages/greencontrol
CCI_DIR = $(GS_CCI_HOME)/api
GS_CCI_PARAM_DIR = $(GS_CCI_HOME)/packages/gs_param
GS_CCI_API_DIR = $(GS_CCI_HOME)/packages/gs_broker

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

