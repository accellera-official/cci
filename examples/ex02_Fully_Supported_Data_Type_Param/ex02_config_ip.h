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
 *  @brief  Try to get sim_ip.int_param handle and typecast it to differnt
 *          data-types
 *  @author A. Nizamudheen, TI
 */

#ifndef EXAMPLES_EX02_FULLY_SUPPORTED_DATA_TYPE_PARAM_EX02_CONFIG_IP_H_
#define EXAMPLES_EX02_FULLY_SUPPORTED_DATA_TYPE_PARAM_EX02_CONFIG_IP_H_

#include <cci_configuration>
#include <cassert>
#include <string>
#include "xreport.hpp"

/**
 *  @class  ex02_config_ip
 *  @brief  The config_ip class, which configures sim_ip's parameter
 */
SC_MODULE(ex02_config_ip) {
 public:
  /**
   *  @fn     void ex02_config_ip
   *  @brief  The constructor for the class
   *  @return void
   */
  SC_CTOR(ex02_config_ip) {
    // Get CCI configuration handle
    m_cci = &cci::cci_broker_manager::get_current_broker(
        cci::cci_originator(*this));
    assert(m_cci != NULL);
    SC_THREAD(execute);
  }

  /**
   *  @fn     void execute()
   *  @brief  The main function which updates the parameter of sim_ip
   *  @return void
   */
  void execute() {
    std::ostringstream ss;
    const std::string param_name = "sim_ip.int_param";

    // Wait for 10 ns to update the values of the param
    wait(10, sc_core::SC_NS);

    cci::cci_param_handle *base_int_param_ptr = NULL;
    // Check for existance of int_param
    if (m_cci->param_exists(param_name)) {
      // Get handle to the param
      base_int_param_ptr = m_cci->get_param_handle(param_name);
      assert(base_int_param_ptr != NULL);

      /* @todo get_basic_type() needs to be implemented
       cci::basic_param_type partype = cci::partype_not_available;
       partype = base_int_param_ptr->get_basic_type();
       if(partype == cci::partype_number) {
       */
      // Typecast the param to an 'int' type
      cci::cci_param_typed_handle<int> *int_param_ptr =
          dynamic_cast<cci::cci_param_typed_handle<int>*>(base_int_param_ptr);
      if (int_param_ptr == NULL) {
        XREPORT_WARNING("@execute: Typecast of " << int_param_ptr->get_name()
                        << " to 'cci::cci_param<int> *' type failed.");
      } else {
        cci::cci_param_typed_handle<int> &int_param_ref = *int_param_ptr;
        XREPORT("@execute: Typecast of " << param_name
                << " to 'cci::cci_param<int> *' succeeded");
        XREPORT("@execute: Current value of " << int_param_ref.get_name()
                << " is " << int_param_ref.get_value());
      }

      // Typecast the param to 'unsigned int' type
      cci::cci_param_typed_handle<unsigned int> *uint_param_ptr =
          dynamic_cast<cci::cci_param_typed_handle<unsigned int>*>(base_int_param_ptr);
      if (uint_param_ptr == NULL) {
        XREPORT_WARNING("@execute: Typecast of " << param_name
                        << " to 'cci::cci_param<unsigned int> *'"
                        " type failed.");
      } else {
        XREPORT("@execute: Current value of " << uint_param_ptr->get_name()
                << " is " << uint_param_ptr->get_value());
      }

      // Typecast the param to 'std::string' type
      cci::cci_param_typed_handle<std::string> *string_param_ptr =
          dynamic_cast<cci::cci_param_typed_handle<std::string>*>(base_int_param_ptr);
      if (string_param_ptr == NULL) {
        XREPORT_WARNING("@execute: Typecast of " << param_name
                        << " to 'cci::cci_param<std::string> *'"
                        " type failed.");
      } else {
        XREPORT("@execute: Current value of " << string_param_ptr->get_name()
                << " is " << string_param_ptr->get_value());
      }
      /*
       }
       else {
       XREPORT_WARNING("@execute: "<< base_int_param_ptr->get_name()<< " returned a non-partype_number type ("<< partype<< ")");
       }
       */

    } else {
      XREPORT_ERROR("execute: Param (" << param_name << ") is not found!");
    }
  }

 private:
  cci::cci_broker_if *m_cci; ///< CCI configuration handle
};
// ex02_config_ip

#endif  // EXAMPLES_EX02_FULLY_SUPPORTED_DATA_TYPE_PARAM_EX02_CONFIG_IP_H_
