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
 *  @brief  Configuration IP block that tries to update immutable parameters
 *  @author Vishal Goel, TI
 */

#ifndef EXAMPLES_EX04_SIMPLE_IMMUTABLE_PARAM_EX04_CONFIG_IP_H_
#define EXAMPLES_EX04_SIMPLE_IMMUTABLE_PARAM_EX04_CONFIG_IP_H_

#include <cci_configuration>
#include <cassert>
#include "xreport.hpp"

/**
 *  @class  ex04_config_ip
 *  @brief  The config_ip configures the sim_ip's parameter
 */
SC_MODULE(ex04_config_ip) {
 public:
  /**
   *  @fn     void ex04_config_ip
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex04_config_ip):
            m_cci(cci::cci_broker_manager::get_broker())
  {
    SC_THREAD(execute);

    setup_sim_ip("Attempting to setup config_ip to 100 before IP construction",
                 "sim_ip.param_1", "100");
  }

  /**
   *  @fn     void setup_sim_ip(const char* msg, const char* key, const char* val)
   *  @brief  Function to setup the simulation
   *  @param  msg A message to be printed regarding the setup
   *  @param  key The parameter key to lookup and modify
   *  @param  val The value to assign to the parameter referenced by key
   *  @return void
   */
  void setup_sim_ip(const char *msg, const char *key, const char *val) {
    XREPORT(msg);

    if (m_cci.param_exists(key)) {
      XREPORT_ERROR("Instantiate config_ip ahead of simple_ip"
                    " to demonstrate this example");
    } else {
      XREPORT("Set init-value of " << key << " to " << val);
      m_cci.set_initial_cci_value(key, cci::cci_value::from_json(val));
    }
  }

  /**
   *  @fn     void execute()
   *  @brief  Configure the value of "sim_ip.param_1" parameter
   *  @return void
   */
  void execute() {
    wait(10, sc_core::SC_NS);

    if (m_cci.param_exists("sim_ip.param_2")) {
      cci::cci_param_handle param_2 =
        m_cci.get_param_handle("sim_ip.param_2");
      if (!param_2.is_valid()) {
        XREPORT_ERROR("Unable to get handle to 'sim_ip.param_2'!");
      } else {
        try {
          XREPORT("Attempting to set value of 'sim_ip.param_2' to 200");
          param_2.set_cci_value(cci::cci_value::from_json("200"));
        } catch (std::exception &x) {
          XREPORT_WARNING(x.what());
        }
      }
      // -- If sim_ip.param_2 is missing
    } else {
      XREPORT_ERROR("Unable to find param - 'sim_ip.param_2'");
    }
  }

 private:
  cci::cci_broker_if& m_cci; ///< CCI configuration handle
};
// ex04_config_ip

#endif  // EXAMPLES_EX04_SIMPLE_IMMUTABLE_PARAM_EX04_CONFIG_IP_H_
