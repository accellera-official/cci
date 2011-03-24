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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#include <systemc>
#include <cci.h>

#include "ModuleA.h"
#include "ModuleB.h"
#include "ModuleC.h"
#include "ObserverModule.h"

/// Testbench for the CCI example application which uses the GreenSocs demo implemenation
int sc_main(int argc, char *argv[]) {
  //sc_core::sc_report_handler::set_actions(sc_core::SC_WARNING, sc_core::SC_ABORT);
  sc_core::sc_report_handler::set_actions(sc_core::SC_ERROR, sc_core::SC_ABORT);
  //sc_core::sc_report_handler::set_actions("/OSCI/CCI/set_param_failed",  sc_core::SC_DISPLAY);
  //sc_core::sc_report_handler::set_actions("/OSCI/CCI/cci_value_failure", sc_core::SC_DISPLAY);

  //ModuleB b("ModuleB");  //causes the global broker to be created
  ModuleC c("ModuleC");    //a private broker
  // TODO: remove this hack: poped previous private brokers from broker stack
  cci::cnf::cci_broker_stack::stack().pop();
  ModuleA a1("ModuleA");   //a different private broker
  // TODO: remove this hack: poped previous private brokers from broker stack
  cci::cnf::cci_broker_stack::stack().pop();
  ModuleA a2("ModuleA2");  //anther instance of a private broker
  // TODO: remove this hack: poped previous private brokers from broker stack
  cci::cnf::cci_broker_stack::stack().pop();
  ModuleB b2("ModuleB2");
  ObserverModule        observer   ("Observer");
  //uncomment following line to set observer's mBroker pointer directly
  //observer.mBroker = c.get_broker();

  std::cout << std::endl << "------ sc_start() ----------------" << std::endl << std::endl;
  sc_core::sc_start(); 
  std::cout << std::endl << "------ sc_start() returned -------" << std::endl << std::endl;
  
  return EXIT_SUCCESS; 
  
}
