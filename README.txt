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
    - SYSTEMC_HOME   (top-level, e.g. .../systemc-2.3.2)

  - build POC library by invoking 'gmake' in the following directory:
    - src

    Additional flags to pass on the gmake command-line to customize
    the behavior:
      V=1       - build in verbose mode     (default: V=0)
      OPT=-O2   - build with optimization   (default: OPT=-g)
      CXX       - override compiler         (default: g++)
      CXXFLAGS  - additional compiler flags (default: empty)

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
examples/  :  directory for user examples
scripts/   :  directory for build script(s)
msvc10/    :  Microsoft Visual Studio project files
