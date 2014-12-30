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

OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

OBJS_W_PATHS = $(SRCS:.cpp=.o)

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
LIBDIR=$(GS_CCI_HOME)/lib

all: $(LIBDIR)/$(LIB)

$(LIBDIR)/$(LIB): $(OBJS)
	mkdir -p $(LIBDIR)
	$(AR) rcs $@ $^
	ranlib $@

-include $(DEPS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(DEFFLAGS) $(INCDIR) -MMD -c $< -o $@

.cc.o:
	$(CXX) $(CXXFLAGS) $(DEFFLAGS) $(INCDIR) -MMD -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS)
	rm -f $(LIBDIR)/$(LIB)
	-rmdir $(LIBDIR) 2>&1 || true
