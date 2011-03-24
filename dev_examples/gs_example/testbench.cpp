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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#include <systemc>
#include "cci.h"

#include "ParameterOwnerModule.h"
#include "ParameterOwnerMutabilityModule.h"
#include "ObserverModule.h"
#include "ParamManipulateModule.h"
#include "ValueModule.h"

/// Testbench for the CCI example application which uses the GreenSocs demo implemenation
int sc_main(int argc, char *argv[]) {
  //sc_core::sc_report_handler::set_actions(sc_core::SC_ERROR, sc_core::SC_ABORT);
  //sc_core::sc_report_handler::set_actions(sc_core::SC_WARNING, sc_core::SC_ABORT);

  // Note: this prevents from catching the error!!
  //sc_core::sc_report_handler::set_actions("/OSCI/CCI/set_param_failed",  sc_core::SC_DISPLAY);
  sc_core::sc_report_handler::set_actions("/OSCI/CCI/cci_value_failure", sc_core::SC_DISPLAY);


  ObserverModule        observer   ("Observer");
  ParamManipulateModule manipulator("Manipulator");
  ParameterOwnerModule  owner      ("Owner");
  ParameterOwnerMutabilityModule  mutability_owner      ("MutabilityOwner");
  ValueModule           valueMod   ("ValueMod");
  
  cci::cnf::cci_param<std::string> top_param1("top.param1", "This is a param with top-level-name", cci::cnf::CCI_TOP_LEVEL_NAME);
  cci::cnf::cci_param<bool>        top_param2("param2", "This is a param with top-level-name");
  cci::cnf::cci_param<int>         top_param3("top.arbitrary.param3", "This is a param with top-level-name", cci::cnf::CCI_TOP_LEVEL_NAME);

  std::cout << std::endl << "------ sc_start() ----------------" << std::endl << std::endl;
  sc_core::sc_start(); 
  std::cout << std::endl << "------ sc_start() returned -------" << std::endl << std::endl;
  
  return EXIT_SUCCESS; 
  
}
