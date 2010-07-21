README.txt
----------

OSCI CCI WG internal document with
GreenSocs interface proposal.

either
- extract both provided archive files
  - CCI API proposal
  - GreenSocs implementation and example
or
- checkout from svn 
  https://svn.greensocs.com/private/packages/greenstarcore
  (ask Mark Burton for access)

- start looking at documentation in doc/API_ref/index.html

- edit Makefiles
  - gs_api_implementation/Makefile
  - gs_param_implementation/Makefile
  - Makefiles.defs

- compile in
  - cci
  - gs_api_implementation
  - gs_param_implementation
  - gs_example (or other example)
  (currently there are compiler warnings)

- run ./testbench in gs_example (or any other example dir)

(Note that there are different compiles to be done to demontrate the 
independency and make it easier to provide alternative implementations)


Contained Files:
----------------

./
doc/   :  Some further documentation
api/   :  CCI interface files, only include cci.h in user code
gs_api_implementation/    :  GreenSocs Config API implementation
gs_param_implementation/  :  GreenSocs parameter implementation
gs_example/          :  Example using the GreenSocs implementation
priv_broker_example/ :  Example demonstrating usage of a private broker