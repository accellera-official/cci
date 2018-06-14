--------------------------------------------------------------------------------
README for building the CCI PoC library and examples with Microsoft Visual C++

Authors: Lei Liang <lei.liang@ericsson.com>
         Philipp A. Hartmann <philipp.hartmann@offis.de>
         Sonal Poddar <sonal.poddar@intel.com>
Date   : 2018-06-13
--------------------------------------------------------------------------------

Table of Contents
-----------------
  - Introduction
  - Prerequisites
  - How to build the proof-of-concept PoC library
  - How to build and run an example


Introduction
-------------

This README gives step-by-step instructions for

  1. building the PoC library (api)

  2. building and running examples with MSVC


Prerequisites
-------------

You shall have the following software/libraries before you start:

  1. CCI POC source code        (mandatory for all operations in this guide)
  2. SystemC installation       (mandatory for all operations in this guide)
  3. Boost library installation (mandatory for all operations in this guide)
  4. Microsoft Visual Studio

And they are verified with following versions:

  3. SystemC
     - 2.3.2 (preferred with central name registry)
     - 2.3.1 (required for 64-bit support)
     - 2.3.0

  4. Boost (only headers needed)
     - boost_1_57_0
     - boost_1_44_0

  5. Microsoft Visual Studio
     - Visual Studio 2010 Professional
     - Visual Studio 2013 Express
	 - Visual Studio 2015 Professional

     Note: Please see the next section on upgrading to newer version of MSVC
           (later than 10.0/2010).

Note: The MSVS project files are configured to use DLL Runtime Libraries, in 
      alignment with the SystemC 2.3.2+ default configuration.


How to build the PoC library
----------------------------

  0. Upgrade the MSVC solution/projects, for newer MSVC versions (optional)

     The provided MSVC solution/project files are prepared for Microsoft
     Visual Studio 2010 (10.0).  You need to upgrade these files when using
     more recent versions of MSVC.

     - Copy the 'msvc10' directory to the correct SystemC architecture alias
       for your MSVC version:

       - 10.0/2010 - "msvc10" (default)
       - 11.0/2012 - "msvc11"
       - 12.0/2013 - "msvc12"
	   - 14.0/2015 - "msvc14"

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
     - Under the `cci.sln' solution, you can see the following MSVC project:
       - cciapi

  2. Update property sheet

     A property sheet (cci_lib.props) is used for setup, e.g. the SystemC and 
	 Boost dependencies.

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
         e.g. "c:\systemc-2.3.2\$(MSVC)"

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

         Note: MSVC 64-bit support has been added to SystemC in version 2.3.2.
               With older SystemC versions (e.g. 2.2.0/2.3.0), only 32-bit is
               available.

     - Build the whole cci solution (e.g. by hitting F7)

       The CCI PoC library will be created for the given configuration.


How to build and run an example
-------------------------------
Projects files are provided for each example and they are included in the CCI 
solution (cci.sln). Right click on an example project (e.g. 
ex01_Simple_Int_Param) in the Solution Explorer and select 'Set as StartUp 
Project'. Then choose Build->Build <example>, followed by Debug->Start Debugging.

Note: See examples/README.txt for instructions to run all examples and check
them against expected results, in batch mode.

