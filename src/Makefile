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

ifeq ($(origin CXX),default)
  CXX := g++
endif

# build optimization and debug information
#OPT := -O2 -g
OPT  := -g

# control compiler warnings
WARN := -Wall -Werror
WARN += -pedantic -Wno-long-long

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

DEFINES := -DSC_INCLUDE_FX

INCLUDES := -I$(CCI_DIR)
INCLUDES += -I$(SYSTEMC)/include
INCLUDES += -I$(RAPIDJSON)/include

## -----------------------------------------------------------------------------
V ?= 0

define cmd-banner
printf "  %-8s %s\n" "$1" "$2"
endef

AT_0 = @
AT   = $(AT_$(V))

AT_CXX_0 = @$(call cmd-banner,CXX,$@);
AT_CXX   = $(AT_CXX_$(V))

AT_MKDIR_0 = @$(call cmd-banner,MKDIR,$(@));
AT_MKDIR   = $(AT_MKDIR_$(V))

AT_LIB_0 = @$(call cmd-banner,LIB,$(notdir $(@)));
AT_LIB   = $(AT_LIB_$(V))

## -----------------------------------------------------------------------------

SRCS := $(wildcard cci_cfg/*.cpp)
SRCS += $(wildcard cci_core/*.cpp)
SRCS += $(wildcard cci_utils/*.cpp)

OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

LIB    := libcciapi.a
LIBDIR := $(CCI_HOME)/lib

all: $(LIBDIR)/$(LIB)

$(LIBDIR):
	$(AT_MKDIR)mkdir -p $(LIBDIR)

$(LIBDIR)/$(LIB): $(OBJS) | $(LIBDIR)
	$(AT_LIB)$(AR) rcs $@ $^
	$(AT)ranlib $@

-include $(DEPS)

.cpp.o:
	$(AT_CXX)$(CXX) $(OPT) $(WARN) $(DEFINES) $(INCLUDES) $(CXXFLAGS) -c $< -MMD -o $@

clean:
	$(AT)rm -vf $(OBJS) $(DEPS)
	$(AT)rm -vf $(LIBDIR)/$(LIB)
	$(AT)[ ! -d $(LIBDIR) ] || rmdir --ignore-fail-on-non-empty $(LIBDIR)
