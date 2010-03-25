// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
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


#include "ModuleA.h"
#include <systemc.h>

void ModuleA::main_action() {

  // get the config API which is responsible for this module
  cci::cnf::cci_cnf_api* mApi = cci::cnf::get_cnf_api_instance(this);
  assert(mApi != NULL && "get_cnf_api_instance returned is NULL");

  
  
  wait(10, SC_SEC);
  
  // show a parameter list
  cout << endl << "**** Parameter list (in "<<name()<<"): " << endl;
  std::vector<std::string> vec = mApi->get_param_list();
  std::vector<std::string>::iterator iter;
  std::stringstream ss_show;
  for (iter = vec.begin() ; iter < vec.end(); iter++) {
    ss_show << "   " << *iter << std::endl;
  }
  std::cout << ss_show.str() << std::endl<<std::endl;

  std::cout << "----------------------------" << std::endl;

}
