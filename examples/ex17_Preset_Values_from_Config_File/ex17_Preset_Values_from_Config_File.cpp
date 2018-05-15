/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 ****************************************************************************/

/**
 *  @file      main.cpp
 *  @brief     This file reads the inputs (parameter name and parameter value pair)
 *             from the 'Configuration Text File' and assigns the preset values to
 *             the corresponding parameters before the construction of the modules begins
 *  @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *             Girish Verma, CircuitSutra Technologies   <girish@circuitsutra.com>
 *  @date      12th July, 2011 (Tuesday)
 */

#include <cci_configuration>
#include <systemc.h>
#include <string>

#include "ex17_cci_configFile_Tool.h"
#include "ex17_parameter_owner.h"
#include "ex17_parameter_configurator.h"
#include <cci_utils/broker.h>

/**
 *  @fn     int sc_main(int sc_argc, char* sc_argv[])
 *  @brief  This function illustrates the way of reading parameters name-value
 *          pair from an external configuration file and then, assigns the
 *          respective preset values to the parameters via the GreenSoCs parser
 *  @param  sc_argc The number of input arguments
 *  @param  sc_argv The list of the input arguments
 *  @return An integer representing the execution exit status of the function.
 */
int sc_main(int sc_argc, char* sc_argv[]) {
  cci::cci_register_broker(new cci_utils::broker("My Global Broker"));
  // Declare an instance of the cci_configFile_Tool that reads in the preset
  // values to be set to a cci-parameter via a configuration file, parses them
  // and sets them to the appropriate parameters
  cci::ex17_cci_configFile_Tool configTool("configFileTool");
  configTool.config("./ex17_Preset_Values_from_Config_File/Configuration_File.txt");

  // Declare instances of the owner and configurator classes
  ex17_parameter_owner param_owner("param_owner");
  ex17_parameter_configurator param_cfgr("param_setter");

  // Start and run the simulation till the time specified
  SC_REPORT_INFO("sc_main", "Begin Simulation.");
  sc_core::sc_start(50.0, sc_core::SC_NS);
  SC_REPORT_INFO("sc_main", "End Simulation.");

  return EXIT_SUCCESS;
}
// sc_main
