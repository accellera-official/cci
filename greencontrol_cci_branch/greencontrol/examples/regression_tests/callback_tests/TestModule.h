//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
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

// GreenConfig
#include "greencontrol/config.h" 

class TestModule
: public sc_core::sc_module
{
  
  GC_HAS_CALLBACKS();
  
public:
  SC_HAS_PROCESS(TestModule);
  TestModule(sc_core::sc_module_name name) 
  : sc_core::sc_module(name) 
  {
    m_cnf_api = gs::cnf::GCnf_Api::getApiInstance(this);
    SC_THREAD(main_action);
  }
  
  ~TestModule() {
    GC_UNREGISTER_CALLBACKS();
  }
  
  void main_action() {
    GC_REGISTER_TYPED_PARAM_CALLBACK(m_cnf_api->getPar("OM.int_param"), gs::cnf::pre_read,   TestModule, callback_function);
    GC_REGISTER_TYPED_PARAM_CALLBACK(m_cnf_api->getPar("OM.int_param"), gs::cnf::post_read,  TestModule, callback_function);
    GC_REGISTER_TYPED_PARAM_CALLBACK(m_cnf_api->getPar("OM.int_param"), gs::cnf::pre_write,  TestModule, callback_function);
    GC_REGISTER_TYPED_PARAM_CALLBACK(m_cnf_api->getPar("OM.int_param"), gs::cnf::post_write, TestModule, callback_function);
    
    m_cnf_api->getPar("OM.int_param")->setString("50");
  }
  
  gs::cnf::callback_return_type callback_function(gs::gs_param_base& changed_param, gs::cnf::callback_type reason) {
    std::cout << " callback function got: "<< callback_type_to_string(reason) <<" callback for param "<<changed_param.getName() << std::endl; 
    std::string val = changed_param.getString();
    if (reason == gs::cnf::pre_write) std::cout << "   callback function: (old value: "<<val<<")" << std::endl;
    if (reason == gs::cnf::post_write)  std::cout << "   callback function: (new value: "<<val<<")" << std::endl;
  }
  
  gs::cnf::cnf_api *m_cnf_api;
  
};
