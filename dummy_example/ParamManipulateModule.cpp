//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
//   This program is free software.
// 
//   If you have no applicable agreement with GreenSocs Ltd, this software
//   is licensed to you, and you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   If you have a applicable agreement with GreenSocs Ltd, the terms of that
//   agreement prevail.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//   02110-1301  USA 
// 
// ENDLICENSETEXT

#include "ParamManipulateModule.h"
#include <systemc.h>

ParamManipulateModule::ParamManipulateModule(sc_core::sc_module_name name)
: sc_core::sc_module(name)
{ 
  mApi = cci::get_cnf_api_instance(this);
  SC_THREAD(main_action);
  
  mApi->set_init_value("Owner.int_param", "11");
  if (mApi->exists_param("Owner.int_param"))
    cout << "Owner.int_param esists (implicit or explicit)" << endl;
  else
    SC_REPORT_WARNING(name, "ERROR: Owner.int_param NOT esists!");
}

void ParamManipulateModule::main_action() {

  wait(10, SC_NS);
  
  cci::cci_param_base *ptr = mApi->get_param("Owner.int_param");
  if (ptr == NULL) return;
  cci::gs_cci_param<int> &p = *static_cast<cci::gs_cci_param<int>* >(ptr);
  //cci::gs_cci_param<std::string> &pu = *static_cast<cci::gs_cci_param<std::string>* >(mApi->get_param("Owner.str_param"));
  
  DEMO_DUMP(name(), "Set parameter Owner.int_param to value=5000");
  p.set_string("5000");
  cout << endl;
  wait(SC_ZERO_TIME);
  
  DEMO_DUMP(name(), "Set parameter Owner.int_param to value=5001");
  p.set(5001);
  cout << endl;
  wait(SC_ZERO_TIME);
  
  DEMO_DUMP(name(), "Set parameter Owner.int_param to value=5002");
  p = 5002;
  cout << endl;
  wait(SC_ZERO_TIME);
  
  /*DEMO_DUMP(name(), "Set parameter Owner.str_param to value=\"SHOW THIS\"");
  pu = "SHOW THIS";
  cout << endl;
  wait(SC_ZERO_TIME);*/

}

