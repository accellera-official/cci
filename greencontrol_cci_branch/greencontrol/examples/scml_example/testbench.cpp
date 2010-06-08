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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
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
  gs::ctr::GC_Core       core;

#ifdef USE_SCML_DATABASE_ADAPTER
  cout << "Use database 'Scml_database_adaptor'"<<endl;
  gs::cnf::Scml_database_adaptor cnfdatabase("ScmlDatabaseAdaptor");
#else
  std::cout << "Use standart database 'ConfigDatabase'"<<std::endl;
  gs::cnf::ConfigDatabase cnfdatabase("ConfigDatabase");
#endif
  gs::cnf::ConfigPlugin configPlugin(&cnfdatabase);
 
  Scml_Test_IP tip("ScmlTestIP");
  Scml_Orig_IP oip("ScmlOriginalPropertyIP");
 
  sc_core::sc_start();
  
  return EXIT_SUCCESS; 
  
}
