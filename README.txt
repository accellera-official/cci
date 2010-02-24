README.txt
----------

OSCI CCI WG internal document / archive file with
GreenSocs interface proposal.

- extract both provided archive files
  - CCI API proposal
  - GreenSocs implementation and example

- start looking at documentation in doc/API_ref/index.html

- compile using make in gs_example, 
  first edit Makefiles.defs setting some directories on top
  (currently there are compiler warnings)


Contained Files:
----------------

./
doc/   :  Some further documentation
api/   :  CCI interface files, only include cci.h in user code
gs_api_implementation/    :  GreenSocs Config API implementation
gs_param_implementation/  :  GreenSocs parameter implementation
gs_example/         :  Example using the GreenSocs implementation
