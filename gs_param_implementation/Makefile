# usage for big_ints:
# make bigsymbols "CCI_SYMBOLS_LOWER_BOUND=1" "CCI_SYMBOLS_UPPER_BOUND=100" OBJNAME=big1
# etc.

#
# small make without bigsymbols
# compile time on Intel Core Duo 1.83 GHz (MacMini) 2 GB DDR2 SDRAM
# -O2 : 13 minutes
# -O0 :  3 minutes (45.5 MB)
#
# big make with bigsymbols
# compile time on Intel Core Duo 1.83 GHz (MacMini) 2 GB DDR2 SDRAM
# -O2 : real 213m user 197m (511 MB (stripped))
# -O0 : real 43m user 33m (550 MB)
#


CC     = gcc
CXX    = g++
OPT    = -g -Wall
OTHER  = -pedantic -Wno-long-long -Wno-unused-local-typedefs
CXXFLAGS = $(OPT) $(OTHER)

OBJS = json_spirit/json_spirit_reader.o \
       json_spirit/json_spirit_writer.o \
       gs_cci_symbols_templ_small_int.o \
       gs_cci_symbols.o
 
DEPS = $(OBJS:.o=.d)

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
CCI_DIR = $(GS_CCI_HOME)/api

DEFFLAGS = -DSC_INCLUDE_DYNAMIC_PROCESSES

INCDIR = -I$(CCI_DIR) -I$(GS_CONTROL_DIR)\
         -I$(SYSTEMC)/include -I$(BOOST) 

## -----------------------------------------------------------------------------

LIB=libcciparamimpl.a
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

bigsymbols:
	$(CXX) $(CXXFLAGS) $(DEFFLAGS) \
		-DCCI_SYMBOLS_LOWER_BOUND=$(CCI_SYMBOLS_LOWER_BOUND) \
		-DCCI_SYMBOLS_UPPER_BOUND=$(CCI_SYMBOLS_UPPER_BOUND) \
		$(INCDIR) \
		-c gs_cci_symbols_templ_big_int.cpp -o $(OBJNAME).o $<

full_library: $(OBJS) $(OBJNAME).o
	$(AR) rcs $(LIBDIR)/$(LIB) $^
	ranlib $(LIBDIR)/$(LIB)
