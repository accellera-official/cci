
CC     = g++
OPT    = -g -Wall 
OTHER  = -pedantic -Wno-long-long
CFLAGS = $(OPT) $(OTHER)

SRCS = cnf/cci_broker_manager.cpp \
       cnf/cci_broker_stack.cpp \
       cnf/cci_value.cpp

OBJS = cnf/cci_broker_manager.o \
       cnf/cci_broker_stack.o \
       cnf/cci_value.o
 
OBJS_W_PATHS = $(SRCS:.cpp=.o)


#include ../Makefile.defs
## ------------------------------------------------------------------------------------

## target architecture (if not defined in Makefile)
ifndef TARGET_ARCH
 TARGET_ARCH = macosx
endif

## set some directories 
GS_CONTROL_DIR = /Users/schroede/mydev/greensocs_greencontrol/greenstarcore/greencontrol_cci_branch
GREENSTARCORE_DIR = /Users/schroede/mydev/greensocs_greencontrol/greenstarcore
CCI_DIR = /Users/schroede/mydev/greensocs_greencontrol/greenstarcore/api
GS_CCI_PARAM_DIR = /Users/schroede/mydev/greensocs_greencontrol/greenstarcore/gs_param_implementation
GS_CCI_API_DIR = /Users/schroede/mydev/greensocs_greencontrol/greenstarcore/gs_api_implementation
BOOST = /cad/tools/boost
SYSTEMC = /cad/tools/systemc

#SCV_INCLUDE = /cad/tools/systemc-scv/include
#SCV_LIB = /cad/tools/systemc-scv/lib-$(TARGET_ARCH)

DEFFLAGS = -DSC_INCLUDE_DYNAMIC_PROCESSES


## muldefs should be avoided in the source code, not by the linker
LDFLAGS =

INCDIR = -I$(GS_CONTROL_DIR) -I$(BOOST) \
         -I. -I.. -I$(SYSTEMC)/include -I$(GREENSTARCORE_DIR) -I$(CCI_DIR) -I$(GS_CCI_PARAM_DIR) -I$(GS_CCI_API_DIR)
LIBDIR = -L. -L.. -L$(SYSTEMC)/lib-$(TARGET_ARCH)

## here may be needed to write -lscv at the beginning - which also may cause problems!
LIBS   =  -lsystemc -lm $(EXTRA_LIBS)

## ------------------------------------------------------------------------------------


#all: $(EXE)

all: myprog
	ar -r "libcciapi.a" *.o
	rm -f  *.o

myprog: $(OBJS)

.cpp.o:
	$(CC) $(CFLAGS) $(SCCFLAG) $(DEFFLAGS) $(INCDIR) -c $<

.cc.o:
	$(CC) $(CFLAGS) $(SCCFLAG) $(DEFFLAGS) $(INCDIR) -c $<

clean:
	rm -f  *.o libcciapi.a
