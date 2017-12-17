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
 *  @brief  Configure simple_ip's param_1 (simple integer) parameter's init value;
 *          and this value will override the value specified the default value
 *          specified in the IP 's constructor. Other parameter param_2 is
 *          not changed; and whtever is the default value, will be used in simulation
 *  @author Vishal Goel, Texas Instruments
 */

#ifndef EXAMPLES_EX05_DEFAULT_AND_PRESET_VALUE_EX05_CONFIG_IP_H_
#define EXAMPLES_EX05_DEFAULT_AND_PRESET_VALUE_EX05_CONFIG_IP_H_

#include <cci_configuration>
#include "xreport.hpp"

/**
 *  @class  ex05_config_ip
 *  @brief  The config_ip configures sim_ip's preset values
 */
SC_MODULE(ex05_config_ip) {
 public:
  /**
   *  @fn     ex05_config_ip
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex05_config_ip):
            m_broker(cci::cci_get_broker())
  {
    setup_sim_ip("Attempting to setup config_ip to 10 before IP construction",
                 "10");
    SC_THREAD(execute);
  }

  /**
   *  @fn     void setup_sim_ip(const char* msg, const char* val)
   *  @brief  Function to setup the simulation
   *  @param  msg Message to print during setup
   *  @param  val The value to assign to the param
   *  @return void
   */
  void setup_sim_ip(const char *msg, const char *val) {
    XREPORT(msg);

    // Check for existence of sim_ip.param_1 param
    if (m_broker.get_param_handle("sim_ip.param_1").is_valid()) {
      XREPORT_ERROR("Instantiate config_ip before simple_ip"
                    " to demonstrate set_preset_cci_value");
    } else {
      XREPORT("Setting up sim_ip.param_1's preset value to " << val);
      m_broker.set_preset_cci_value("sim_ip.param_1",
                                   cci::cci_value::from_json(val));
    }

    XREPORT("Setting up cfg_ip.param_implicit_3's preset value to 3");
    m_broker.set_preset_cci_value("cfg_ip.param_implicit_3",
                                cci::cci_value(3));
  }


  /**
   *  @fn     void execute()
   *  @brief  Configure the value of "sim_ip.param_1" parameter
   *  @return void
   */
  void execute() {
    const std::string int_param_name = "sim_ip.param_1";

    // Set Preset value after construction is NOT treated as normal value
    // update, it will update only the preset value registery.
    m_broker.set_preset_cci_value(int_param_name,cci::cci_value(5));

    // Get handle to the param
    cci::cci_param_handle int_param_handle =
        m_broker.get_param_handle(int_param_name);
    
    // Check its validity
    if (int_param_handle.is_valid()) {

      // Display new value
      std::string new_value = int_param_handle.get_cci_value().to_json();
      XREPORT("execute: [EXTERNAL] Current value of "
              << int_param_handle.get_name() << " is " << new_value);
    } else {
      XREPORT_ERROR("execute: Param (" << int_param_name<< ") is not found!");
    }
  }

 private:
  cci::cci_broker_handle m_broker; ///< CCI configuration handle
};
// ex05_config_ip

#endif  // EXAMPLES_EX05_DEFAULT_AND_PRESET_VALUE_EX05_CONFIG_IP_H_
