#-- [Do not Update] 
#---- Depth with respect to common build dir
DEPTH=../../build-unix

#-- [Update] 
#---- Add source files to be compiled for this example without path
SRCS_WOPATH=testbench.cpp

#-- [Update, if required] 
#---- Update sources with path and derive object names
SRCS=$(addprefix ../src/,$(SRCS_WOPATH))
OBJS=$(SRCS:.cpp=.o)
#---- Any extra include/ dirs and libraries to be linked in
INC_DIR=-I../inc 
LIB_DIR=

#-- [Update] Name of executable
EXE=elab_time_param.exe

#-- [Do not Update]
include $(DEPTH)/Makefile.config

