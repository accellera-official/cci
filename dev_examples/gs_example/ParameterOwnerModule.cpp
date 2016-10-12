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
 * @file   ParameterOwnerModule.cpp
 * @author Christian Schroeder, GreenSocs
 * @author Mark Burton, GreenSocs
 */

#include "ParameterOwnerModule.h"
#include <systemc.h>

void ParameterOwnerModule::main_action() {

  std::cout << "----------------------------" << std::endl;

  // get the config broker which is responsible for this module
  cci::cci_broker_if* mBroker = &cci::cci_broker_manager::get_current_broker(cci::cci_originator(*this));
  assert(mBroker != NULL && "get_cnf_broker_instance returned is NULL");

  // demonstrate is_default_value
  cout << name() << ": uint_param get_default_value()=" << (dec) << uint_param.get_default_value()<<endl;
  cout << name() << ": uint_param="<<(dec)<< uint_param <<std::endl;
  cout << name() << ": uint_param is_default_value()=" << uint_param.is_default_value()<<endl;
  uint_param = 12000;
  cout << name() << ": set uint_param="<<(dec)<< uint_param <<std::endl;
  cout << name() << ": uint_param is_default_value()=" << uint_param.is_default_value()<<endl;
  uint_param = 10;
  cout << name() << ": set uint_param="<<(dec)<< uint_param <<std::endl;
  cout << name() << ": uint_param is_default_value()=" << uint_param.is_default_value()<<endl<<endl;

  // demonstrate has_default_value
  cout << name() << ": uint_param get_default_value()=" << (dec) << uint_param.get_default_value()<<endl;

  cout << name() << ": bool_param get_default_value()=" << bool_param.get_default_value()<<endl;
  
  wait(1, SC_NS);

  std::cout << "----------------------------" << std::endl;

  // get json string from parameter
  std::string str = int_param.get_cci_value().to_json();
  cout << name() << ": "<<int_param.get_name() << ".get_cci_value().to_json() = "<< str <<endl;
  
  // set parameter using json string
  int_param.set_cci_value(cci::cci_value::from_json("555000"));
  str = int_param.get_cci_value().to_json();
  cout << name() << ": "<<int_param.get_name() << ".set_cci_value(cci::cci_value::from_json(\"555000\")) results in "<< str <<endl;

  // set parameter directly using operator=
  cout << name() << ": set int_param = 100" << endl;
  int_param = 100;

  // set parameter directly using set function
  cout << name() << ": set int_param.set(200)" << endl;
  int_param.set(200);

  // create and access a local parameter
  cout << name() << ": create parameter " << endl;
  cci::cci_param<unsigned char> uchar_param("uchar_param", 0);
  cout << "  created " << uchar_param.get_name() << endl;
  cout << name() << ": Set the new parameter uchar_param" << endl;
  uchar_param = 'u';
  cout << endl;

  // copy operator
  cout << name() << ": copy ";
  uint_param = uint_param2;
  
  wait(10, SC_SEC);
  
  std::cout << "----------------------------" << std::endl;

  // show a parameter list
  cout << endl << "**** Parameter list: " << endl;
  std::vector<std::string> vec = mBroker->get_param_list();
  std::vector<std::string>::iterator iter;
  std::stringstream ss_show;
  for (iter = vec.begin() ; iter < vec.end(); iter++) {
    if (iter != vec.begin()) ss_show << ", ";
    ss_show << *iter;
  }
  std::cout << "   " << ss_show.str() << std::endl<<std::endl;

}
