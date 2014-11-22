
CC     = gcc
CXX    = g++
OPT    = -g -Wall -Werror -Wno-unused-local-typedefs
OTHER  = -pedantic -Wno-long-long
CXXFLAGS = $(OPT) $(OTHER)

SRCS = cci_cnf/cci_broker_manager.cpp \
       cci_cnf/cci_broker_stack.cpp \
       cci_cnf/cci_value.cpp

OBJS = $(SRCS:.cpp=.o)

OBJS_W_PATHS = $(SRCS:.cpp=.o)

DEPS := $(OBJS:.o=.d)

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
GS_CCI_HOME ?= $(shell cd $(CURDIR)/.. && pwd)

## set some directories 
CCI_DIR ?= $(GS_CCI_HOME)/api

DEFFLAGS = -DSC_INCLUDE_DYNAMIC_PROCESSES

INCDIR = -I. -I$(CCI_DIR) -I$(SYSTEMC)/include -I$(BOOST) 

## -----------------------------------------------------------------------------

LIB=libcciapi.a

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) rcs $@ $^
	ranlib $@

-include $(DEPS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(DEFFLAGS) $(INCDIR) -c $< -MMD -o $@

.cc.o:
	$(CXX) $(CXXFLAGS) $(DEFFLAGS) $(INCDIR) -c $< -MMD -o $@

clean:
	rm -f  $(OBJS) $(DEPS) $(LIB)
