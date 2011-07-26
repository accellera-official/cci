// LICENSETEXT
//
//   Copyright (C) 2009-2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//    Mark Burton, mark@greensocs.com
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#include "ValueModule.h"
#include <systemc.h>


ValueModule::ValueModule(sc_core::sc_module_name name)
: sc_core::sc_module(name)
{ 
  // get the config broker which is responsible for this module
  mBroker = &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));
  SC_THREAD(main_action);
}


void ValueModule::main_action() {

  wait(20, SC_NS);
  std::cout << std::endl;
  cout << "----------------------------" << endl;
  
  // get a parameter using the local config broker
  cci::cnf::cci_base_param *uint_param_ptr = mBroker->get_param("Owner.uint_param");
  if (uint_param_ptr == NULL) return;
  // make it a reference for convenience
  cci::cnf::cci_param<unsigned int> &uint_param_p = *static_cast<cci::cnf::cci_param<unsigned int>* >(uint_param_ptr);

  // demonstrate setting a parameter using a cci value
  DEMO_DUMP(name(), "Set parameter Owner.uint_param to value=555 using cci_value");
  //This is still TODO
  cci::cnf::cci_value val(555);
  try {
    uint_param_p.set_value(val);
  } catch(sc_core::sc_report e) {
    switch ( cci::cnf::cci_report_handler::get_param_failure(e) ) {
      case cci::cnf::CCI_VALUE_FAILURE: 
        std::cout << std::endl << name() << ": Caught " << e.what() << std::endl;
        break;
      default:
        // If other error, throw it again
        throw e;
    }
  }
  std::cout << "uint_param has value = " << uint_param_p.json_serialize() << endl;

  // get a parameter using the local config API
  cci::cnf::cci_base_param *str_param_ptr = mBroker->get_param("Owner.str_param");
  if (str_param_ptr == NULL) return;
  // make it a reference for convenience
  cci::cnf::cci_param<std::string> &str_param_p = *static_cast<cci::cnf::cci_param<std::string>* >(str_param_ptr);
  
  // show parameter's default value
  std::cout << std::endl << "str_param has value = " << str_param_p.json_serialize() << endl;
  // demonstrate setting a parameter using a cci value
  DEMO_DUMP(name(), "Set parameter Owner.str_param to value=\"test cci value string\" using cci_value");
  cci::cnf::cci_value vals("test cci value string");
  str_param_p.set_value(vals);
  std::cout << "str_param has value = " << str_param_p.json_serialize() << endl;
  
  std::cout << std::endl;
  
  
}
