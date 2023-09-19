!include <.\Makefile.config>

SUBDIRS = \
	..\cci\ex01_Simple_Int_Param \
	..\cci\ex02_Fully_Supported_Data_Type_Param \
	..\cci\ex03_Elab_Time_Param \
	..\cci\ex04_Simple_Immutable_Param \
	..\cci\ex05_Default_and_Preset_Value \
	..\cci\ex06_Parameter_Naming \
	..\cci\ex07_Parameter_Information \
	..\cci\ex08_Parameter_Value_Change_Callbacks \
	..\cci\ex09_Parent_Control_of_Preset_Values \
	..\cci\ex10_Shared_Parameters \
	..\cci\ex11_Directly_Associating_Parameters \
	..\cci\ex12_Indirectly_Associating_Parameters \
	..\cci\ex13_Parameter_Value_Locking \
	..\cci\ex14_Hiding_Parameters \
	..\cci\ex15_Inter_Parameter_Value_Constraints \
	..\cci\ex16_User_Defined_Data_Type \
	..\cci\ex17_Preset_Values_from_Config_File \
	..\cci\ex18_Preset_Values_Superset \
	..\cci\ex19_Reset_Param \
	..\cci\ex20_Tracing_Parameter_Activity \
	..\cci\ex21_Param_Destruction_Resurrection \
	..\cci\ex22_Search_Predicate \
	..\cci\ex23_Hierarchical_Value_Update


TARGETS = build run check clean

# default target
all: build

# see
#  - Makefile.rules  for actual implementation
#  - Makefile.config for available configuration options
$(TARGETS):
	@for %%i in ($(SUBDIRS)) do \
	  @( chdir $(MAKEDIR)\%%i && \
	    $(MAKE) /f Makefile.msvc /nologo FLAG_BATCH=1 \
	      CONFIG=$(CONFIG) PLATFORM=$(PLATFORM) $@ )
