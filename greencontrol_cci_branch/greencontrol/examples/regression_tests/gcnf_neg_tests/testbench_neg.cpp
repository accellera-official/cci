//   GreenConfig framework
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

// GreenControl
#include "greencontrol/config.h"    // Core


//#define TEST_CORE_SINGLETON_ERROR
#define TEST_CNF_PLUGIN_SINGLETON_ERROR

/// negative regression test to test error message if automatic plugin creation conflicts with manual instantiation
int sc_main(int argc, char *argv[]) {

  std::cout << "***** THIS TEST IS PASSED IF THERE ARE ERRORS!!! ***** (error depending on #defines)" << std::endl;
  
  //sc_core::sc_report_handler::set_actions(sc_core::SC_ERROR, sc_core::SC_ABORT);
  //sc_core::sc_report_handler::set_actions(sc_core::SC_WARNING, sc_core::SC_ABORT);
  //sc_core::sc_report_handler::set_actions(sc_core::SC_FATAL, sc_core::SC_STOP);

  // This will automatically create a Core and a Config Plugin
  gs::gs_param<int> my_param("my_param");
  
  
  // ////////////////////////////////////////////////////
  // GreenControl Core
#ifdef TEST_CORE_SINGLETON_ERROR
  gs::ctr::GC_Core       core;
#endif
  //  or
  gs::ctr::GC_Core::get_instance(); // optional, done automatically by gc_ports
  //  or
  //nothing

  // ////////////////////////////////////////////////////
  // GreenConfig Service Plugin
  //gs::cnf::ConfigDatabase cnfdatabase("ConfigDatabase");
  //gs::cnf::ConfigPlugin configPlugin(&cnfdatabase);
  //  or
#ifdef TEST_CNF_PLUGIN_SINGLETON_ERROR
  gs::cnf::ConfigPlugin configPlugin;
#endif
  //  or
  gs::cnf::ConfigPlugin::get_instance();
  //  or
  //nothing
  

  sc_core::sc_start();

  return EXIT_SUCCESS; 
  
}
