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
 *  @brief  Configuration IP block that tries to update immutable parameters
 *  @author Vishal Goel, TI
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
  SC_HAS_PROCESS(config_ip);

  /// Ctor
 config_ip(sc_core::sc_module_name a_name)
   : sc_core::sc_module(a_name)
    {
      // Get CCI configuration handle specific for this module
      m_cci = &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));
      assert(m_cci != NULL);
      SC_THREAD(execute);

      setup_sim_ip("Attempting to setup config_ip to 100 before IP construction", "sim_ip.param_1", "100");
    }

  void setup_sim_ip(const char *msg, const char *key, const char *val) {
    XREPORT(msg);
    
    if( m_cci->exists_param(key) ) {
      XREPORT_ERROR("Instantiate config_ip ahead of simple_ip to demonstrate this example");
    }
    else {
      XREPORT("Set init-value of "<< key<< " to "<< val);
      m_cci->set_init_value(key, val);
    }
  }

  /// Configure the value of "sim_ip.param_1" parameter
  void execute() {
    wait(10, sc_core::SC_NS);

    if( m_cci->exists_param("sim_ip.param_2") ) {
      cci::cnf::cci_base_param *param_2_ptr = m_cci->get_param("sim_ip.param_2");
      if(param_2_ptr == NULL) {
        XREPORT_ERROR("Unable to get handle to 'sim_ip.param_2_ptr'!");
      }
      else {
	try {
	  XREPORT("Attempting to set value of 'sim_ip.param_2' to 200");
	  param_2_ptr->json_deserialize("200");
	}
	catch(std::exception &x) {
	  XREPORT_WARNING(x.what());
	}
      }
    }
    //-- If sim_ip.param_2 is missing
    else {
      XREPORT_ERROR("Unable to find param - 'sim_ip.param_2'");
    }
  }

 private:
  /// CCI configuration handle
  cci::cnf::cci_cnf_broker_if *m_cci;
}; // config_ip

#endif // INC_CONFIG_IP_H

