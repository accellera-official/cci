README.txt
----------

Accellera CCI WG internal document with
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

- compile and run examples:
  - Initialize environment variables
    - CCI_HOME (git root)
    - BOOST_HOME (Boost include dir)
    - SYSTEMC_HOME (top-level, e.g. .../systemc-2.3.0)
    - TARGET_ARCH (e.g. linux64)
  - build libraries by invoking 'gmake' in the following directories:
    - api
    - gs_broker_implementation
    - gs_param_implementation
  - run individual examples or the full suite
    - follow instructions in scripts/README_CCI.txt
    - be sure to review the PowerPoint presentation in each example's docs/
      directory for insights and explanations

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
scripts/   :  directory for build script(s)

