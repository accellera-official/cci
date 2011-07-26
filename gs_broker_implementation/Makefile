# enable compiler flag -DCCI_CNF_IMPL_VERBOSE to get broker
#  implementation debug output (see CFLAGS)

CC     = g++
OPT    = -g -Wall 
OTHER  = -pedantic -Wno-long-long
CFLAGS = $(OPT) $(OTHER) # -DCCI_CNF_IMPL_VERBOSE 

SRCS = gs_cci_broker.cpp \
       gs_cci_cnf_broker.cpp \
       gs_cci_cnf_broker_accessor.cpp \
       gs_cci_cnf_broker_accessor_handler.cpp \
       gs_cci_cnf_private_broker.cpp \
       gs_cci_cnf_private_broker_accessor.cpp \
       helpers.cpp

OBJS = gs_cci_broker.o \
       gs_cci_cnf_broker.o \
       gs_cci_cnf_broker_accessor.o \
       gs_cci_cnf_broker_accessor_handler.o \
       gs_cci_cnf_private_broker.o \
       gs_cci_cnf_private_broker_accessor.o \
       helpers.o
 
OBJS_W_PATHS = $(SRCS:.cpp=.o)


#include ../Makefile.defs
## ------------------------------------------------------------------------------------

## target architecture (if not defined in Makefile)
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
	ar -r "libccibrokerimpl.a" *.o
	rm -f  *.o

myprog: $(OBJS)

.cpp.o:
	$(CC) $(CFLAGS) $(SCCFLAG) $(DEFFLAGS) $(INCDIR) -c $<

.cc.o:
	$(CC) $(CFLAGS) $(SCCFLAG) $(DEFFLAGS) $(INCDIR) -c $<

clean:
	rm -f  *.o libccibrokerimpl.a
