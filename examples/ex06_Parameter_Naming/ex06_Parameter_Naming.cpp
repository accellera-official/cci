/*******************************************************************************
  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2013 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.1 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.
******************************************************************************/

/**
 *  @file   ex06_Parameter_Naming.cpp
 *  @brief  A test that demonstrates how to configure param names to sit
 *          in different hierarchies
 *  @author Asif Mondal, TI
 */

//------------------------------------------------------------------------------ 
#include <systemc>
#include "ex06_simple_ip.h"
#include "ex06_config_ip.h"

//------------------------------------------------------------------------------ 
/// Testbench for the CCI Parameter Naming example
int sc_main(int argc, char *argv[]) {
  std::ostringstream sout;
  cci::cnf::cci_param<int> int_param("int_param", 10);
  cci::cnf::cci_param<int> int_param_top("int_param_top", 10);
  ex06_simple_ip sim_ip("sim_ip");
  ex06_config_ip cfg_ip("cfg_ip");

  sout<< "sc_main: "<< "Default value of "<< int_param.get_name()<< " is "<< int_param;
  SC_REPORT_INFO("sc_main", sout.str().c_str());
  sout.str("");

  sout<< "sc_main: "<< "Default value of "<< int_param_top.get_name()<< " is "<< int_param_top;
  SC_REPORT_INFO("sc_main", sout.str().c_str());
  sout.str("");

  sout<< "sc_main: "<< "int_param = 100";
  SC_REPORT_INFO("sc_main", sout.str().c_str());
  sout.str("");
  int_param = 100;
  sout<< "sc_main: "<< "Current value of "<< int_param.get_name()<< " is "<< int_param;
  SC_REPORT_INFO("sc_main", sout.str().c_str());
  sout.str("");

  sout<< "sc_main: "<< "int_param_top = 100";
  SC_REPORT_INFO("sc_main", sout.str().c_str());
  sout.str("");
  int_param_top = 100;
  sout<< "sc_main: "<< "Current value of "<< int_param_top.get_name()<< " is "<< int_param_top;
  SC_REPORT_INFO("sc_main", sout.str().c_str());
  sout.str("");

  SC_REPORT_INFO( "sc_main", "Begin Simulation.");
  sc_core::sc_start(); 

  sout<< "sc_main: "<< "Current value of "<< int_param.get_name()<< " is "<< int_param;
  SC_REPORT_INFO("sc_main", sout.str().c_str());
  sout.str("");

  sout<< "sc_main: "<< "Current value of "<< int_param_top.get_name()<< " is "<< int_param_top;
  SC_REPORT_INFO("sc_main", sout.str().c_str());
  sout.str("");

  SC_REPORT_INFO( "sc_main", "End Simulation.");

  return EXIT_SUCCESS; 
}

