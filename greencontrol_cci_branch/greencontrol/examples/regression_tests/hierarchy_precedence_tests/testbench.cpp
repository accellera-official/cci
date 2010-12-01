//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#include "greencontrol/config.h"   // GreenConfig

#include "ModuleATop.h"

/*
 Hierarchical/time precendence tests
 
                           ModuleATop1
                                 setInitValue ("ModuleATop1.ModuleBSubsystem1.ModuleC1.ModuleD2.my_param3", val);
 
 
                 ModuleATop1.ModuleBSubsystem1
                                 setInitValue ("ModuleATop1.ModuleBSubsystem1.ModuleC1.ModuleD1.my_param2", val);

 
            ModuleATop1.ModuleBSubsystem1.ModuleC1
                                 setInitValue ("ModuleATop1.ModuleBSubsystem1.ModuleC1.ModuleD1.my_param1", val);
                                 setInitValue ("ModuleATop1.ModuleBSubsystem1.ModuleC1.ModuleD1.my_param3", val);
 
                                 setInitValue ("ModuleATop1.ModuleBSubsystem1.ModuleC1.ModuleD2.my_param1", val);
                                 setInitValue ("ModuleATop1.ModuleBSubsystem1.ModuleC1.ModuleD2.my_param3", val);
 
 
       ModuleATop1.ModuleBSubsystem1.ModuleC1.ModuleD1
 ModuleATop1.ModuleBSubsystem1.ModuleC1.ModuleD1.my_param1   <= set by ModuleC1 (init val)
 ModuleATop1.ModuleBSubsystem1.ModuleC1.ModuleD1.my_param2   <= set by ModuleBSubsystem1 (init val)
 ModuleATop1.ModuleBSubsystem1.ModuleC1.ModuleD1.my_param3   <= set by ModuleC1 (init val)

       ModuleATop1.ModuleBSubsystem1.ModuleC1.ModuleD2
 ModuleATop1.ModuleBSubsystem1.ModuleC1.ModuleD2.my_param1   <= set by ModuleC1 (init val)
 ModuleATop1.ModuleBSubsystem1.ModuleC1.ModuleD2.my_param2   <= set by this (default)
 ModuleATop1.ModuleBSubsystem1.ModuleC1.ModuleD2.my_param3   <= set by ModuleC1 AND ModuleATop1 (init val), result depending on precedence mechanism
 
*/

/// Testbench for the GreenConfig regresson test
int sc_main(int argc, char *argv[]) {
  
  /// GreenControl Core instance
  gs::ctr::GC_Core       core;
  gs::cnf::ConfigDatabase cnfdatabase("ConfigDatabase");
  gs::cnf::ConfigPlugin configPlugin(&cnfdatabase);

  ModuleATop topmodA("ModuleATop1");
  
  sc_core::sc_start();
  
  std::cout << "END SIMULATION" << std::endl;
  std::cout << "Parameter list:" << std::endl;
  gs::cnf::show_database();
    
  return EXIT_SUCCESS; 
  
}
