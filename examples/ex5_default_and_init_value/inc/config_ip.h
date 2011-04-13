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
 *  @brief  Configure simple_ip's param_1 (simple integer) parameter's init value;
 *          and this value will override the value specified the default value
 *          specified in the IP 's constructor. Other parameter param_2 is
 *          not changed; and whtever is the default value, will be used in simulation
 *  @author Vishal Goel, Texas Instruments
 */

#ifndef INC_CONFIG_IP_H
#define INC_CONFIG_IP_H

//------------------------------------------------------------------------------
#include <cassert>
#include "cci.h"
#include "xreport.hpp"

//------------------------------------------------------------------------------
/// The config_ip configures sim_ip's init-values
SC_MODULE(config_ip) {
 public:

  /// Ctor
  SC_CTOR(config_ip)
  {
    // Get CCI configuration handle specific for this module
    m_cci = &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));
    assert(m_cci != NULL);
    setup_sim_ip("Attempting to setup config_ip to 10 before IP construction", "10");
  }

  void setup_sim_ip(const char *msg, const char *val){
    XREPORT(msg);

    // Check for existance of sim_ip.param_1 param
    if( m_cci->exists_param("sim_ip.param_1") ) {
      XREPORT_ERROR("Instantiate config_ip before simple_ip to demonstrate set_init_value");
    }
    else {
      XREPORT("Setting up sim_ip.param_1's init-value to "<< val);
      m_cci->set_init_value("sim_ip.param_1", val);
    }
  }

 private:
  /// CCI configuration handle
  cci::cnf::cci_cnf_broker_if *m_cci;
}; // config_ip

#endif // INC_CONFIG_IP_H

