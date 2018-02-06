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
 *  @file   config_ip.h
 *  @brief  Configure simple_ip's int_param (simple integer) parameter
 *  @author R. Swaminathan, TI
 */

#ifndef EXAMPLES_EX24_RESET_PARAM_EX24_CONFIG_IP_H_
#define EXAMPLES_EX24_RESET_PARAM_EX24_CONFIG_IP_H_

#include <cci_configuration>
#include <string>
#include "xreport.hpp"

/**
 *  @class  ex24_config_ip
 *  @brief  The config_ip configures simple_ip's parameter.
 */
SC_MODULE(ex24_config_ip) {
 public:
  /**
   *  @fn     void ex24_config_ip()
   *  @brief  The constructor for the class.
   *  @return void
   */

SC_CTOR(ex24_config_ip):
    // Get CCI configuration handle specific for this module
   m_broker(cci::cci_get_broker())
  {
    SC_THREAD(execute);

    XREPORT("Setting up sim_ip.paramB's prest value to 10 ");
    m_broker.set_preset_cci_value("sim_ip.paramB",
                                   cci::cci_value::from_json("10"));
  }

  /**
   *  @fn     void execute()
   *  @brief  Configure the value of "sim_ip.int_param" parameter
   *  @return void
   */
  void execute() {

    // Wait until first reset sequence is complete, then update preset values
    wait(15, sc_core::SC_NS);

    XREPORT("[CONFIG_IP] Updating preset values.");
    std::string params[4] = { "paramA", "paramB", "paramC", "paramD" };
    std::string param_name("sim_ip.");
    for (unsigned i = 0; i < 4; ++i)
    {
        m_broker.set_preset_cci_value(param_name + params[i],
            cci::cci_value(100 * i));
    }
  }

 private:
  cci::cci_broker_handle m_broker; ///< CCI configuration handle
};

#endif  // EXAMPLES_EX24_RESET_PARAM_EX24_CONFIG_IP_H_
