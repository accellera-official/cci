#-- [Do not Update] 
#---- Depth with respect to common build dir
DEPTH=../../build-unix

#-- [Update] 
#---- Add source files to be compiled for this example without path
SRCS_WOPATH=main.cpp

#-- [Update, if required] 
#---- Update sources with path and derive object names
SRCS=$(addprefix ../src/,$(SRCS_WOPATH))
OBJS=$(SRCS:.cpp=.o)

#---- Any extra include/ dirs and libraries to be linked in
INC_DIR=-I../inc \
        -I$(GS_CCI_HOME)/gs_broker_implementation \
        -I$(GS_CCI_HOME)/greencontrol_cci_branch
LIB_DIR=

#-- [Update] Name of executable
EXE=ex14_Hiding_Parameters.x

#EXE_ARGS= | tee ../reports/expected.log 

#-- [Do not Update]
include $(DEPTH)/Makefile.config

