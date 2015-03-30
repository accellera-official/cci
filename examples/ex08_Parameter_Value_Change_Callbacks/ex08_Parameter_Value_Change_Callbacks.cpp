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
 *  @brief  A testbench that demonstrates Requesting and responding to value change notifications 
 *          for an integer parameter
 *  @author Sheshadri Chakravarthy, Texas Instruments
 */

#include "ex08_simple_ip.h"
#include "ex08_config_ip.h"

/**
 *  @fn     int sc_main(int argc, char* argv[])
 *  @brief  The testbench for the CCI simple_int_param example
 *  @param  argc  The number of input arguments
 *  @param  argv  The list of input arguments
 *  @return An interger representing the execution status
 */
int sc_main(int argc, char *argv[]) {
  ex08_config_ip cfg_ip("cfg_ip");
  ex08_simple_ip sim_ip("sim_ip");

  SC_REPORT_INFO("sc_main", "Begin Simulation.");
  sc_core::sc_start();
  SC_REPORT_INFO("sc_main", "End Simulation.");

  return EXIT_SUCCESS;
}
