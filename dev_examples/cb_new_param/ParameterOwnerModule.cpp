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

  wait(1, SC_NS);

  cout << endl;
  DEMO_DUMP(name(), "Executing main_action");

  // get the config API which is responsible for this module
  cci::cci_broker_if& mApi = cci::cci_broker_manager::get_broker();
  
  // create a local parameter
  DEMO_DUMP(name(), "- Creating uchar_param parameter");
  cci::cci_param<unsigned char> uchar_param("uchar_param", 0);

  // update parameter values, activating registered callbacks
  cout << endl;
  DEMO_DUMP(name(), "- Changing uint_param value to 1");
  uint_param = 1;

  cout << endl;
  DEMO_DUMP(name(), "- Changing uint_param2 value to 2");
  uint_param2 = 2;

  // show a parameter list
  std::vector<std::string> vec = mApi.get_param_list();
  std::vector<std::string>::iterator iter;
  std::stringstream ss_show;
  ss_show << "- List of all parameters:" << endl;
  for (iter = vec.begin() ; iter < vec.end(); iter++) {
    ss_show << "\t" << *iter << endl;
  }
  cout << endl;
  DEMO_DUMP(name(), ss_show.str());
}
