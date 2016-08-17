/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2009-2010 GreenSocs
  All rights reserved.
  Authors:
      Christian Schroeder <schroeder@eis.cs.tu-bs.de>
      Mark Burton <mark@greensocs.com>

  Copyright 2016 Ericsson
  All rights reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 *****************************************************************************/


#include "ParamManipulateModule.h"
#include <systemc.h>


ParamManipulateModule::ParamManipulateModule(sc_core::sc_module_name name)
: sc_core::sc_module(name)
{ 
  // get the config broker which is responsible for this module
  mBroker = &cci::cci_broker_manager::get_current_broker(cci::cci_originator(*this));
  SC_THREAD(main_action);

  // demonstrate setting of an initial value
  mBroker->json_deserialize_initial_value("Owner.int_param", "10");
  // demonstrate waring issued by a second initial value
  mBroker->json_deserialize_initial_value("Owner.int_param", "11");
  // demonstrate testing for existence
  if (mBroker->param_exists("Owner.int_param"))
    cout << "Owner.int_param exists (implicit or explicit)" << endl;
  else
    SC_REPORT_WARNING(name, "ERROR: Owner.int_param NOT exists!");
  const cci::cci_originator* orig = mBroker->get_latest_write_originator("Owner.int_param");
  assert (orig && "Originator must not be NULL here!");
  if (mBroker->get_param("Owner.int_param")) {
    DEMO_DUMP(this->name(), "Write originator for EXPLICIT param Owner.int_param (from broker): " << orig->name());
  } else {
    DEMO_DUMP(this->name(), "Write originator for IMPLICIT param Owner.int_param (from broker): " << orig->name());
  }
}


void ParamManipulateModule::main_action() {

  wait(10, SC_NS);
  
  cout << "----------------------------" << endl;
  // get a parameter using the local config API
  cci::cci_base_param *int_param_ptr = mBroker->get_param("Owner.int_param");
  if (int_param_ptr == NULL) return;
  // make it a reference for convenience
  cci::cci_param<int> &int_param_p = *static_cast<cci::cci_param<int>* >(int_param_ptr);

  // get a parameter using the local config API
  cci::cci_base_param *uint_param_ptr = mBroker->get_param("Owner.uint_param");
  if (uint_param_ptr == NULL) return;
  // make it a reference for convenience
  cci::cci_param<unsigned int> &uint_param_p = *static_cast<cci::cci_param<unsigned int>* >(uint_param_ptr);
  
  // demonstrate json setting
  DEMO_DUMP(name(), "Set parameter Owner.int_param to value=5000");
  int_param_p.set_cci_value(cci::cci_value::from_json("5000"));
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
