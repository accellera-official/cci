/*******************************************************************************
  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2010 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.
 ******************************************************************************/

/**
 *  @file   testbench.cpp
 *  @brief  A testbench that demonstrates immutable parameters
 *  @author Vishal Goel, TI
 */
//------------------------------------------------------------------------------ 
#include <systemc>
#include "simple_ip.h"
#include "config_ip.h"

//------------------------------------------------------------------------------ 
/// Testbench for the CCI simple immutable parameters example
int sc_main(int argc, char *argv[]) {
    config_ip cfg_ip("cfg_ip");
    simple_ip sim_ip("sim_ip");

    SC_REPORT_INFO( "sc_main", "Begin Simulation");
    sc_core::sc_start(); 
    SC_REPORT_INFO( "sc_main", "End Simulation");

    return EXIT_SUCCESS; 
}

