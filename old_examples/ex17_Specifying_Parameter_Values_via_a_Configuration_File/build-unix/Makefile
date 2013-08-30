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
INC_DIR=-I../inc
LIB_DIR=

#-- [Update] Name of executable
EXE=Specifying_Parmeter_Values_via_a_Configuration_File

#EXE_ARGS= | tee ../reports/expected.log 

#-- [Do not Update]
include $(DEPTH)/Makefile.config
INCS+=-I${GS_CCI_HOME}/greencontrol_cci_branch/greencontrol/gcnf/apis/configFileApi/
