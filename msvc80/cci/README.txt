--------------------------------------------------------------------------------
README for building CCI libraries/examples/regression with MSVC

Author: Lei Liang (lei.liang@ericsson.com)
Date  :  11/26/2014 
--------------------------------------------------------------------------------

Table of Contents
-----------------
  - Introduction
  - Prerequisite
  - How to build the 3 POC libraries
  - How to run regression tests
  - How to build examples
  
Introduction
-------------
  This README gives the step-by-step operation for 
  1.build all 3 POC libraries
    - api
    - gs_broker_implementation
    - gs_param_implementation
  
  2.run existing regression on windows environment 
    - Cygwin + MSVC 
 
  3.build and run the existing examples with msvc
  
Prerequisite
-------------
  You shall have the following softwares/libraries before you start 
  1. SystemC installation       (mandatory for all operations in this guide)
  2. Cygwin installation        (mandatory for regression tests in this guide)
  3. Boost library installation (mandatory for all operations in this guide)

  And they are verified with following versions:

  1. SystemC
     - 2.3.1
     Note: You shall have the msvc08 directory named to msvc10 if you are using 
	       Visual Studio 2010 version
     
  2. Cygwin
     - CYGWIN_NT-6.1
     Note: Depends on your Cygwin installation, you may need to install perl and
	   remove the link.exe 
	   
  3. Boost
     - boost_1_57_0
     Note: You must have static libraries built
  
  4. Microsoft visual studio
     - visual studio 2010 professional
	 
	 Note: This MSVC10 is created based on visual studio 2010 professional which means
	 older version like Visual Studio 2005 is not compatible. And for visual studio 2010
	 express, the provided solution cannot be used directly due to miss x64 support in
	 visual studio 2010 express version.
	 
How to build the 3 POC libraries
-------------	 
  1. Open the prepared MSVC solution 
     - File -> Open -> Project/Solution
	 - From your CCI home -> msvc10 -> cci -> cci.sln
     - Under the cci.sln solution, you can see three MSVC project:
	 - cciapi
	 - ccibrokerimpl
	 - cciparamimpl
	   
  2. Update property sheet
     - All three projects share the same property sheets 
       - cci_lib_msvc_ps_debug   	 (for debug build with win32)
       - cci_lib_msvc_ps_release 	 (for release build with win32)
	   - cci_lib_msvc_ps_debug_x64	 (for debug build with x64)
	   - cci_lib_msvc_ps_release_x64 (for release build with x64)
     - Update the property sheet for debug build (same for x64 if needed) 
       - Open cci_lib_msvc_ps_debug -> User Macros
       - Update the two Macros according to your environment, default is
       - SYSTEMC_HOME c:\systemc-2.3.0
       - BOOST_HOME c:\boost_1_57_0
	 
     - Update the property sheet for release build (same for x64 if needed) 
       - Do the same operation as you made for debug build
	 
       Note: if your SystemC is not built according to the given MSVC project in
	     the release, you may need to change the library path and so on.
  
  3. Build the Solution
     - Choose debug/release win32/x64
     - Build the whole cci solution (by which are three libraries will be created)

How to run regression tests
-------------	 	 
  0. You must have the 3 POC libraries built accordining to previous instructions
  1. Update the scripts/cygwincfg.sh 
     Note: All following environment setting is based on bash shell inside cygwin
     - SYSTEMC_HOME
     - BOOST_HOME
     - CCI_HOME
  2. Open visual studio command prompt
  3. Enter the cygwin installation directory at the opened prompt
  4. Enter the command
     $ Cygwin.bat
  5. source the cygwincfg.sh updated in step 1
  6. create the "run" directory under CCI_HOME and enter into the directory
  7. use the same command as you run regression test under linux, like 
     - ../scripts/verify.pl -g .

How to build examples
-------------
  There is no exiting MSVC project provided for each example. Instead, the property sheet
is provided. By importing the given property sheet and updating the User Macros stated below,
you shall be able to comple and run the examples.

  0. You must have the 3 POC libraries built accordining to previous instructions
  1. create a new MSVC project
     - File -> New -> Project -> Win32 Console Application
  2. import proper sheet
     Import following property sheets under msvc10/cci/ directory
     - cci_example_msvc_ps_debug       (for win32 debug build)
     - cci_example_msvc_ps_release     (for win32 release build)
     - cci_example_msvc_ps_debug_x64   (for x64 debug build)
     - cci_example_msvc_ps_release_x64 (for x64 release build)
  3. Update the "User Macros" according to your environment which are
     - CCI_HOME
     - BOOST_HOME
     - SYSTEMC_HOME
  4. Import the header and source files from given examples
	
You are ready to build & run now
