/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2009 by all Contributors.
  All Rights reserved.

  Developed by GreenSocs : http://www.greensocs.com/
   Christian Schroeder, schroeder@eis.cs.tu-bs.de
   Mark Burton, mark@greensocs.com

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/


#include "ParameterOwnerModule.h"
#include <systemc.h>

void ParameterOwnerModule::main_action() {

  cci::cci_cnf_api* mApi = cci::get_cnf_api_instance(this);
  assert(mApi != NULL && "get_cnf_api_instance returned is NULL");
  
  wait(1, SC_NS);
  cout << name() << ": set int_param = 100" << endl;
  int_param = 100;
  cout << name() << ": set int_param.set(200)" << endl;
  int_param.set(200);
  cout << endl;
  wait(SC_ZERO_TIME);

  /*cout << name() << ": set str_param = 'Hello World!'" << endl;
  str_param = "Hello World!";
  cout << endl;
  wait(SC_ZERO_TIME);*/

  std::cout << "----------------------------" << std::endl;

}
