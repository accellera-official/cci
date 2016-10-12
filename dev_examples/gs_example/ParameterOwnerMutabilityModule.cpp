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
 * @file   ParameterOwnerMutabilityModule.cpp
 * @author Christian Schroeder, GreenSocs
 */

#include "ParameterOwnerMutabilityModule.h"
#include <systemc.h>

void ParameterOwnerMutabilityModule::main_action() {

  std::cout << "----------------------------" << std::endl;

  // get the config broker which is responsible for this module
  cci::cci_broker_if* mBroker = &cci::cci_broker_manager::get_current_broker(cci::cci_originator(*this));
  assert(mBroker != NULL && "get_cnf_broker_instance returned is NULL");

  // ************************************************************************
  // **** demonstrate mutable parameter being made temporarily immutable 
  cout << name() << ": demonstrate mutable parameter being made temporarily immutable" <<endl;
  cout << name() << ":  mutable_int_param=" << (dec) << mutable_int_param<<endl;
  cout << name() << ":  set mutable_int_param to 150"<<endl;
  mutable_int_param = 150;
  assert(mutable_int_param == 150);
  cout << name() << ":  mutable_int_param=" << (dec) << mutable_int_param<<endl;

  void* key = &mutable_int_param;
  cout << name() << ":  lock mutable_int_param"<<endl; // "with key "<< (void*)&key <<endl;
  mutable_int_param.lock(&key);
  cout << name() << ":    set mutable_int_param should fail"<<endl;
  try {
    mutable_int_param = 55555; // should fail!
  } catch(sc_core::sc_report e) {
    // If set_param_failed error, catch it
    switch ( cci::cci_report_handler::get_param_failure(e) ) {
	    case cci::CCI_SET_PARAM_FAILURE: 
        std::cout << std::endl << name() << ": Caught " << e.what() << std::endl;
        break;
      default:
        // If other error, throw it again
        throw e;
    }
  }
  assert(mutable_int_param == 150);
  cout << name() << ":  mutable_int_param=" << (dec) << mutable_int_param<<endl;
  cout << name() << ":    set mutable_int_param with wrong key"<<endl;// "<< (void*)&mBroker << " - should fail twice"<<endl;
  try {
    mutable_int_param.set(4444, &mBroker); // should fail!
  } catch(sc_core::sc_report e) {
    // If set_param_failed error, catch it
    switch ( cci::cci_report_handler::get_param_failure(e) ) {
	    case cci::CCI_SET_PARAM_FAILURE: 
        std::cout << std::endl << name() << ": Caught " << e.what() << std::endl;
        break;
      default:
        // If other error, throw it again
        throw e;
    }
  }
  try {
    mutable_int_param.set(4444, NULL); // should fail!
  } catch(sc_core::sc_report e) {
    // If set_param_failed error, catch it
    switch ( cci::cci_report_handler::get_param_failure(e) ) {
	    case cci::CCI_SET_PARAM_FAILURE: 
        std::cout << std::endl << name() << ": Caught " << e.what() << std::endl;
        break;
      default:
        // If other error, throw it again
        throw e;
    }
  }
  assert(mutable_int_param == 150);

  cout << name() << ":    set mutable_int_param with key - should succeed"<<endl;// "<< (void*)&key << " - should succeed"<<endl;
  mutable_int_param.set(4444, &key); // should succeed!
  cout << name() << ":  mutable_int_param=" << (dec) << mutable_int_param<<endl;
  assert(mutable_int_param == 4444);

  cout << name() << ":  unlock mutable_int_param"<<endl;// with key "<< (void*)&key <<endl;
  mutable_int_param.unlock(&key);
  cout << name() << ":    set mutable_int_param should succeed"<<endl;
  mutable_int_param = 66666; // should succeed!
  assert(mutable_int_param == 66666);
  
  
  // ************************************************************************
  // **** demonstrate immutable_parameter


  // ************************************************************************
  // **** demonstrate elaboration_time_parameter

}
