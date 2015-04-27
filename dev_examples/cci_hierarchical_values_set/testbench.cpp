// LICENSETEXT
//
//   Copyright (C) 2009-2011 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//    Mark Burton, mark@greensocs.com
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

/**
 * @file   cci_hierarchical_values_set/ChildModule.h
 * @author Lei Liang, Ericsson
 * @brief  Simple example for setting hierarchical values
 */

#include <systemc>
#include "cci"

#include "ParentModule.h"
#include "ConfigModule.h"

int sc_main(int argc, char *argv[]) {


  ParentModule     sim_ip   ("sim_ip");
  ConfigModule     cfg_ip   ("cfg_ip");
  
  std::cout << std::endl << "------ sc_start() ----------------" << std::endl << std::endl;
  sc_core::sc_start(); 
  std::cout << std::endl << "------ sc_start() returned -------" << std::endl << std::endl;
  
  return EXIT_SUCCESS; 
  
}
