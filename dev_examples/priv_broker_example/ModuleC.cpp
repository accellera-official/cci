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
 * @file   ModuleC.cpp
 * @author Christian Schroeder, GreenSocs
 * @author Mark Burton, GreenSocs
 */

#include "ModuleC.h"
#include <systemc.h>

ModuleC::ModuleC(sc_core::sc_module_name name)
: sc_core::sc_module(name)
, m_broker(cci::cci_broker_manager::get_broker())
, priv_param ("priv_param", "this is private information")
{
  SC_THREAD(main_action);
}

void ModuleC::main_action() {
  wait(10, SC_SEC);
  
  // show a parameter list
  cout << endl << "**** Parameter list (visible in "<<name()<<"): " << endl;
  std::vector<cci::cci_param_untyped_handle> vec = m_broker.get_param_handles();
  std::vector<cci::cci_param_untyped_handle>::iterator iter;
  std::stringstream ss_show;
  for (iter = vec.begin() ; iter < vec.end(); iter++) {
    ss_show << "   " << iter->get_name() << std::endl;
  }
  std::cout << ss_show.str() << std::endl<<std::endl;

  std::cout << "----------------------------" << std::endl;

}
