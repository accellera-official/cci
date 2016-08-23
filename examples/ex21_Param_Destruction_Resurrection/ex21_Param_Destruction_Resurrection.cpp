/*****************************************************************************
  Copyright 2006-2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2016 Ericsson
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
 *  @file   ex21_Param_Destruction_Resurrection.cpp
 *  @brief  An example that demonstrates the usage and effect of parameter
 *          destruction and resurrection
 */

#include "ex21_simple_ip.h"
#include "ex21_config_ip.h"

int sc_main(int argc, char *argv[]) {
    ex21_simple_ip sim_ip("sim_ip");
    ex21_config_ip cfg_ip("cfg_ip");

    SC_REPORT_INFO("sc_main", "Begin Simulation.");
    sc_core::sc_start();
    SC_REPORT_INFO("sc_main", "End Simulation.");

    return EXIT_SUCCESS;
}
