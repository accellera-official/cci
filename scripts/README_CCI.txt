--------------------------------------------------------------------------------
                  README for CCI-Examples Build/Run

Author: R. Swaminathan (swaminathan@ti.com)
Date  :  09/14/2013 
--------------------------------------------------------------------------------

Table of Contents
-----------------
  - Introduction
  - How to build/run the CCI examples?
  - Testing
  - Changes to verify.pl(1) script

Introduction
-------------
  This script is derived from verify.pl(1)* script present in 
OSCI/systemc_regressions/scripts folder of SystemC Regressions Git Repo. The
script has been updated to accomodate minor changes to build/run CCI examples.

(*) verify.pl(1) refers to original verify.pl script in systemc-regressions
area

How to build/run the CCI examples?
----------------------------------
Step 0. Build the required SystemC/CCI libraries

  * SystemC lib
    - libsystemc.a

  * CCI libs
    - libcciapi.a
    - libcciparamimpl.a
    - libccibrokerimpl.a

Step 1. Setup the following environment variables -
SYSTEMC_HOME, BOOST_HOME, CCI_HOME, SYSTEMC_TEST, CXX

The following is an example of the environment variables to be setup for the
bash shell.

-- [ source.bash ]
export  SYSTEMC_HOME=/vobs/ti_systemc/tools/systemc/systemc-2.3.0
export    BOOST_HOME=/vobs/ti_systemc/tools/boost/boost_1_44_0
export      CCI_HOME=/proj/sds_cce/cci_ti/cci
export  SYSTEMC_TEST=${CCI_HOME}/examples
export           CXX=g++
#-- Update the following, if need be
# export SYSTEMC_PTHREADS=1
# export PATH=/apps/free/gcc/4.4.5/bin:${PATH}
# export LD_LIBRARY_PATH=/apps/free/gcc/4.4.5/lib64:${LD_LIBRARY_PATH}
--

Step 2. Create a temporary folder to build/run the examples and switch to it.

$ cd cci/scripts
$ mkdir run
$ cd run

Step 3. Build/Run the examples.

[To build/run all the examples]
$ ../verify.pl .

[To build/run a specific example]
$ ../verify.pl ex1_simple_int_param

[Do not clean up temporary files and directories (in current dir)]
$ ../verify.pl -no-cleanup ex1_simple_int_param

[FOR CCI WG MEMBERS: TO ADD DEV_EXAMPLES TO THE REGRESSION]
$ unset SYSTEMC_TEST
$ ../verify.pl -I $CCI_HOME/examples/common/inc dev_examples examples
$ ../verify.pl -I $CCI_HOME/examples/common/inc dev_examples/priv_broker_example

Testing
-------
* The script has been verified on Linux-RHEL4/gcc-4.4/64-bit platform
* [NOTE] Could not get the verify.pl(1) script to work on Windows platform with
Cygwin installed and hence skipped testing on this.

Changes to verify.pl(1) script
------------------------------
1. verify.pl(1) was auto-appending tests/ path to the environment variable.
Due to different hierarchy in cci/examples, this feature has been disabled.

  - SYSTEMC_TEST must include the trailing tests/ directory, if changes are
  merged back.

2. Bug?  verify.pl(1) was not stripping the header line - 'SystemC Simulation' 
in output log. This is fixed. 

  - Existing systemc-regression golden logs may need to strip this extra line 
  at beginning, if these changes are merged back.

3. New Environment Variables have been added.
    o CCI_HOME: CCI home directory path
    o BOOST_HOME: Boost home directory path

#-- Taf!

