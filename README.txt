README.txt
----------

This file provides brief instructions for getting started with the CCI 
Configuration standard.  

- unpack the CCI Config release kit

- install rapidjson
   download from https://github.com/Tencent/rapidjson/releases/tag/v1.1.0
   extract to cci/packages within the kit
   rename to rapidjson (from e.g. rapidjson-1.1.0)
   Note: the official cci releases will later bundle json to simplify 
         installation.

- generate doxygen documentation in doc/ for a useful reference:
   doxygen cci_doxygen_examples.conf
   doxygen cci_doxygen_api_ref.conf
   Note: this may require installing doxygen and its dependent packages, such
         as graphviz.

- review the generated documentation in:
   doc/Examples/index.html
   doc/API_ref/index.html

- build the proof-of-concept library:
  Instructions are provided here for Linux; see msvc10/cci/README.txt for 
  Windows instructions.

  - Initialize environment variables
    - CCI_HOME (release kit root, e.g. .../cci)
    - SYSTEMC_HOME (top-level, e.g. .../systemc-2.3.2)

  - Change to the cci/src/ directory and invoke 'gmake'

    Additional flags to pass on the gmake command-line to customize
    the behavior:
      V=1       - build in verbose mode     (default: V=0)
      OPT=-O2   - build with optimization   (default: OPT=-g)
      CXX       - override compiler         (default: g++)
      CXXFLAGS  - additional compiler flags (default: empty)

- build the provided examples
  - follow instructions in examples/README.txt
    Note: be sure to review the PowerPoint presentation in each example's docs/
    directory for insights and explanations

Contained Files:
----------------

./
doc/       :  The LRM and overview tutorial presentation
src/       :  CCI source code, only include <cci_configuration> in user code
packages/  :  for rapidjson which is obtained separately (for now)
examples/  :  directory for user examples
scripts/   :  directory for build script(s)
msvc10/    :  Microsoft Visual Studio project files

-------------------------------------------------------------------------------
Licensing and Copyright

   See the separate LICENSE and NOTICE files to determine your rights
   and responsiblities for using SystemC CCI Configuration.

User Documentation

   You can find documentation for this release in the docs/ directory.

Release Notes

   See the separate RELEASENOTES file that provides up-to-date
   information about this release of SystemC CCI Configuration.

