# Building and running the examples

## Unix-like systems

In addition to running the examples during the SystemC CCI library build
process ('make check'), simple Makefiles are provided for building and
running the examples after the installation of the SystemC CCI library.

Each subdirectory contains a simple Makefile, which includes the top-level
Makefiles

 - Makefile.config

   Setup environment for the build.  Default values are provided, but some
   variables (like the TARGET_ARCH) may need to be provided from the
   environment.

   The following variables should  be set (or overridden from the environment):

     SYSTEMC_HOME           - path to SystemC installation
     CCI_HOME               - path to CCI installation
     RAPIDJSON_HOME         - path to RapidJSON library
     TARGET_ARCH            - target architecture
     ARCH_SUFFIX            - library architecture suffix (see INSTALL)

   For more details, please see Makefile.config directly.

 - Makefile.rules

   Default rules for building and running the examples.  The following
   targets are provided:
     all        - build example
     run        - run example
     check      - compare with expected output, if present
     clean      - clean example
     ultraclean - clean even more

For convenience, a simple recursing Makefile is available as well.

 - Makefile.all

   With this, a specific target can be invoked on all examples at once:

     gmake -f Makefile.all TARGET_ARCH=linux64 run

## Microsoft Visual C++

Project files for Microsoft Visual C++ 2015 (msvc14, 14.0) are provided in the 
subdidrectory containing each of these examples.

Each project file has the correct settings to build the example in either
Debug or Release modes.

The project files assume an environment variable names SYSTEMC_HOME, SYSTEMC_MSVC,
CCI_HOME, and RAPIDJSON_HOME are set.
