# README 

This directory allows the examples to be launched from 
the command line on Windows using Microsoft Visual Studio C++.

**NOTE**: In normal use, you'll probably find it easier to use the Visual C++ integrated
development environment. However this command line build system is useful for
running the examples with self-checking.

# Usage

 1. Check the settings in the file Makefile.config

    Check the path to the SystemC, CCI, and RapidJSON installation.  
    The setup requires the following corresponding environment variables:
    SYSTEMC_HOME, CCI_HOME, RAPIDJSON_HOME, SYSTEMC_MSVC, CCI_MSVC.

    If you want to use the CCI reference implementation and examples with newer 
    versions of MSVC, it is recommended to copy the MSVC project
    files to a new subdirectory (e.g., msvc<version>/cci) before upgrading
    the project files.  Afterwards, update the CCI_MSVC variable to point
    to the newly versions msvc directory.

    If you use a full-featured Visual Studio version, you might
    want to use the VCBuild-based command-line interface.
    Uncomment the "VCBUILD" setting in Makefile.config.

 2. Double-click openhere.bat to open a cmd prompt

 3. Run vsvars.bat to set up Visual C++ paths

    You can specify the version and the platform to use via additional
    command-line options given to vsvars.bat, e.g.

    ```
    vsvars.bat [arch]               # load MSVC 2005 for [arch]
    vsvars.bat [version]            # load MSVC [version] for x86
    vsvars.bat [version] [platform] # load MSVC [version] for [platform]
    ```

    Examples:

    ```
    vsvars.bat 2010 amd64  # load 64-bit tools for MSVC 10.0
    vsvars.bat 11.0        # load default (x86) tools for MSVC 2012
    vsvars.bat x86_amd64   # load x64 cross-tools for MSVC 2005
    ```

 4. You can then use the Microsoft tool nmake, e.g.

    ```
    nmake build            # compile unit tests [default]
    nmake run              # run unit tests
    nmake check            # run unit tests and compare against golden results
    nmake clean            # clean unit tests
    ```

Additionally, you can override the settings from Makefile.config via the
command-line as well.  Common settings are

  ```
  CONFIG    (Debug, Release, Both): build specific configuration
  PLATFORM  (Win32, x64, Both)    : build for given platform(s)
  VCBUILD                         : command-line tool to build project files
  V=1                             : build with more verbosity
  ```

Build examples:

 - Build, run and compare output for "Release" configuration on 32 and 64-bit,
   by using the VCBuild command-line interface:
   `nmake /nologo CONFIG=Release PLATFORM=Both VCBUILD=vcbuild check`

 - Build and run examples for "Release" and "Debug" configurations on 32-bit
   by using the compiler and linker directly:
   `nmake /nologo CONFIG=Both run`
