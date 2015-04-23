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
 *  @file   config_ip.h
 *  @brief  Configure simple_ip's int_param (simple integer) parameter
 *  @author R. Swaminathan, TI
 */

#ifndef EXAMPLES_EX01_SIMPLE_INT_PARAM_EX01_CONFIG_IP_H_
#define EXAMPLES_EX01_SIMPLE_INT_PARAM_EX01_CONFIG_IP_H_

#include <cci>
#include <cassert>
#include <string>
#include "xreport.hpp"

/**
 *  @class  ex01_config_ip
 *  @brief  The config_ip configures simple_ip's parameter.
 */
SC_MODULE(ex01_config_ip) {
 public:
  /**
   *  @fn     void ex01_config_ip()
   *  @brief  The constructor for the class.
   *  @return void
   */
  SC_CTOR(ex01_config_ip) {
    // Get CCI configuration handle specific for this module
    m_cci = &cci::cnf::cci_broker_manager::get_current_broker(
        cci::cnf::cci_originator(*this));
    assert(m_cci != NULL);
    SC_THREAD(execute);
  }

  /**
   *  @fn     void execute()
   *  @brief  Configure the value of "sim_ip.int_param" parameter
   *  @return void
   */
  void execute() {
    std::ostringstream ss;
    const std::string int_param_name = "sim_ip.int_param";

    // Wait for a while to update param value
    wait(20, sc_core::SC_NS);

    // Check for existance of the param
    if (m_cci->param_exists(int_param_name)) {
      // Get handle to the param
      cci::cnf::cci_base_param *int_param_ptr = m_cci->get_param(
          int_param_name);
      assert(int_param_ptr != NULL);

      // Update the param's value to 2
      XREPORT("execute: [EXTERNAL] Set value of " << int_param_name << " to 2");
      int_param_ptr->json_deserialize("2");

      // Display new value
      std::string new_value = int_param_ptr->json_serialize();
      XREPORT("execute: [EXTERNAL] Current value of "
              << int_param_ptr->get_name() << " is " << new_value);
    } else {
      XREPORT_ERROR("execute: Param (" << int_param_name<< ") is not found!");
    }
  }

 private:
  cci::cnf::cci_cnf_broker_if *m_cci; ///< CCI configuration handle
};
// ex01_config_ip

#endif  // EXAMPLES_EX01_SIMPLE_INT_PARAM_EX01_CONFIG_IP_H_
