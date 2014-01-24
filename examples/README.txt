================================================================================
                    Accellera/CCI Examples README
Date  : 07/05/2013
================================================================================

ToC
---
  Introduction
  Directory Structure
  Running the examples
  TODO
  ISSUES

Introduction
------------
  This README provides an overview of the Accellera/CCI package examples and 
how to build and run them. The build is based on systemc-regressions/ package's
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
  export SYSTEMC_HOME=/vobs/ti_systemc/tools/systemc/systemc-2.3.0
  export BOOST_HOME=/vobs/ti_systemc/tools/boost/boost_1_44_0
  export CCI_HOME=/proj/sds_cce/cci_ti/cci
# export SYSTEMC_PTHREADS=1
  export SYSTEMC_TEST=${CCI_HOME}/examples
  export CXX=g++
# export PATH=/apps/free/gcc/4.4.5/bin:${PATH}
# export LD_LIBRARY_PATH=/apps/free/gcc/4.4.5/lib64:${LD_LIBRARY_PATH}
--

2) Ensure that you build the 3 CCI libs before attempting to build the examples.

3) Create a directory called run/ in your CCI_HOME folder, and 'cd' into it.
+------------------------------------------------+
  % pwd
  /proj/sds_cce/cci_ti/cci
  % mkdir run
  % cd run
  % pwd
  /proj/sds_cce/cci_ti/cci/run
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
    %../scripts/verify.pl -I $CCI_HOME/examples/common/inc dev_examples examples
+-----------------------------------------------+


TODO
----
1) The flow has been tested only on Linux. Needs to be tested on other hosts like 
PC, MacOSX etc.

ISSUES
-------
1) Should the scripts/ folder be part of CCI package or should this be imported
  from systemc-regressions/ package.
  1.1) If this is imported from systemc-integration package, then
    a. Everyone using cci/ package should also import systemc-regression/ package.
    b. The CCI related changes to the script must be protected with flags so as to 
       not impact systemc-regressions.
  1.2) If this not imported then,
    a. The verify.pl scripts may need to be synced up periodically for any new 
       features and/or bug-fixes.

