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

- review the generated documentation in:
   doc/Examples/index.html
   doc/API_ref/index.html

- build and run examples:
  See README.txt and 'CCI Instructions for MSVS2015.pdf' in msvc10/cci for 
  Windows-specific instructions.

  - Initialize environment variables
    - CCI_HOME (git root)
    - BOOST_HOME (Boost include dir)
    - SYSTEMC_HOME (top-level, e.g. .../systemc-2.3.2)
    - TARGET_ARCH (e.g. linux64)

  - build libraries by invoking 'gmake' in the following directories:
    - src
    - packages/gs_broker
   
  - run individual examples or the full suite
    - follow instructions in examples/README.txt
    - be sure to review the PowerPoint presentation in each example's docs/
      directory for insights and explanations

Contained Files:
----------------

./
doc/       :  Some further documentation
src/       :  CCI interface files, only include <cci_configuration> in user code
packages/  :  external packages, namely rapidjson
    greencontrol/ : GreenSocs configuration implementation
    gs_broker/    : GreenSocs broker wrapper
examples/  :  directory for user examples
scripts/   :  directory for build script(s)
msvc10/    :  Microsoft Visual Studio project files
