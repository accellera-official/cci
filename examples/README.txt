================================================================================
                    Accellera/CCI Examples README
Date  : 09/13/2016
================================================================================

ToC
---
  Introduction
  Directory Structure
  Running the examples
  Note

Introduction
------------
  This README provides an overview of the Accellera/CCI package examples and 
how to build and run them. The build is branched from systemc-regressions/ package's
verify.pl script.

Directory Structure
-------------------
        cci/                            // CCI_HOME folder
          examples/                     // Examples folder
                |- README.txt           // This README
                |- common               // Code common to one or more examples
                |       |- inc          //   Common headers
                |- ex1_xxx              // ex[Index]_[Example Name]
                |       |- doc          //   PPT having example description
                |       |- golden       //   Contains 'main.log' golden reference file
                |       |- *.h *.cpp    //   Contains example sources
                |- ...                  // More examples


Running the examples
---------------------
1) The following environment variables need to be setup based on your 
    installation. A sample file has been provided for your reference.

-- [ cci_env.bash ]
  export SYSTEMC_HOME=../systemc
  export BOOST_HOME=../boost/1.57.0/win_64/vc_12/include/boost-1_57
  export CCI_HOME=../cci
# export SYSTEMC_PTHREADS=1
  export SYSTEMC_TEST=${CCI_HOME}/examples
  export CXX=g++ (for Linux)
         CXX=cl  (for Windows)
# export PATH=/apps/free/gcc/4.4.5/bin:${PATH} (for Linux)
         PATH=/cygdrive/c/Program\ Files\ \(x86\)/Mirosoft\ Visual\ Studio\ 14.0/VC/bin/amd64:${PATH} (for Windows)
# export LD_LIBRARY_PATH=/apps/free/gcc/4.4.5/lib64:${LD_LIBRARY_PATH}
--

2) Ensure that you build the 2 CCI libs before attempting to build the examples.

3) Create a directory called run/ in your CCI_HOME folder, and 'cd' into it.
+------------------------------------------------+
  % pwd
  ../cci
  % mkdir run
  % cd run
  % pwd
  ../cci/run
+------------------------------------------------+

4) The verify.pl script needs to be invoked to build/run/compare-golden results.

  4.1) To run a specific example, say ex1_simple_int_param, do:
+------------------------------------------------+
    % ../scripts/verify.pl ex1_simple_int_param
+------------------------------------------------+

  4.2) To run all examples, do:
+-----------------------------------------------+
    % ../scripts/verify.pl .
+------------------------------------------------+
 
5) The build and run logs are placed in the run/ folder; reference logs are
   provided in the scripts/run/ folder.

6*) For CCI WG members only, dev_examples can be included in the regression testing by:
+-----------------------------------------------+
    % unsetenv SYSTEMC_TEST
    %../scripts/verify.pl dev_examples examples
+-----------------------------------------------+

Note
-------
CCI specific changes are included between #--CCI and #--/CCI in verify.pl script which is branched from 
systemc-regressions/ package

