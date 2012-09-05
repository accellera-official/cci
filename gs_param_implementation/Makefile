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
OPT    = -O0 -Wall
OTHER  = -pedantic -Wno-long-long
CFLAGS = $(OPT) $(OTHER)

#SRCS = json_spirit/json_spirit_reader.cpp \
#       json_spirit/json_spirit_writer.cpp \
#       gs_cci_value.cpp \
#       gs_cci_symbols_templ_small_int.cpp \
#       gs_cci_symbols.cpp

OBJS = json_spirit/json_spirit_reader.o \
       json_spirit/json_spirit_writer.o \
       gs_cci_symbols_templ_small_int.o \
       gs_cci_symbols.o
 

# Makefile.defs
## ------------------------------------------------------------------------------------

BOOST  ?=/cad/tools/boost

ifdef SYSTEMC_HOME
SYSTEMC=$(SYSTEMC_HOME)
else
SYSTEMC?=/cad/tools/systemc
endif

## set some directories 
GS_CCI_HOME ?= /Users/schroede/mydev/cci_git/cci
GS_CONTROL_DIR = $(GS_CCI_HOME)/greencontrol_cci_branch
GREENSTARCORE_DIR = $(GS_CCI_HOME)
CCI_DIR = $(GS_CCI_HOME)/api
GS_CCI_PARAM_DIR = $(GS_CCI_HOME)/gs_param_implementation
GS_CCI_API_DIR = $(GS_CCI_HOME)/gs_api_implementation
BOOST ?= /cad/tools/boost
SYSTEMC ?= /cad/tools/systemc

DEFFLAGS = -DSC_INCLUDE_DYNAMIC_PROCESSES

INCDIR = -I$(CCI_DIR) \
         -I$(GS_CONTROL_DIR) \
         -I$(SYSTEMC)/include -I$(BOOST) # \
#        -I$(GS_CCI_BROKER_DIR) -I$(GREENSTARCORE_DIR) -I$(GS_CCI_API_DIR)

## ------------------------------------------------------------------------------------

LIB=libcciparamimpl.a

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) rcs $@ $^
	ranlib $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $(SCCFLAG) $(DEFFLAGS) $(INCDIR) \
	  -c $< -o $@

.cc.o:
	$(CXX) $(CXXFLAGS) $(SCCFLAG) $(DEFFLAGS) $(INCDIR) \
	  -c $< -o $@

clean:
	rm -f  $(OBJS) $(LIB)

bigsymbols:
	$(CXX) $(CXXFLAGS) $(SCCFLAG) $(DEFFLAGS) \
		-DCCI_SYMBOLS_LOWER_BOUND=$(CCI_SYMBOLS_LOWER_BOUND) \
		-DCCI_SYMBOLS_UPPER_BOUND=$(CCI_SYMBOLS_UPPER_BOUND) \
		$(INCDIR) \
		-c gs_cci_symbols_templ_big_int.cpp -o $(OBJNAME).o $<

full_library: $(OBJS) $(OBJNAME).o
	$(AR) rcs $(LIB) $^
	ranlib $(LIB)
