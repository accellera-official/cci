#-------------------------------------------------------------------------------  
# \file   source.bash
# \brief  Update the following environment variables for your installation
#-------------------------------------------------------------------------------  
export SYSTEMC_HOME=/vobs/ti_systemc/tools/systemc/systemc-2.3.0
export TLM_HOME="${SYSTEMC_HOME}/src"
export BOOST_HOME=/vobs/ti_systemc/tools/boost/boost_1_44_0
export CCI_HOME=/proj/sds_cce/cci_ti/cci
export SYSTEMC_TEST=${CCI_HOME}/examples
export CXX=g++
#-- Update following, if needed
# export SYSTEMC_PTHREADS=1
# export PATH=/apps/free/gcc/4.4.5/bin:${PATH}
# export LD_LIBRARY_PATH=/apps/free/gcc/4.4.5/lib64:${LD_LIBRARY_PATH}

