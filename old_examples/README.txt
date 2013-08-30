================================================================================
                        OSCI-CCI Examples
Date: 04/12/2011
================================================================================

ToC
---
  Introduction
  Directory Structure
  Running the examples
  The examples
  TODO
  ISSUES

Introduction:
-------------
This README describes the content and structure of examples in the cci/examples/ 
folder.

The CCI examples are patterned after the TLM2 examples. The examples implemented 
here are based on the brief in 'Description of Config Examples.docx'.

Directory Structure:
--------------------
          examples/
                |- build-msvc           // Windows build scripts
                |- build-unix           // *nix build scripts
                |- common               // Code common to one or more examples
                |       |- inc          //   Common headers
                |       |- src          //   Common sources
                |       |- lib*         // Lib folder to install CCI-libs
                |- ex1_xxx              // ex[Index]_[Example Name]
                |       |- doc          //   PPT having example description
                |       |- reports      //   Contains 'expected.log' file
                |       |- build-msvc   //   Windows build dir
                |       |- build-unix   //   *nix build dir
                |       |- inc          //   Example headers
                |       |- src          //   Example sources
                |- ...                  // More examples

(*) The GS-CCI static libraries are built for the first example and are stored in 
common/lib folder for use by subsequent examples.

Running the examples:
---------------------
Support for building the examples and unit tests is provided for 'unix', and
'msvc' (project and make files).

Makefiles work hierarchically. The default target builds the tests; 
Other targets are:
   'clean', 
   'run'  (executes built tests/examples) 
   'check' (against expected results)

To run the makefiles the following environment needs to be set:
   On Unix/Linux/MacOS, 
      $TLM_HOME should be set to this kit's location
      $SYSTEMC_HOME should point to the SystemC installation
      $BOOST_HOME should point to the Boost library installation
      $GS_CCI_HOME should point to the GreenSocs-CCI reference installation
      $CCI_EXAMPLES_HOME should point to the cci/examples/ dir
      $TARGET_ARCH defines the appendix of the systemc library directory
        these should be set such that $SYSTEMC_HOME/lib-$TARGET_ARCH is a 
        directory containing the systemc library.  
   Or alternatively,
      Update the DEFAULT_* paths in build-unix/Makefile.common with paths
      specific to your installation.

todo |   On Windows, 
todo |      the SYSTEMC and TLM environments variables should be used.
todo |   
todo |   To run the tests with SystemC-2.1v1 and gcc-3.4.4 you need to change
todo |   the Makefile.config in build-unix/ to unset the FLAG_WERROR
todo |   You also need to unset this flag when using gcc-4.1.2 and above
todo |
todo | To run the Visual C++ solutions (.sln) or project (.vcproj) files, you must edit  
todo | the Property sheet as follows:
todo | 
todo | 1 Select Property Manager from the View menu
todo | 2 Under projectName > Debug | Win32 select systemc
todo | 3 Select Properties from the View menu
todo | 4 Select User Macros under Common Properties
todo | 5 Update the SYSTEMC and TLM entries and apply

The examples:
-------------
Each examples comes with documentation in the form of a set of powerpoint slides
that can be found in the docs/ subdirectory of the example.

XREPORT() class of convenience macros are defined in common/inc dir.

The examples have been tested in the following environment:
 - SystemC-2.2
 - greenstarcore_ver613
 - RedHat Enterprise Linux 4
 - gcc-3.4.6 (32-bit)

TODO:
-----
1. Lines prefixed with 'todo |' needs to be updated.
2. The Windows build scripts need to be created.

ISSUES:
------
1. General
    a. Unimplemented features are marked with 'Warning's/TBD.
2. Link time error reported when the 3 component CCI libraries are built 
and linked independently. Hence the component CCI libs are archived as a single lib
-lgs_cci.

