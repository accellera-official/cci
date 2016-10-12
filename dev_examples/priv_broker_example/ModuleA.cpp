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
 * @file   ModuleA.cpp
 * @author Christian Schroeder, GreenSocs
 * @author Mark Burton, GreenSocs
 */

#include "ModuleA.h"
#include <systemc.h>

// within this contructor the private broker stack is valid containing the priv_broker broker as top
ModuleA::ModuleA(sc_core::sc_module_name name, cci::cci_broker_manager priv_broker)
: sc_core::sc_module(name)
//, cci::cci_broker_manager(new cci::gs_cci_private_broker_accessor(*this, boost::assign::list_of("int_param"), cci::cci_originator(*this)))
, m_broker(priv_broker)
, int_param ("int_param", 50, *m_broker )
, uint_param("uint_param", 12000, *m_broker )
, uint_param2("uint_param2", 12, *m_broker )
, str_param ("str_param", "This is a test string.", *m_broker)
, bool_param("bool_param", false, *m_broker)
, m_modB("ModuleB")
{ 
  SC_THREAD(main_action);
}

ModuleA::~ModuleA() {
}

void ModuleA::main_action() {

  // Note: Do NOT use cci_broker_manager::get_current_broker here, it won't return the private broker!
  
  
  wait(10, SC_SEC);
  
  // show a parameter list
  cout << endl << "**** Parameter list (visible in "<<name()<<"): " << endl;
  std::vector<std::string> vec = m_broker->get_param_list();
  std::vector<std::string>::iterator iter;
  std::stringstream ss_show;
  for (iter = vec.begin() ; iter < vec.end(); iter++) {
    ss_show << "   " << *iter << std::endl;
  }
  std::cout << ss_show.str() << std::endl<<std::endl;

  std::cout << "----------------------------" << std::endl;

}

