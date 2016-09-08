
ifndef CC
CC     = gcc
endif
ifndef CXX
CXX    = g++
endif
OPT    = -g -Wall -Werror -Wno-unused-local-typedefs
OTHER  = -pedantic -Wno-long-long
CXXFLAGS = $(OPT) $(OTHER)

SRCS = cci_cfg/cci_broker_manager.cpp \
       cci_cfg/cci_broker_stack.cpp \
       cci_cfg/cci_value.cpp \
       cci_cfg/cci_value_traits.cpp \
       cci_cfg/cci_originator.cpp \
	   cci_cfg/cci_param_untyped.cpp \
	   cci_cfg/cci_param_untyped_handle.cpp

OBJS = $(SRCS:.cpp=.o)

OBJS_W_PATHS = $(SRCS:.cpp=.o)

DEPS := $(OBJS:.o=.d)

## -----------------------------------------------------------------------------

ifdef BOOST_HOME
BOOST = $(BOOST_HOME)
endif
BOOST ?= /cad/tools/boost

ifdef GREENCONTROL_HOME
GREENCONTROL=$(GREENCONTROL_HOME)
endif
GREENCONTROL ?= $(GS_CCI_HOME)/packages

ifdef RAPIDJSON_HOME
RAPIDJSON=$(RAPIDJSON_HOME)
endif
RAPIDJSON ?= $(GS_CCI_HOME)/packages/rapidjson

ifdef SYSTEMC_HOME
SYSTEMC = $(SYSTEMC_HOME)
endif
SYSTEMC ?= /cad/tools/systemc

ifdef CCI_HOME
GS_CCI_HOME = $(CCI_HOME)
endif
GS_CCI_HOME ?= $(shell cd $(CURDIR)/.. && pwd)

## set some directories 
CCI_DIR ?= $(GS_CCI_HOME)/src

DEFFLAGS = -DSC_INCLUDE_DYNAMIC_PROCESSES -DSC_INCLUDE_FX

INCDIR = -I. -I$(CCI_DIR) -I$(SYSTEMC)/include -I$(BOOST)  -I$(RAPIDJSON)/include \
         -I$(GREENCONTROL)

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
