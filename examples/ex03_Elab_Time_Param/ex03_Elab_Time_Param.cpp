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
 *  @brief  A testbench that demonstrates possible updates to a elaboration-time 
 *          integer parameter.
 *  @detail sim_ip:
 *          1. Instantiates a structure_param with a default value. 
 *             a. Updates are made to this param at these simulation points:
 *                - Before end of elaboration (valid)
 *                - After start of simulation (invalid)
 *          2. Creates a struc_param_post_eoe, after end of elaboration with a default value.
 *             a. Updates are made to this param at these simulation points:
 *                - After start of simulation (invalid)        
 *          cfg_ip:
 *          1. Updates parameters viz. structure_param and struc_param_post_eoe, both updations are invalid.
 *
 *  @author Udaya Ranga, TI
 */

#include "ex03_simple_ip.h"
#include "ex03_config_ip.h"

/// Testbench for the CCI elab_time_param example
int sc_main(int argc, char *argv[]) {
  ex03_simple_ip sim_ip("sim_ip");
  ex03_config_ip cfg_ip("cfg_ip");

  SC_REPORT_INFO("sc_main", "Begin Simulation.");
  sc_core::sc_start();
  SC_REPORT_INFO("sc_main", "End Simulation.");

  return EXIT_SUCCESS;
}
