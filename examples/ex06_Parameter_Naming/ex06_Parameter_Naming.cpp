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
 *  @file   ex06_Parameter_Naming.cpp
 *  @brief  A test that demonstrates how to configure param names to sit
 *          in different hierarchies
 *  @author Asif Mondal, TI
 */

#include "ex06_simple_ip.h"
#include "ex06_config_ip.h"
#include <cci_utils/broker.h>

#define REPORT_INFO(msg_stream) do { \
  std::ostringstream ss; \
  ss<< msg_stream; \
  SC_REPORT_INFO("sc_main", ss.str().c_str()); \
} while (0)

class Tool {
public:
Tool(): external_param("tool.external_param", 10,
                       cci::cci_get_global_broker(cci::cci_originator("tool")),
                       "External parameter",
                       cci::CCI_RELATIVE_NAME, cci::cci_originator("tool"))
    {}

    const std::string get_external_param_originator_name() {
        return external_param.get_originator().name();
    }

private:
    cci::cci_param<int> external_param;
};

/**
 *  @fn     int sc_main(int argc, char *argv[])
 *  @brief  The testbench for the CCI parameter naming example
 *  @param  argc  The number of input arguments
 *  @param  argv  The list of the input arguments
 *  @return An integer representing the execution status of the program
 */
int sc_main(int argc, char *argv[]) {
  cci::cci_register_broker(new cci_utils::broker("My Global Broker"));
  Tool tool;
  ex06_simple_ip sim_ip("sim_ip");
  ex06_config_ip cfg_ip("cfg_ip");
  cci::cci_originator me=cci::cci_originator("sc_main");

  cci::cci_param<std::string> sc_main_param("sc_main_param", "value",
                                            "sc_main parameter",
                                            cci::CCI_ABSOLUTE_NAME,
                                            me
                                            );

  SC_REPORT_INFO("sc_main", "Begin Simulation.");
  sc_core::sc_start();
  SC_REPORT_INFO("sc_main", "End Simulation.");

  cci::cci_broker_handle sc_main_broker =
          cci::cci_get_global_broker(me);

  std::cout << std::endl << "List of parameters:" << std::endl;
  std::vector<cci::cci_param_untyped_handle> vec =
          sc_main_broker.get_param_handles();
  std::vector<cci::cci_param_untyped_handle>::iterator iter;
  std::stringstream ss_show;
  for (iter = vec.begin() ; iter < vec.end(); iter++) {
      std::cout << "   " << iter->get_name() << std::endl;
  }

  cci::cci_param_handle param_handle = sc_main_broker.get_param_handle(
          "tool.external_param");
  REPORT_INFO("Current value of tool.external_param is "
                      << param_handle.get_cci_value().to_json());
  REPORT_INFO("Originator of tool.external_param handle is "
                      << param_handle.get_originator().name());
  REPORT_INFO("Originator of tool.external_param is "
                      << tool.get_external_param_originator_name());

  param_handle = sc_main_broker.get_param_handle(
          "sc_main_param");
  REPORT_INFO("Current value of sc_main_param is "
                      << param_handle.get_cci_value().to_json());
  REPORT_INFO("Originator of sc_main_param handle is "
                      << param_handle.get_originator().name());
  REPORT_INFO("Originator of sc_main_param is "
                      << sc_main_param.get_originator().name());

  return EXIT_SUCCESS;
}
