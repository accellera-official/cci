// LICENSETEXT
//
//   Copyright (C) 2009-2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//    Mark Burton, mark@greensocs.com
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


#include "ValueModule.h"
#include <systemc.h>


ValueModule::ValueModule(sc_core::sc_module_name name)
: sc_core::sc_module(name)
{ 
  // get the config API which is responsible for this module
  mApi = cci::cnf::get_cnf_api_instance(this);
  SC_THREAD(main_action);
}


void ValueModule::main_action() {

  wait(20, SC_NS);
  std::cout << std::endl;
  cout << "----------------------------" << endl;
  
  // get a parameter using the local config API
  cci::cnf::cci_base_param *uint_param_ptr = mApi->get_param("Owner.uint_param");
  if (uint_param_ptr == NULL) return;
  // make it a reference for convenience
  cci::cnf::cci_param<unsigned int> &uint_param_p = *static_cast<cci::cnf::cci_param<unsigned int>* >(uint_param_ptr);

  // demonstrate setting a parameter using a cci value
  DEMO_DUMP(name(), "Set parameter Owner.uint_param to value=555 using cci_value");
  cci::cnf::cci_value val(555);
  uint_param_p.set_value(val);
  std::cout << "uint_param has value = " << uint_param_p.json_serialize() << endl;

  // get a parameter using the local config API
  cci::cnf::cci_base_param *str_param_ptr = mApi->get_param("Owner.str_param");
  if (str_param_ptr == NULL) return;
  // make it a reference for convenience
  cci::cnf::cci_param<std::string> &str_param_p = *static_cast<cci::cnf::cci_param<std::string>* >(str_param_ptr);
  
  // show parameter's default value
  std::cout << std::endl << "str_param has value = " << str_param_p.json_serialize() << endl;
  // demonstrate setting a parameter using a cci value
  DEMO_DUMP(name(), "Set parameter Owner.str_param to value=\"test cci value string\" using cci_value");
  cci::cnf::cci_value vals("test cci value string");
  str_param_p.set_value(vals);
  std::cout << "str_param has value = " << str_param_p.json_serialize() << endl;
  
  cout << "----------------------------" << endl;
  std::cout << std::endl;
  
  
}
