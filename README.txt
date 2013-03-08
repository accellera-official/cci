README.txt
----------

OSCI CCI WG internal document with
the WG's API proposal and
GreenSocs' proof-of-concept implementation.

- checkout from git repository 
  https://github.com/OSCI-WG/cci.git
  (ask David Black for access)

- generate doxygen documentation in doc:
   doxygen cci_doxygen_api_ref.conf
    and
   doxygen cci_doxygen_gs_impl_ref.conf
- start looking at documentation in doc/API_ref/index.html

- for compile development examples:
  - edit Makefiles
    - gs_broker_implementation/Makefile
    - gs_param_implementation/Makefile
    - api/Makefile
    - Makefiles.defs
  - compile in
    - api
    - gs_broker_implementation
    - gs_param_implementation
    - dev_examples/gs_example (or other example)
    (currently there are compiler warnings)
    (read README.txt files if existing)
  - run ./testbench in gs_example (or any other example dir)
(Note that instead of providing one Makefile or compile script for all
there are different compiles to be done manually to demonstrate the 
independency and make it easier to provide alternative implementations)

- for compile official examples
  - read examples/README.txt



Contained Files:
----------------

./
doc/   :  Some further documentation
api/   :  CCI interface files, only include cci.h in user code
gs_broker_implementation/ :  GreenSocs broker    implementation
gs_param_implementation/  :  GreenSocs parameter implementation
dev_examples/                     :  some development examples
             gs_example/          :  Example using the GreenSocs implementation
             priv_broker_example/ :  Example demonstrating usage of a private broker
             gs_example_diff_impl :  still TODO!!
examples/  :  directory for official examples

