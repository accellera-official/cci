##############################################################################
#
#  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
#  more contributor license agreements.  See the NOTICE file distributed
#  with this work for additional information regarding copyright ownership.
#  Accellera licenses this file to you under the Apache License, Version 2.0
#  (the "License"); you may not use this file except in compliance with the
#  License.  You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
#  implied.  See the License for the specific language governing
#  permissions and limitations under the License.
#
##############################################################################

ifndef CC
CC     = gcc
endif
ifndef CXX
CXX    = g++
endif
OPT    = -g -Wall -Werror -Wno-unused-local-typedefs
OTHER  = -pedantic -Wno-long-long
CXXFLAGS = $(OPT) $(OTHER)

SRCS := $(wildcard cci_cfg/*.cpp)
SRCS += $(wildcard cci_core/*.cpp)
SRCS += $(wildcard cci_utils/*.cpp)

OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

## -----------------------------------------------------------------------------

## set some directories
CCI_HOME ?= $(patsubst %/,%,$(dir $(CURDIR)))
CCI_DIR  ?= $(CCI_HOME)/src

ifdef RAPIDJSON_HOME
RAPIDJSON=$(RAPIDJSON_HOME)
endif
RAPIDJSON ?= $(CCI_HOME)/packages/rapidjson

ifdef SYSTEMC_HOME
SYSTEMC = $(SYSTEMC_HOME)
endif
SYSTEMC ?= /cad/tools/systemc

ifdef BOOST_HOME
BOOST := $(BOOST_HOME)
endif
BOOST ?= /cad/tools/boost

DEFFLAGS = -DSC_INCLUDE_DYNAMIC_PROCESSES -DSC_INCLUDE_FX

INCDIR += -I.
INCDIR := -I$(CCI_DIR)
INCDIR += -I$(SYSTEMC)/include
INCDIR += -I$(RAPIDJSON)/include
INCDIR += -I$(BOOST)

## -----------------------------------------------------------------------------

LIB=libcciapi.a
LIBDIR=$(CCI_HOME)/lib

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
