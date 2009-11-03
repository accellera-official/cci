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

// SystemC library
#include <boost/tokenizer.hpp> // Because of buggy boost integrated in SystemC 2.1
#include <systemc>

#include "globals.h"

// GreenConfig
#include "greencontrol/config.h" 

#ifdef USE_SCML_DATABASE_ADAPTER
  #include "greencontrol/gcnf/plugin/scml_database_adaptor.h"
#endif

#include "scml_test_ip.h"
#include "scml_orig_ip.h"

/// Testbench for the scml example
/**
 * The testbench instantiates the GreenControl Core and the needed user modules.
 *
 * You need to make sure that the scml.h included by scml_orig_ip.h
 * is available: 
 * Include either the CoWare scml or the GreenSocs scml file.
 * The CoWare scml.h file is part of the free downloadable CoWare package scml 1.1.
 */
int sc_main(int argc, char *argv[]) {
  
  /// GreenControl Core instance
  gs::ctr::GC_Core       core("ControlCore");

#ifdef USE_SCML_DATABASE_ADAPTER
  cout << "Use database 'Scml_database_adaptor'"<<endl;
  gs::cnf::Scml_database_adaptor cnfdatabase("ScmlDatabaseAdaptor");
#else
  std::cout << "Use standart database 'ConfigDatabase'"<<std::endl;
  gs::cnf::ConfigDatabase cnfdatabase("ConfigDatabase");
#endif
  gs::cnf::ConfigPlugin configPlugin("ConfigPlugin", &cnfdatabase);
 
  Scml_Test_IP tip("ScmlTestIP");
  Scml_Orig_IP oip("ScmlOriginalPropertyIP");
 
  sc_core::sc_start();
  
  return EXIT_SUCCESS; 
  
}
