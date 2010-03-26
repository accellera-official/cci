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


#include "ParameterOwnerModule.h"
#include <systemc.h>

void ParameterOwnerModule::main_action() {

  // get the config API which is responsible for this module
  cci::cnf::cci_cnf_broker* mApi = cci::cnf::get_cnf_broker_instance(this);
  assert(mApi != NULL && "get_cnf_broker_instance returned is NULL");

  // demonstrate is_default_value
  cout << name() << ": uint_param is_default_value()=" << uint_param.is_default_value()<<endl;
  uint_param = 10;
  cout << name() << ": uint_param is_default_value()=" << uint_param.is_default_value()<<endl<<endl;

  // demonstrate is_invalid_value
  cout << name() << ": bool_param is_invalid_value()=" << bool_param.is_invalid_value()<<endl;
  bool_param = true;
  cout << name() << ": bool_param is_invalid_value()=" << bool_param.is_invalid_value()<<endl;
  bool_param.set_invalid_value();
  cout << name() << ": bool_param is_invalid_value()=" << bool_param.is_invalid_value()<<endl<<endl;
  
  wait(1, SC_NS);

  // get json string from parameter
  std::string str = int_param.json_serialize();
  cout << name() << ": "<<int_param.get_name() << ".json_serialize() = "<< int_param.json_serialize()<<endl;
  
  // set parameter using json string
  int_param.json_deserialize("555000");
  str = int_param.json_serialize();
  cout << name() << ": "<<int_param.get_name() << ".json_deserialize(555000) results in "<< str <<endl;

  // set parameter directly using operator=
  cout << name() << ": set int_param = 100" << endl;
  int_param = 100;

  // set parameter directly using set function
  cout << name() << ": set int_param.set(200)" << endl;
  int_param.set(200);

  // create and access a local parameter
  cout << name() << ": create param ";
  cci::cnf::cci_param<unsigned char> uchar_param("uchar_param");
  cout << uchar_param.get_name() << endl;
  cout << name() << ": Set the new parameter uchar_param" << endl;
  uchar_param = 'u';
  cout << endl;

  // copy operator
  cout << name() << ": copy ";
  uint_param = uint_param2;
  
  wait(10, SC_SEC);
  
  // show a parameter list
  cout << endl << "**** Parameter list: " << endl;
  std::vector<std::string> vec = mApi->get_param_list();
  std::vector<std::string>::iterator iter;
  std::stringstream ss_show;
  for (iter = vec.begin() ; iter < vec.end(); iter++) {
    if (iter != vec.begin()) ss_show << ", ";
    ss_show << *iter;
  }
  std::cout << "   " << ss_show.str() << std::endl<<std::endl;

  std::cout << "----------------------------" << std::endl;

}
