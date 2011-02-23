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


#include "ParamManipulateModule.h"
#include <systemc.h>


ParamManipulateModule::ParamManipulateModule(sc_core::sc_module_name name)
: sc_core::sc_module(name)
{ 
  // get the config broker which is responsible for this module
  mBroker = cci::cnf::get_cnf_broker_instance(cci::cnf::cci_originator(*this));
  SC_THREAD(main_action);

  // demonstrate setting of an initial value
  mBroker->set_init_value("Owner.int_param", "11");
  // demonstrate testing for existence
  if (mBroker->exists_param("Owner.int_param"))
    cout << "Owner.int_param exists (implicit or explicit)" << endl;
  else
    SC_REPORT_WARNING(name, "ERROR: Owner.int_param NOT exists!");
}


void ParamManipulateModule::main_action() {

  wait(10, SC_NS);
  
  cout << "----------------------------" << endl;
  // get a parameter using the local config API
  cci::cnf::cci_base_param *int_param_ptr = mBroker->get_param("Owner.int_param");
  if (int_param_ptr == NULL) return;
  // make it a reference for convenience
  cci::cnf::cci_param<int> &int_param_p = *static_cast<cci::cnf::cci_param<int>* >(int_param_ptr);

  // get a parameter using the local config API
  cci::cnf::cci_base_param *uint_param_ptr = mBroker->get_param("Owner.uint_param");
  if (uint_param_ptr == NULL) return;
  // make it a reference for convenience
  cci::cnf::cci_param<unsigned int> &uint_param_p = *static_cast<cci::cnf::cci_param<unsigned int>* >(uint_param_ptr);
  
  // demonstrate json setting
  DEMO_DUMP(name(), "Set parameter Owner.int_param to value=5000");
  int_param_p.json_deserialize("5000");
  cout << endl;
  wait(SC_ZERO_TIME);
  
  // demonstrate value setting
  DEMO_DUMP(name(), "Set parameter Owner.int_param to value=5001");
  int_param_p.set(5001);
  cout << endl;
  wait(SC_ZERO_TIME);
  DEMO_DUMP(name(), "Set parameter Owner.int_param to value=5002");
  int_param_p = 5002;
  cout << endl;
  wait(SC_ZERO_TIME);
  DEMO_DUMP(name(), "Set parameter Owner.uint_param to value=9000");
  uint_param_p = 9000;
  cout << endl;
  wait(SC_ZERO_TIME);
  
}
