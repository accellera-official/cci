include $(top_srcdir)/config/Make-rules.examples

all:
	@echo "To compile and run the examples call"
	@echo "   make check "


# begin with empty variables, is filled later in test.am files
examples_BUILD = 
examples_CLEAN = 
examples_DIRS =
examples_FILES = 
examples_TESTS = 
examples_XFAIL_TESTS = 


# include simple tests
include $(srcdir)/ex01_Simple_Int_Param/test.am
include $(srcdir)/ex02_Fully_Supported_Data_Type_Param/test.am
include $(srcdir)/ex03_Elab_Time_Param/test.am
include $(srcdir)/ex04_Simple_Immutable_Param/test.am
include $(srcdir)/ex05_Default_and_Preset_Value/test.am
include $(srcdir)/ex06_Parameter_Naming/test.am
include $(srcdir)/ex07_Parameter_Information/test.am
include $(srcdir)/ex08_Parameter_Value_Change_Callbacks/test.am
include $(srcdir)/ex09_Parent_Control_of_Preset_Values/test.am
include $(srcdir)/ex10_Shared_Parameters/test.am
include $(srcdir)/ex11_Directly_Associating_Parameters/test.am
include $(srcdir)/ex12_Indirectly_Associating_Parameters/test.am
include $(srcdir)/ex13_Parameter_Value_Locking/test.am
include $(srcdir)/ex14_Hiding_Parameters/test.am
include $(srcdir)/ex15_Inter_Parameter_Value_Constraints/test.am
include $(srcdir)/ex16_User_Defined_Data_Type/test.am
include $(srcdir)/ex17_Preset_Values_from_Config_File/test.am
include $(srcdir)/ex18_Preset_Values_Superset/test.am
include $(srcdir)/ex19_Reset_Param/test.am
include $(srcdir)/ex20_Tracing_Parameter_Activity/test.am
include $(srcdir)/ex21_Param_Destruction_Resurrection/test.am
include $(srcdir)/ex22_Search_Predicate/test.am
include $(srcdir)/ex23_Hierarchical_Value_Update/test.am

examples_DIRS += \
				 ex01_Simple_Int_Param \
				 ex02_Fully_Supported_Data_Type_Param \
				 ex03_Elab_Time_Param \
				 ex04_Simple_Immutable_Param \
				 ex05_Default_and_Preset_Value \
				 ex06_Parameter_Naming \
				 ex07_Parameter_Information \
				 ex08_Parameter_Value_Change_Callbacks \
				 ex09_Parent_Control_of_Preset_Values \
				 ex10_Shared_Parameters \
				 ex11_Directly_Associating_Parameters \
				 ex12_Indirectly_Associating_Parameters \
				 ex13_Parameter_Value_Locking \
				 ex14_Hiding_Parameters \
				 ex15_Inter_Parameter_Value_Constraints \
				 ex16_User_Defined_Data_Type \
				 ex17_Preset_Values_from_Config_File \
				 ex18_Preset_Values_Superset \
				 ex19_Reset_Param \
				 ex20_Tracing_Parameter_Activity \
				 ex21_Param_Destruction_Resurrection \
				 ex22_Search_Predicate \
				 ex23_Hierarchical_Value_Update

CLEANFILES += $(examples_CLEAN)

check_PROGRAMS = $(examples_TESTS)

TESTS = $(check_PROGRAMS)

XFAIL_TESTS = $(examples_XFAIL_TESTS)

# install examples?
if SEPARATE_INSTALL_TREE
nobase_examples_DATA = \
	$(examples_FILES)

uninstall-hook:
	test ! -d "$(examplesdir)" || ( set -e ; cd "$(examplesdir)" ; \
		for dir in $(examples_DIRS) ; do test ! -d $$dir || rmdir $$dir ; done ; \
		cd - ; rmdir "$(examplesdir)" )

endif # SEPARATE_INSTALL_TREE

EXTRA_DIST += \
	$(examples_FILES)

CLEANFILES += \
	$(examples_CLEAN) \
	$(check_SCRIPTS)

## Taf!
