--------------------------------------------------------------------------------
                  README for CCI-Examples Build/Run

Author: R. Swaminathan (swaminathan@ti.com)
        Sonal Poddar (sonal.poddar@intel.com)
Date  :  09/13/2016 
--------------------------------------------------------------------------------

Table of Contents
-----------------
  - Introduction
  - How to build/run the CCI examples?
  - Testing
  - Note

Introduction
-------------
  This script is branched from verify.pl script present in 
OSCI/systemc_regressions/scripts folder of SystemC Regressions Git Repo. The
script has been updated to accomodate minor changes to build/run CCI examples.

CCI specific changes are included between #--CCI and #--/CCI in verify.pl script.

How to build/run the CCI examples?
----------------------------------
Step 0. Build the required SystemC/CCI libraries

  * SystemC lib
    - libsystemc.a

  * CCI libs
    - libcciapi.a
    - libccibrokerimpl.a

Step 1. Setup the following environment variables -
SYSTEMC_HOME, BOOST_HOME, CCI_HOME, SYSTEMC_TEST, CXX

The following is an example of the environment variables to be setup for the
bash shell.

-- [ source.bash ]
export  SYSTEMC_HOME=.../systemc-2.3.2
export    BOOST_HOME=.../boost/1.57.0/win_64/vc_12/include/boost-1_57
export      CCI_HOME=.../cci
export  SYSTEMC_TEST=${CCI_HOME}/examples
export           CXX=g++ (for Linux)
                 CXX=cl (for Windows)
#-- Update the following, if need be
# export SYSTEMC_PTHREADS=1
# export PATH=/apps/free/gcc/4.4.5/bin:${PATH} (for Linux)
         PATH=/cygdrive/c/Program\ Files\ \(x86\)/Mirosoft\ Visual\ Studio\ 14.0/VC/bin/amd64:${PATH} (for Windows)
# export LD_LIBRARY_PATH=/apps/free/gcc/4.4.5/lib64:${LD_LIBRARY_PATH}
--

Step 2. Create a temporary folder to build/run the examples and switch to it.

$ cd cci/scripts
$ mkdir run
$ cd run

Step 3. Build/Run the examples.

[To build/run all the examples]
$ ../verify.pl examples

[To build/run a specific example]
$ ../verify.pl ex01_Simple_Int_Param

[Do not clean up temporary files and directories (in current dir)]
$ ../verify.pl -no-cleanup ex01_Simple_Int_Param

Testing
-------
The script has been verified on Linux-RHEL4/gcc-4.4/64-bit platform and on Windows platform with Cygwin installed

Note
-------
CCI specific changes are included between #--CCI and #--/CCI in verify.pl script which is branched from 
systemc-regressions/ package

#-- Taf!

