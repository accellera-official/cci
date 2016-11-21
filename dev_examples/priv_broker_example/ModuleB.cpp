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
 * @file   ModuleB.cpp
 * @author Christian Schroeder, GreenSocs
 * @author Mark Burton, GreenSocs
 */

#include "ModuleB.h"
#include <systemc.h>

ModuleB::ModuleB(sc_core::sc_module_name name)
: sc_core::sc_module(name)
, int_param ("int_param", 50)
, uint_param("uint_param", 12000)
, uint_param2("uint_param2", 12)
, str_param ("str_param", "This is a test string.")
, bool_param("bool_param", false)
, m_broker(cci::cci_broker_manager::get_broker())
, mSubB("SubModuleB")
{
  SC_THREAD(main_action);
}

void ModuleB::main_action() {

  wait(10, SC_SEC);
  
  // show a parameter list
  cout << endl << "**** Parameter list (visible in "<<name()<<"): " << endl;
  std::vector<std::string> vec = m_broker.get_param_list();
  std::vector<std::string>::iterator iter;
  std::stringstream ss_show;
  for (iter = vec.begin() ; iter < vec.end(); iter++) {
    ss_show << "   " << *iter << std::endl;
  }
  std::cout << ss_show.str() << std::endl<<std::endl;
  
  std::cout << "----------------------------" << std::endl;

}

SubModuleB::SubModuleB(sc_core::sc_module_name name) {
  m_priv_broker = new cci::gs_cci_private_broker_handle(*this,
                                                        boost::assign::list_of(
                                                                "int_param"));
  cci::cci_broker_manager::register_broker(m_priv_broker);
  m_module_c = new ModuleC("ModuleC");
}

SubModuleB::~SubModuleB() {
  delete m_module_c;
  delete m_priv_broker;
}