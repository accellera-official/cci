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
 *  @brief  Configure simple_ip's int_param (simple integer) parameter
 *  @author R. Swaminathan, TI
 */

#ifndef INC_CONFIG_IP_H
#define INC_CONFIG_IP_H

//------------------------------------------------------------------------------
#include <cassert>
#include "cci.h"
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

  /// Configure the value of "sim_ip.int_param" parameter
  void execute() {
    std::ostringstream ss;
    const std::string int_param_name = "sim_ip.int_param";
    
    // Wait for a while to update param value
    wait(20, sc_core::SC_NS);

    // Check for existance of the param
    if( m_cci->exists_param(int_param_name) ) {
      // Get handle to the param
      cci::cnf::cci_base_param *int_param_ptr = m_cci->get_param(int_param_name);
      assert(int_param_ptr != NULL);

      // Update the param's value to 2
      XREPORT("execute: [EXTERNAL] Set value of "<< int_param_name<< " to 2");
      int_param_ptr->json_deserialize("2");

      // Display new value
      std::string new_value = int_param_ptr->json_serialize();
      XREPORT("execute: [EXTERNAL] Current value of "<< int_param_ptr->get_name()<< " is "<< new_value);
    }
    else {
      XREPORT_ERROR("execute: Param ("<< int_param_name<< ") is not found!");
    }
  }

 private:
  /// CCI configuration handle
  cci::cnf::cci_cnf_broker_if *m_cci;
}; // config_ip

#endif // INC_CONFIG_IP_H

