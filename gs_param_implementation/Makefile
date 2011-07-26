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


CC     = g++
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
       gs_cci_value.o \
       gs_cci_symbols_templ_small_int.o \
       gs_cci_symbols.o
 

# Makefile.defs
## ------------------------------------------------------------------------------------

## target architecture (if not defined before)
ifndef TARGET_ARCH
 TARGET_ARCH = macosx
endif

## set some directories 
GS_CONTROL_DIR = /Users/schroede/mydev/cci_git/cci/greencontrol_cci_branch
GREENSTARCORE_DIR = /Users/schroede/mydev/cci_git/cci
CCI_DIR = /Users/schroede/mydev/cci_git/cci/api
GS_CCI_PARAM_DIR = /Users/schroede/mydev/cci_git/cci/gs_param_implementation
GS_CCI_API_DIR = /Users/schroede/mydev/cci_git/cci/gs_api_implementation
BOOST = /cad/tools/boost
SYSTEMC = /cad/tools/systemc

DEFFLAGS = -DSC_INCLUDE_DYNAMIC_PROCESSES


## muldefs should be avoided in the source code, not by the linker
LDFLAGS =

INCDIR = -I$(GS_CONTROL_DIR) -I$(BOOST) \
         -I. -I.. -I$(SYSTEMC)/include -I$(GREENSTARCORE_DIR) -I$(CCI_DIR) -I$(GS_CCI_PARAM_DIR) -I$(GS_CCI_API_DIR)
LIBDIR = -L. -L.. -L$(SYSTEMC)/lib-$(TARGET_ARCH)

## here may be needed to write -lscv at the beginning - which also may cause problems!
LIBS   =  -lsystemc -lm $(EXTRA_LIBS)

## ------------------------------------------------------------------------------------



all: myprog
	ar -r "libcciparamimpl.a" *.o
	rm -f  *.o

myprog: $(OBJS)

.cpp.o:
	$(CC) $(CFLAGS) $(SCCFLAG) $(DEFFLAGS) $(INCDIR) -c $<

.cc.o:
	$(CC) $(CFLAGS) $(SCCFLAG) $(DEFFLAGS) $(INCDIR) -c $<

clean:
	rm -f  *.o libcciparamimpl.a

bigsymbols:
	$(CC) $(CFLAGS) $(SCCFLAG) $(DEFFLAGS) -DCCI_SYMBOLS_LOWER_BOUND=$(CCI_SYMBOLS_LOWER_BOUND) -DCCI_SYMBOLS_UPPER_BOUND=$(CCI_SYMBOLS_UPPER_BOUND) $(INCDIR) -c gs_cci_symbols_templ_big_int.cpp -o $(OBJNAME).o $<

full_library:
	ar -r "libcciparamimpl.a" *.o
	rm -f  *.o

