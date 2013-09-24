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
 *  @brief  Try to get sim_ip.int_param handle and typecast it to differnt
 *          data-types
 *  @author A. Nizamudheen, TI
 */

#ifndef INC_CONFIG_IP_H
#define INC_CONFIG_IP_H

//------------------------------------------------------------------------------
#include <cassert>
#include "cci"
#include "xreport.hpp"

//------------------------------------------------------------------------------
/// The my_sc_query configures sim_ip's parameter
SC_MODULE(config_ip) {
 public:
  /// Ctor
  SC_CTOR(config_ip)
  {
    // Get CCI configuration handle
    m_cci = &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));
    assert(m_cci != NULL);
    SC_THREAD(execute);
  }

  void execute() {
    std::ostringstream ss;
    const std::string param_name = "sim_ip.int_param";
    
    // Wait for 10 ns to update the values of the param
    wait(10, sc_core::SC_NS);

    cci::cnf::cci_base_param *base_int_param_ptr = NULL;
    // Check for existance of int_param
    if( m_cci->param_exists(param_name) ) {
      // Get handle to the param
      base_int_param_ptr = m_cci->get_param(param_name);
      assert(base_int_param_ptr != NULL);

      /* @todo get_basic_type() needs to be implemented
	cci::cnf::basic_param_type partype = cci::cnf::partype_not_available;
	partype = base_int_param_ptr->get_basic_type();
	if(partype == cci::cnf::partype_number) {
      */
      // Typecast the param to an 'int' type
      cci::cnf::cci_param<int> *int_param_ptr = dynamic_cast<cci::cnf::cci_param<int>*>(base_int_param_ptr);
      if(int_param_ptr == NULL) {
	XREPORT_WARNING("@execute: Typecast of "<< int_param_ptr->get_name()<< " to 'cci::cnf::cci_param<int> *' type failed.");
      }
      else {
	cci::cnf::cci_param<int> &int_param_ref = *int_param_ptr;
	XREPORT("@execute: Typecast of "<< param_name<< " to 'cci::cnf::cci_param<int> *' succeeded");
	XREPORT("@execute: Current value of "<< int_param_ref.get_name()<< " is "<< int_param_ref.get());
      }

      // Typecast the param to 'unsigned int' type
      cci::cnf::cci_param<unsigned int> *uint_param_ptr = dynamic_cast<cci::cnf::cci_param<unsigned int>*>(base_int_param_ptr);
      if(uint_param_ptr == NULL) {
	XREPORT_WARNING("@execute: Typecast of "<< param_name<< " to 'cci::cnf::cci_param<unsigned int> *' type failed.");
      }
      else {
	XREPORT("@execute: Current value of "<< uint_param_ptr->get_name()<< " is "<< uint_param_ptr->get());
      }

      // Typecast the param to 'std::string' type
      cci::cnf::cci_param<std::string> *string_param_ptr = dynamic_cast<cci::cnf::cci_param<std::string>*>(base_int_param_ptr);
      if(string_param_ptr == NULL) {
	XREPORT_WARNING("@execute: Typecast of "<< param_name<< " to 'cci::cnf::cci_param<std::string> *' type failed.");
      }
      else {
	XREPORT("@execute: Current value of "<< string_param_ptr->get_name()<< " is "<< string_param_ptr->get());
      }
      /*
	}
	else {
        XREPORT_WARNING("@execute: "<< base_int_param_ptr->get_name()<< " returned a non-partype_number type ("<< partype<< ")");
	}
      */

    }
    else {
      XREPORT_ERROR("execute: Param ("<< param_name<< ") is not found!");
    }

  }

 private:
  /// CCI configuration handle
  cci::cnf::cci_cnf_broker_if *m_cci;
}; // config_ip

#endif // INC_CONFIG_IP_H

