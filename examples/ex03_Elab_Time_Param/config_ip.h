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
 *  @brief  Configure simple_ip's structure_param (simple integer) parameter
 *  @author Udaya Ranga, TI
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

  /// Configure the value of "sim_ip.struc_param" parameter
  void execute() {
    std::ostringstream ss;
    const std::string struc_param_name = "sim_ip.struc_param";
    
    // Wait for a while to update param value
    wait(20, sc_core::SC_NS);

    // Check for existance of the structure_param
    if( m_cci->param_exists(struc_param_name) ) {
      // Get handle to the param
      cci::cnf::cci_base_param *struc_param_ptr = m_cci->get_param(struc_param_name);
      assert(struc_param_ptr != NULL);

      // Update the structure_param value to 3 (invalid)
      try {
        XREPORT("execute: [EXTERNAL] Set value of "<< struc_param_name<< " to 3");
        struc_param_ptr->json_deserialize("3");
      }catch(std::exception &x) {
        XREPORT_WARNING(x.what());
      }

      // Display new value
      std::string new_value = struc_param_ptr->json_serialize();
      //XREPORT("execute: [EXTERNAL] Current value of "<< struc_param_ptr->get_name()<< " is "<< new_value);
      if( "1" == new_value )  {
        XREPORT("execute: [EXTERNAL] Value of " << struc_param_ptr->get_name() << " remains unchanged "<< new_value); 
      }
      else if( "3" == new_value ) {
        XREPORT_WARNING("execute: [EXTERNAL] Value of " << struc_param_ptr->get_name() << " changed to: "<< new_value); 
      }
      else    {
        XREPORT_ERROR("execute: [EXTERNAL] Invalid update to " << struc_param_ptr->get_name() << " value changed to: "<< new_value); 
        assert( 0 );
      }
    }
    else {
      XREPORT_ERROR("execute: Param ("<< struc_param_name<< ") is not found!");
    }

  }

 private:
  /// CCI configuration handle
  cci::cnf::cci_cnf_broker_if *m_cci;
}; // config_ip

#endif // INC_CONFIG_IP_H

