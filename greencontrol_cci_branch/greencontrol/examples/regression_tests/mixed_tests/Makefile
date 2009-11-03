# enabling SystemC2.1 automatically changes 
# includes and compiler flags in Makefile.defs
#SYSTEMC21V1 = yes

CC     = g++
#OPT    = -O3 -funroll-loops -mtune=pentium4
OPT  = -g -Wall -lpthread -D_REENTRANT
#OTHER  = -Wno-deprecated
#OTHER = -fprofile-arcs -ftest-coverage
CFLAGS = $(OPT) $(OTHER)
# EXTRA_LIBS = -lboost_program_options # -lgcov

MODULE = testbench


SRCS = testbench.cpp testip.cpp testip2.cpp testip3.cpp testtool.cpp testipVec.cpp ParamListIP.cpp AttributeTestsIP.cpp scattribparam.cpp sceventparam.cpp

OBJS = $(SRCS:.cpp=.o)

include ../../Makefile.gspm
include ../../Makefile.defs

all: $(EXE)

