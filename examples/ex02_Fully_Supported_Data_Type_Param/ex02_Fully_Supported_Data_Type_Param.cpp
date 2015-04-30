/*****************************************************************************
  Copyright 2006-2014 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010-2015 Texas Instruments Inc.
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
 *  @file   testbench.cpp
 *  @brief  
 *  @author A. Nizamudheen, TI
 */

#include "ex02_simple_ip.h"
#include "ex02_config_ip.h"

/**
 *  @fn     int sc_main(int argc, char* argv[])
 *  @brief  Testbench for the CCI ex2_fully_supported_data_type_param example
 *  @param  argc  An integer for the number of input arguments
 *  @param  argv  A a list of the input arguments
 *  @return an integer representing whether the execution was successful
 */
int sc_main(int argc, char *argv[]) {
  ex02_simple_ip sim_ip("sim_ip");
  ex02_config_ip cfg_ip("cfg_ip");

  SC_REPORT_INFO("sc_main", "Begin Simulation.");
  sc_core::sc_start();
  SC_REPORT_INFO("sc_main", "End Simulation.");

  return EXIT_SUCCESS;
}
