/*******************************************************************************
  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2013 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.1 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.
******************************************************************************/

/**
 *  @file   config_ip.h
 *  @brief  Get handles to sim_ip/sc_main's params and update them
 *  @author Asif Mondal, TI
 */

#ifndef INC_CONFIG_IP_H
#define INC_CONFIG_IP_H

//------------------------------------------------------------------------------
#include <cassert>
#include "cci"
#include "xreport.hpp"

//------------------------------------------------------------------------------
/// The config_ip configures sim_ip's parameter
SC_MODULE(config_ip) {
 public:

  /// Ctor
  SC_CTOR(config_ip)
  {
    // Get CCI configuration handle specific for this module
    m_cci = &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));
    assert(m_cci != NULL);
    SC_THREAD(execute);
  }

  /// Update the value of params
  void execute() {
    std::ostringstream ss;
    cci::cnf::cci_base_param *int_param_ptr = NULL;
    const std::string sim_ip_int_param_ip_name = "sim_ip.int_param_ip";
    const std::string sc_main_int_param_top_name = "int_param_top";
    const std::string int_param_custom_name = "top.sub.int_param_custom";
    
    // Wait for a while to update param value
    wait(20, sc_core::SC_NS);

    // Check for existance of the param
    if( m_cci->param_exists(sim_ip_int_param_ip_name) ) {
      // Get handle to the param
      int_param_ptr = m_cci->get_param(sim_ip_int_param_ip_name);
      assert(int_param_ptr != NULL);

      // Update the param's value
      XREPORT("execute: [EXTERNAL] Current value of "<< sim_ip_int_param_ip_name<< " is "<< int_param_ptr->json_serialize());
      XREPORT("execute: [EXTERNAL] Set value of "<< sim_ip_int_param_ip_name<< " to 50");
      int_param_ptr->json_deserialize("50");

      // Display new value
      std::string new_value = int_param_ptr->json_serialize();
      XREPORT("execute: [EXTERNAL] Current value of "<< int_param_ptr->get_name()<< " is "<< new_value);
    }
    else {
      XREPORT_ERROR("execute: Param ("<< sim_ip_int_param_ip_name<< ") is not found!");
    }

    // Check for existance of the param
    if( m_cci->param_exists(sc_main_int_param_top_name) ) {
      // Get handle to the param
      int_param_ptr = m_cci->get_param(sc_main_int_param_top_name);
      assert(int_param_ptr != NULL);

      // Update the param's value
      XREPORT("execute: [EXTERNAL] Current value of "<< sc_main_int_param_top_name<< " is "<< int_param_ptr->json_serialize());
      XREPORT("execute: [EXTERNAL] Set value of "<< sc_main_int_param_top_name<< " to 50");
      int_param_ptr->json_deserialize("50");

      // Display new value
      std::string new_value = int_param_ptr->json_serialize();
      XREPORT("execute: [EXTERNAL] Current value of "<< int_param_ptr->get_name()<< " is "<< new_value);
    }
    else {
      XREPORT_ERROR("execute: Param ("<< sc_main_int_param_top_name<< ") is not found!");
    }

    // Check for existance of the param
    if( m_cci->param_exists(int_param_custom_name) ) {
      // Get handle to the param
      int_param_ptr = m_cci->get_param(int_param_custom_name);
      assert(int_param_ptr != NULL);

      // Update the param's value
      XREPORT("execute: [EXTERNAL] Current value of "<< int_param_custom_name<< " is "<< int_param_ptr->json_serialize());
      XREPORT("execute: [EXTERNAL] Set value of "<< int_param_custom_name<< " to 50");
      int_param_ptr->json_deserialize("50");

      // Display new value
      std::string new_value = int_param_ptr->json_serialize();
      XREPORT("execute: [EXTERNAL] Current value of "<< int_param_ptr->get_name()<< " is "<< new_value);
    }
    else {
      XREPORT_ERROR("execute: Param ("<< int_param_custom_name<< ") is not found!");
    }
  }

 private:
  /// CCI configuration handle
  cci::cnf::cci_cnf_broker_if *m_cci;
}; // config_ip

#endif // INC_CONFIG_IP_H

