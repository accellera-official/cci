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
 * @file   ValueModule.cpp
 * @author Christian Schroeder, GreenSocs
 * @author Mark Burton, GreenSocs
 */

#include "ValueModule.h"
#include <systemc.h>

ValueModule::ValueModule(sc_core::sc_module_name name)
: sc_core::sc_module(name)
{ 
  // get the config broker which is responsible for this module
  mBroker = &cci::cci_broker_manager::get_current_broker(cci::cci_originator(*this));
  SC_THREAD(main_action);
}


void ValueModule::main_action() {

  wait(20, SC_NS);
  std::cout << std::endl;
  cout << "----------------------------" << endl;
  
  // get a parameter using the local config broker
  cci::cci_base_param *uint_param_ptr = mBroker->get_param("Owner.uint_param");
  if (uint_param_ptr == NULL) return;
  // make it a reference for convenience
  cci::cci_param<unsigned int> &uint_param_p = *static_cast<cci::cci_param<unsigned int>* >(uint_param_ptr);

  // demonstrate setting a parameter using a cci value
  DEMO_DUMP(name(), "Set parameter Owner.uint_param to value=555 using cci_value");
  cci::cci_value val(555);
  try {
    uint_param_p.set_value(val);
  } catch(sc_core::sc_report e) {
    switch ( cci::cci_report_handler::get_param_failure(e) ) {
      case cci::CCI_VALUE_FAILURE: 
        std::cout << std::endl << name() << ": Caught " << e.what() << std::endl;
        break;
      default:
        // If other error, throw it again
        throw e;
    }
  }
  std::cout << "uint_param has value = " << uint_param_p.get_cci_value().to_json() << endl;

  // get a parameter using the local config API
  cci::cci_base_param *str_param_ptr = mBroker->get_param("Owner.str_param");
  if (str_param_ptr == NULL) return;
  // make it a reference for convenience
  cci::cci_param<std::string> &str_param_p = *static_cast<cci::cci_param<std::string>* >(str_param_ptr);
  
  // show parameter's default value
  std::cout << std::endl << "str_param has value = " << str_param_p.get_cci_value().to_json() << endl;
  // demonstrate setting a parameter using a cci value
  DEMO_DUMP(name(), "Set parameter Owner.str_param to value=\"test cci value string\" using cci_value");
  cci::cci_value vals("test cci value string");
  str_param_p.set_value(vals);
  std::cout << "str_param has value = " << str_param_p.get_cci_value().to_json() << endl;
  
  std::cout << std::endl;
  
  
}
