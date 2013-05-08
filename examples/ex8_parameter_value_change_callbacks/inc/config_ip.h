/*******************************************************************************
  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2010 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.
******************************************************************************/

/**
 *  @file   config_ip.h
 *  @brief  Configure simple_ip's param_1 and param_2 values
 *  @author Sheshadri Chakravarthy, Texas Instruments
 */

#ifndef INC_CONFIG_IP_H
#define INC_CONFIG_IP_H

//------------------------------------------------------------------------------
#include <sstream>
#include <cassert>
#include "cci"
#include "xreport.hpp"

//------------------------------------------------------------------------------
/// The config_ip configures sim_ip's parameter
SC_MODULE(config_ip) {
 public:

  /// Ctor
  SC_CTOR(config_ip) {
    // Get CCI configuration handle specific for this module
    m_cci = &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));
    assert(m_cci != NULL);
  }

  void end_of_elaboration()  {
    std::ostringstream ss;
    const std::string param_1_name = "sim_ip.param_1";
    const std::string param_2_name = "sim_ip.param_2";

    // Check for existance of the param_1
    if( m_cci->param_exists(param_1_name) ) {
      // Get handle to the param
      cci::cnf::cci_base_param *param_1_ptr = m_cci->get_param(param_1_name);
      assert(param_1_ptr != NULL);

      // Update the param's value
      XREPORT("config_ip::end_of_elaboration: [EXTERNAL] Set value of "<< param_1_name<< " to 10");
      param_1_ptr->json_deserialize("10");

      // Display new value
      std::string new_value = param_1_ptr->json_serialize();
      XREPORT("config_ip::end_of_elaboration: [EXTERNAL] Current value of "<< param_1_ptr->get_name()<< " is "<< new_value);
    }
    else {
      XREPORT_ERROR("end_of_elaboration: Param ("<< param_1_name<< ") is not found!");
    }

    // Check for existence of param_2
    if( m_cci->param_exists(param_2_name) ) {
      // Get handle to the param
      cci::cnf::cci_base_param *param_2_ptr = m_cci->get_param(param_2_name);
      assert(param_2_ptr != NULL);

      // Update the param's value
      XREPORT("config_ip::end_of_elaboration: [EXTERNAL] Set value of "<< param_2_name<< " to 20");
      try {
        param_2_ptr->json_deserialize("20"); /// @note This should be rejected
      } 
      catch(sc_core::sc_report const &e) {
        // Catch exception, if value-change is rejected and handle it
        if (cci::cnf::CCI_SET_PARAM_FAILURE == cci::cnf::cci_report_handler::get_param_failure(e)) {
          XREPORT_WARNING("Caught exception: " << e.what());
        }
        else {
          // Unidentified exeption. Rethrow
          throw e;
        }
      }

      // Display new value
      std::string new_value = param_2_ptr->json_serialize();
      XREPORT("config_ip::end_of_elaboration: [EXTERNAL] Current value of "<< param_2_ptr->get_name()<< " is "<< new_value);
    }
    else {
      XREPORT_ERROR("config_ip::end_of_elaboration: Param ("<< param_2_name<< ") is not found!");
    }
  }

 private:
  /// CCI configuration handle
  cci::cnf::cci_cnf_broker_if *m_cci;
}; // config_ip

#endif // INC_CONFIG_IP_H

