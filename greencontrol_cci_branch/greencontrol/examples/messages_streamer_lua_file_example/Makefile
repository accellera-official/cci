# enabling SystemC2.1 automatically changes 
# includes and compiler flags in Makefile.defs
#SYSTEMC21V1 = yes

ifneq ($(strip $(GSROOT)),)
  # Using GSPM, load more dependencies in Makefile.gspm
  BUILD_DEPENDS = lua
endif

CC     = g++
#OPT    = -O3 -funroll-loops -mtune=pentium4
OPT  = -g -Wall -lpthread -D_REENTRANT
#OTHER  = -Wno-deprecated
CFLAGS = $(OPT) $(OTHER) -DSC_INCLUDE_DYNAMIC_PROCESSES
# CFLAGS = $(DEBUG) $(OTHER)
EXTRA_LIBS = -lscv -llua # -lboost_program_options

MODULE = testb_rep_msg_lua
SRCS = testb_rep_msg_lua.cpp
OBJS = $(SRCS:.cpp=.o)

include ../Makefile.gspm
include ../Makefile.defs

all: $(EXE)
