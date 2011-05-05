export FLAG_BATCH = 1

SUBDIRS = \
          ../ex1_simple_int_param \
          ../ex2_fully_supported_data_type_param \
          ../ex3_elab_time_param \
          ../ex4_simple_immutable_param \
          ../ex5_default_and_init_value \
          ../ex6_parameter_naming \
          ../ex7_Parameter_Information	\
          ../ex8_parameter_value_change_callbacks \
					../ex9_Hierarchical_Override_of_Parameter_Values		
	
.PHONY: all clean check run

all:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-unix && $(MAKE)); done

check:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-unix && $(MAKE) check); done

run:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-unix && $(MAKE) run); done

clean:
	for i in $(SUBDIRS); do (cd $$i/build-unix && $(MAKE) clean); done; \

cleanall:
	for i in $(SUBDIRS); do (cd $$i/build-unix && $(MAKE) cleanall); done; \

