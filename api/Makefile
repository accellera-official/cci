
CC     = gcc
CXX    = g++
OPT    = -g -Wall -Werror -Wno-unused-local-typedefs
OTHER  = -pedantic -Wno-long-long
CXXFLAGS = $(OPT) $(OTHER)

SRCS = cnf/cci_broker_manager.cpp \
       cnf/cci_broker_stack.cpp \
       cnf/cci_value.cpp

OBJS = cnf/cci_broker_manager.o \
       cnf/cci_broker_stack.o \
       cnf/cci_value.o
 
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
LIBDIR=$(GS_CCI_HOME)/lib

all: $(LIBDIR)/$(LIB)

$(LIBDIR)/$(LIB): $(OBJS)
	mkdir -p $(LIBDIR)
	$(AR) rcs $@ $^
	ranlib $@

-include $(DEPS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(DEFFLAGS) $(INCDIR) -c $< -MMD -o $@

.cc.o:
	$(CXX) $(CXXFLAGS) $(DEFFLAGS) $(INCDIR) -c $< -MMD -o $@

clean:
	rm -f $(OBJS) $(DEPS)
	rm -f $(LIBDIR)/$(LIB)
	-rmdir $(LIBDIR) 2>&1 || true
