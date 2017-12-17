================================================================================
                    Accellera/CCI Examples README
Date  : 12/12/2017
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

See Compile_and_Run_CCI_Example.ppt (in the same directory) with reference 
instructions for interactive/debug build of the examples on both Linux and Windows
(the instruction herein are for batch execution).

Directory Structure
-------------------
        cci/                            // CCI_HOME folder
          examples/                     // Examples folder
                |- README.txt           // This README
				|- Compile_and_Run_CCI_Example.ppt
				                        // Presentation summarizing build and run steps
                |- common               // Code common to one or more examples
                |       |- inc/         // Common headers
                |- ex01_xxx             // ex[Index]_[Example Name]
                |       |- doc/         // Contains a presentation explaining the example 
                |       |- golden/      // Contains 'ex01_xxx.log' golden reference log file
                |       |- *.h *.cpp    // Example source code
                |- ...                  // More examples


Running the examples
---------------------
1) The following environment variables need to be setup based on your 
    installation. A sample file has been provided for your reference.

-- [ cci_env.bash ]
  export SYSTEMC_HOME=.../systemc
  export BOOST_HOME=.../boost/1.57.0/win_64/vc_12/include/boost-1_57
  export CCI_HOME=.../cci
# export SYSTEMC_PTHREADS=1
  export SYSTEMC_TEST=${CCI_HOME}/examples
  export CXX=g++ (for Linux)
         CXX=cl  (for Windows)
# export PATH=/apps/free/gcc/4.4.5/bin:${PATH} (for Linux)
         PATH=/cygdrive/c/Program\ Files\ \(x86\)/Mirosoft\ Visual\ Studio\ 14.0/VC/bin/amd64:${PATH} (for Windows)
# export LD_LIBRARY_PATH=/apps/free/gcc/4.4.5/lib64:${LD_LIBRARY_PATH}
--

2) Ensure that you build the CCI lib before attempting to build the examples; see the top-level 
   README.txt file for instructions.

3) Create a directory called run/ in your CCI_HOME folder, and 'cd' into it.
+------------------------------------------------+
  % pwd
  .../cci
  % mkdir run
  % cd run
  % pwd
  .../cci/run
+------------------------------------------------+

4) The verify.pl script needs to be invoked to build/run/compare-golden results.

  4.1) To run a specific example, say ex1_simple_int_param, do:
+------------------------------------------------+
    % ../scripts/verify.pl ex01_Simple_Int_Param
+------------------------------------------------+

  4.2) To run all examples, do:
+-----------------------------------------------+
    % ../scripts/verify.pl examples
+------------------------------------------------+

  4.3) To see available invocation options, do:
+-----------------------------------------------+
    % ../scripts/verify.pl -help
+------------------------------------------------+

5) The results are summarized and, for the case of running all examples, can be
   compared against scripts/results/summary.txt. The build and run logs are
   placed in the run/ folder though only problem logs are kept unless the
   -no-cleanup invocation argument is specified.

Note
----
CCI specific changes to verify.pl are included between #--CCI and #--/CCI in
verify.pl script which is branched from systemc-regressions/ package.

