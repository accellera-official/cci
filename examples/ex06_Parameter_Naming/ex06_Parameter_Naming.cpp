/*****************************************************************************
  Copyright 2006-2014 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010-2015 Texas Instruments Inc.
  All rights reserved.

  Copyright 2015 Intel Corporation
  All rights reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 *****************************************************************************/

/**
 *  @file   ex06_Parameter_Naming.cpp
 *  @brief  A test that demonstrates how to configure param names to sit
 *          in different hierarchies
 *  @author Asif Mondal, TI
 */

#include "ex06_simple_ip.h"
#include "ex06_config_ip.h"

/**
 *  @fn     int sc_main(int argc, char *argv[])
 *  @brief  The testbench for the CCI parameter naming example
 *  @param  argc  The number of input arguments
 *  @param  argv  The list of the input arguments
 *  @return An integer representing the execution status of the program
 */
int sc_main(int argc, char *argv[]) {
  ex06_simple_ip sim_ip("sim_ip");
  ex06_config_ip cfg_ip("cfg_ip");

  SC_REPORT_INFO("sc_main", "Begin Simulation.");
  sc_core::sc_start();
  SC_REPORT_INFO("sc_main", "End Simulation.");

  return EXIT_SUCCESS;
}
