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

#define GS_PARAM_CALLBACK_VERBOSE

// GreenConfig
#include "greencontrol/config.h" 

#include "OwningModule.h"
#include "TestModule.h"

/*
 GreenConfig callback tests
 
 - register
 - pre_read cb
 - post_read cb
 - pre_write cb
 - post_write cb
 - destroy_param cb
 - legacy post_write_and_destroy cb
 - test all registered callbacks
 
 - unregister all registered callbacks
 - test all UNregistered callbacks
 
 
 - all tests for different gs_param types (as representant of types):
 - integer 
 - sc_event
 - sc_time
 - sc_bigint<64>
 - sc_attribute
 - gs_param_array 
 - with member callbacks switched on
 - with member callbacks switched off
*/

/// Testbench for the GreenConfig regresson test
int sc_main(int argc, char *argv[]) {
  
  /// GreenControl Core instance
  gs::ctr::GC_Core       core("ControlCore");
  gs::cnf::ConfigDatabase cnfdatabase("ConfigDatabase");
  gs::cnf::ConfigPlugin configPlugin("ConfigPlugin", &cnfdatabase);

  OwningModule om("OM");
  TestModule tm("TM");
  
  sc_core::sc_start();
  
  std::cout << "END SIMULATION" << std::endl;
  std::cout << "Parameter list:" << std::endl;
  gs::ctr::show_vector<std::string>(gs::cnf::GCnf_Api::getApiInstance(NULL)->getParamList());
    
  return EXIT_SUCCESS; 
  
}
