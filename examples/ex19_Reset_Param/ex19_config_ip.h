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
 *  @brief  Configure simple_ip's parameters
 *  @author Trevor Wieman, Intel
 */

#ifndef EXAMPLES_EX19_RESET_PARAM_EX19_CONFIG_IP_H_
#define EXAMPLES_EX19_RESET_PARAM_EX19_CONFIG_IP_H_

#include <cci_configuration>
#include <string>
#include "xreport.hpp"

/**
 *  @class  ex19_config_ip
 *  @brief  The config_ip configures simple_ip's parameters.
 */
SC_MODULE(ex19_config_ip) {
 public:
  /**
   *  @fn     void ex19_config_ip()
   *  @brief  The constructor for the class.
   *  @return void
   */

SC_CTOR(ex19_config_ip):
   m_broker(cci::cci_get_broker())
  {
    SC_THREAD(execute);

    XREPORT("Setting up sim_ip.paramB's preset value to 10 ");
    m_broker.set_preset_cci_value("sim_ip.paramB",
                                   cci::cci_value::from_json("10"));
  }

  /**
   *  @fn     void execute()
   *  @brief  Synchronize new preset values.
   *  @return void
   */
  void execute() {

    // Move beyond time zero then update mutable parameters (C is locked, 
    // D is immutable)
    wait(10, sc_core::SC_NS);
    XREPORT("Updating mutable parameters.");
    m_broker.get_param_handle("sim_ip.paramA").set_cci_value(cci::cci_value(100));
    m_broker.get_param_handle("sim_ip.paramB").set_cci_value(cci::cci_value(200));

    // Wait until first reset sequence is complete, then update preset values
    wait(20, sc_core::SC_NS);
    XREPORT("Updating preset values.");
    std::string params[4] = { "paramA", "paramB", "paramC", "paramD" };
    std::string param_name("sim_ip.");
    for (unsigned i = 0; i < 4; ++i)
    {
        m_broker.set_preset_cci_value(param_name + params[i],
            cci::cci_value(1000 * (i+1)));
    }
  }

 private:
  cci::cci_broker_handle m_broker;
};

#endif  // EXAMPLES_EX19_RESET_PARAM_EX19_CONFIG_IP_H_
