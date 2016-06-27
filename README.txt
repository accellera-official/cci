README.txt
----------

This file provides brief WG member instructions for getting started with the
CCI Configuration standard.  Refer to 'Getting Started in the CCI WG.docx', 
available from the WG's Accellera workspace, for more detailed instructions.

- clone the git repository 
  https://github.com/OSCI-WG/cci.git
  - create and clone a fork if you intend to commit updates

- generate doxygen documentation in doc/:
   doxygen cci_doxygen_examples.conf
   doxygen cci_doxygen_api_ref.conf
   doxygen cci_doxygen_gs_impl_ref.conf
- review the generated documentation in:
   doc/Examples/index.html
   doc/API_ref/index.html
   doc/GS_implementation_ref/index.html

- build and run examples:
  See README.txt and 'CCI Instructions for MSVS2015.pdf' in msvc80/cci for 
  Windows-specific instructions.

  - Initialize environment variables
    - CCI_HOME (git root)
    - BOOST_HOME (Boost include dir)
    - SYSTEMC_HOME (top-level, e.g. .../systemc-2.3.1)
    - TARGET_ARCH (e.g. linux64)
  - build libraries by invoking 'gmake' in the following directories:
    - src
    - gs_broker_implementation
    - gs_param_implementation
  - run individual examples or the full suite
    - follow instructions in examples/README.txt
    - be sure to review the PowerPoint presentation in each example's docs/
      directory for insights and explanations

Contained Files:
----------------

./
doc/       :  Some further documentation
src/       :  CCI interface files, only include <cci_configuration> in user code
greencontrol_cci_branch/  :  GreenSocs configuration implementation
gs_broker_implementation/ :  GreenSocs broker wrapper
gs_param_implementation/  :  GreenSocs parameter wrapper
packages/  :  external packages, namely rapidjson
dev_examples/ :  developer examples (i.e. test cases)
examples/  :  directory for user examples
scripts/   :  directory for build script(s)
msvc80/    :  Microsoft Visual Studio project files
