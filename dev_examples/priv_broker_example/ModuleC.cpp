/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010 GreenSocs
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


#include "ModuleC.h"
#include <systemc.h>


ModuleC::ModuleC(sc_core::sc_module_name name, cci::cci_broker_manager priv_broker)
: sc_core::sc_module(name)
//, cci::cci_broker_manager(new cci::gs_cci_private_broker_accessor(*this, boost::assign::list_of("int_param"), cci::cci_originator(*this)))
, m_broker(priv_broker)
, priv_param ("priv_param", "this is private information", *m_broker)
{ 
  SC_THREAD(main_action);
}

ModuleC::~ModuleC() {
  // TODO: delete private broker that was newed during construction!
  // Don't delete while params existing!
  /*cci::cci_broker_if* pb = get_broker();
   register_private_broker(NULL);
   delete pb;*/
}

void ModuleC::main_action() {

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
