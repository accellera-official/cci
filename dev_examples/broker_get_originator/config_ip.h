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
 * @file   config_ip.h
 * @brief  Configure simple_ip's parameter and get the originator which shall
           be this module itself
 * @author Lei Liang, Ericsson
 */
#ifndef BROKER_GET_ORIGINATOR_CONFIG_IP_H_
#define BROKER_GET_ORIGINATOR_CONFIG_IP_H_

#include <systemc>
#include "cci_configuration"

/**
 *  @class  config_ip
 *  @brief  The config_ip class, which set sim_ip's
 *          parameter and then get the originator which
 *          shall be itself
 */
class config_ip: public sc_core::sc_module {
public:
    SC_HAS_PROCESS(config_ip);
    /**
     *  @fn     void config_ip
     *  @brief  The constructor for the class
     *  @return void
     */
    config_ip(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      m_cci(cci::cci_broker_manager::get_broker())
    {
      SC_THREAD(execute);
    }

    /**
     *  @fn     void execute()
     *  @brief  The main function which update set param value and get originator
     *  @return void
     */
    void execute() {
      std::ostringstream ss;
      const std::string int_param_name = "sim_ip.int_param";

      cci::cci_originator orig = m_cci.get_latest_write_originator(int_param_name);

      std::cout<<"execute: [EXTERNAL] Write originator for param sim_ip.int_param (from broker): "
               << orig.name() << std::endl;

      // Wait for a while to update param value
      wait(20, sc_core::SC_NS);

      // Check for existance of the param
      if (m_cci.param_exists(int_param_name)) {
        // Get handle to the param
        cci::cci_param_handle int_param = m_cci.get_param_handle(int_param_name);
        assert(int_param.is_valid());

        // Update the param's value to 2
        std::cout<<"execute: [EXTERNAL] Set value of "
                 << int_param_name << " to 2" << std::endl;
        int_param.set_cci_value(cci::cci_value::from_json("2"));

      } else {
          std::cout<<"execute: Param ("
                   << int_param_name<< ") is not found!"<< std::endl;
      }

      orig = m_cci.get_latest_write_originator(int_param_name);

      std::cout<<"execute: [EXTERNAL] Write originator for param sim_ip.int_param (from broker): "
               << orig.name() << std::endl;
    }

private:
    cci::cci_broker_if& m_cci; ///< CCI configuration handle
};


#endif //EXAMPLES_EX23_HIERARCHICAL_VALUE_UPDATE_EX23_CONFIG_IP_H_
