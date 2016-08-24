--------------------------------------------------------------------------------
README for building CCI libraries/examples/regression with Microsoft Visual C++

Authors: Lei Liang <lei.liang@ericsson.com>
         Philipp A. Hartmann <philipp.hartmann@offis.de>
Date   : 2014-12-30
--------------------------------------------------------------------------------

Table of Contents
-----------------
  - Introduction
  - Prerequisites
  - How to build the three PoC libraries
  - How to run regression tests
  - How to build examples


Introduction
-------------

This README gives step-by-step instructions for

  1. building all three proof-of-concept (PoC) libraries
    - api
    - packages/gs_broker
    - packages/gs_param
  
  2. running existing regression in a Windows environment
    - Cygwin or MSYS + MSVC

  3. building and running the existing examples with MSVC


Prerequisites
-------------

You shall have the following software/libraries before you start:

  1. SystemC installation       (mandatory for all operations in this guide)
  2. Cygwin/MSYS installation   (mandatory for regression tests in this guide)
  3. Boost library installation (mandatory for all operations in this guide)
  4. Microsoft Visual Studio
  5. rapidjson git submodule	(`git submodule update -i` from CCI root dir)

And they are verified with following versions:

  1. SystemC
     - 2.3.1 (required for 64-bit support)
     - 2.3.0

  2. Cygwin / MSYS
     - CYGWIN_NT-6.1
     - MINGW32_NT-6.1 1.0.17

     Note: Depending on your Cygwin/MSYS installation, you may need to install
           Perl and make sure that the `link.exe' points to the MSVC linker.

  3. Boost (only headers needed)
     - boost_1_57_0
     - boost_1_44_0

  4. Microsoft Visual Studio
     - Visual Studio 2005 Professional
     - Visual Studio 2010 Professional
     - Visual Studio 2013 Express

     Note: Please see the next section on upgrading to newer version of MSVC
           (later than 10.0/2010).


How to build the three PoC libraries
------------------------------------

  0. Upgrade the MSVC solution/projects, for newer MSVC versions (optional)

     The provided MSVC solution/project files are prepared for Microsoft
     Visual Studio 2010 (10.0).  You need to upgrade these files when using
     more recent versions of MSVC.

     - Copy the 'msvc10' directory to the correct SystemC architecture alias
       for your MSVC version:

       - 10.0/2010 - "msvc10" (default)
       - 11.0/2012 - "msvc11"
       - 12.0/2013 - "msvc12"

       IMPORTANT NOTE: you will need to manually sync future git updates to
       msvc10 once you've made a copy of this directory!

     - Open the solution file `cci.sln' with your version of MSVC and
       follow the upgrade instructions.

     Note: The same steps are needed for building the SystemC library itself.

     Note: 64-bit support is not available in MS Visual C++ 2010 Express by
           default. You need to install the "Microsoft Windows SDK v7.1"
           before upgrading the MSVC solution, which is available from
             http://www.microsoft.com/en-us/download/details.aspx?id=8279

  1. Open the prepared MSVC solution 

     - File -> Open -> Project/Solution
        - From your CCI home -> msvcXX -> cci -> cci.sln
          (with `msvcXX' matching your MSVC version as of Step 0)
     - Under the `cci.sln' solution, you can see three MSVC projects:
       - cciapi
       - ccibrokerimpl
       - cciparamimpl

  2. Update property sheet

     All three projects share the same property sheet `cci_lib.props',
     carrying the common setup for e.g. the SystemC and Boost dependencies.

     - Open the "Property Manager" via View -> Property Manager

       Note: On some MSVC Express editions, you need to enable the
             "Expert Settings" via Tools -> Settings -> Expert Settings

     - Open the `cci_lib' property sheet
       - Select cci_lib -> User Macros

     - Update/configure the provided User Macros for your environment

       Note: The default values assume the presence of the environment
             variables SYSTEMC_HOME and BOOST_HOME to locate the base
             directories of these libraries.  If you setup these two variables
             before starting MSVC, these libraries are already correctly found.

       * `MSVC' - MSVC architecture alias as used by SystemC

         This macro shall point to the correct `msvcXX' alias, matching the
         used MSVC version as described in Step 0 (default "msvc10").

       * `SYSTEMC' - location of MSVC directory of SystemC installation

         This macro shall point to the correct MSVC sub-directory of your
         SystemC installation (default: "$(SYSTEMC_HOME)\$(MSVC)").

         If you don't want to set up an environment variable `SYSTEMC_HOME`
         for the SystemC base directory, you can add a fixed path here as well,
         e.g. "c:\systemc-2.3.1\$(MSVC)"

       * `BOOST' - location of Boost header files

         This macro is an alias for the environment variable `$(BOOST_HOME)'
         by default.  You can set an explicit path as well,
         e.g. "c:\boost_1_57_0".

       Note: If your SystemC is not built according to the given MSVC project
             in the release, you may need to change the library path setup.

  3. Build the Solution

     - Choose the configuration
       - Debug / Release
       - Win32 / x64

         Note: MSVC 64-bit support has been added to SystemC in version 2.3.1.
               With older SystemC versions (e.g. 2.2.0/2.3.0), only 32-bit is
               available.

     - Build the whole cci solution (e.g. by hitting F7)

       All three CCI PoC libraries will be created for the given configuration.


How to run regression tests
---------------------------

To run the regression tests on Windows, a basic Un*x shell environment like
Cygwin or MinGW/MSYS including a Perl runtime is required.

Make sure that folder that containing the SystemC and CCI Visual C++ projects
has the correct name for the version of Visual C++ you are using
(see previous section).


  1. The INCLUDE, LIB, and PATH environment variables must be set in Windows
     before launching a Cygwin or MinGW shell.

     The quickest way to do this is to launch a Visual C++ command prompt
     from your Start Menu.

     Alternatively, you can open a plain command prompt and use the
     `scripts\vsvars.bat' file to setup the Visual C++ tools environment.
     With this batch file, you can specify the version and the platform to
     use via additional command-line options given to vsvars.bat, e.g.

      cd %CCI_HOME%\scripts

      vsvars.bat [arch]               # load MSVC 2005 for [arch]
      vsvars.bat [version]            # load MSVC [version] for x86
      vsvars.bat [version] [platform] # load MSVC [version] for [platform]

      vsvars.bat 2010 amd64  # load 64-bit tools for MSVC 10.0
      vsvars.bat 11.0        # load default (x86) tools for MSVC 2012
      vsvars.bat x86_amd64   # load x64 cross-tools for MSVC 2005


  2. From that command prompt, launch a shell for your Un*x shell
     compatibility layer (Cygwin or MinGW/MSYS):

     - Cygwin:
       c:\cygwin\cygwin.bat

     - MinGW/MSYS:
       c:\MinGW\msys\1.0\msys.bat

    The remaining commands are typed at the shell prompt that should
    have now appeared in a new window.

  3. Some environment variables must be set up to locate the required
     libraries:

     - SYSTEMC_HOME - location of SystemC installation directory

       - MinGW  (drop drive colon and use '/' instead of '\'):
         export SYSTEMC_HOME=/c/systemc-2.3.1

       - Cygwin (additionally, paths are prefixed with '/cygdrive'):
         export SYSTEMC_HOME=/cygdrive/c/systemc-2.3.1

     - BOOST_HOME - location of Boost installation directory

     - CCI_HOME - location of CCI directory (optional)

     Note: These environment variables can be set to the canonical Windows
           paths globally as well.

  4. Set the compiler to use Visual C++:
     (otherwise the Cygwin/MinGW GNU C++ compiler will be used by default)

       export CXX=cl

  5. Change to the main CCI folder and create a directory to
     hold the test results and cd into it, e.g.:

       cd $CCI_HOME
       mkdir run
       cd run

  6. Run the verify.pl script as described in the main scripts/README_CCI.txt,
     e.g.:

     - ../scripts/verify.pl examples    (runs all CCI examples using release library)
     - ../scripts/verify.pl -g examples (runs all CCI examples using debug library)

     Note: In order to perform source-level debugging of an example, you can
           keep the results of a previous debug run (with `-g -no-cleanup')
           and open the executable (as a project) from the Visual Studio GUI 
	   afterwards.


How to build MSVC projects for CCI examples
-------------------------------------------

There is no MSVC projects provided for each example by default. Instead, a
`cci_example' property sheet is provided for convenience. By importing
the given property sheet and updating the User Macros stated below,
you shall be able to compile and run the examples.

  0. Build the 3 PoC libraries according to the previous instructions.

  1. Create a new MSVC project
     - File -> New -> Project -> Win32 Console Application

  2. Use cci_example.props for msvc10 and newer.

  3. Update the "User Macros" according to your environment

     The following macros should have been updated correctly during the PoC
     library build (see above):
     - MSVC
     - SYSTEMC
     - BOOST

	Additionally, the $CCI_HOME environment variable (configured above) is used.

  4. Import the header and source files from a given example.

You are ready to build & run now.
