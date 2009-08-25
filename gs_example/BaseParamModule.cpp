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

#include "BaseParamModule.h"
#include <systemc.h>

BaseParamModule::BaseParamModule(sc_core::sc_module_name name)
: sc_core::sc_module(name)
{ 
  mApi = cci::get_cnf_api_instance(this);
  SC_THREAD(main_action);
  
}


void BaseParamModule::main_action() {
  
  wait(100, SC_NS);
  
  cci::gs_cci_base_param my_b_par("b_par", "this is a base param value stored by the base param");
  
}
