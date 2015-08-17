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
#include <cci_configuration>

#include "ModuleA.h"
#include "ObserverModule.h"

class Top
: public sc_core::sc_module
{
  
public:
  
  SC_HAS_PROCESS(Top);
  Top(sc_core::sc_module_name name) {
    
    // Note: This would also be possible:
    // Private broker for Modules A and A2
    //   Parameters "ModuleA.int_param" and "ModuleA2.int_param" are public
    //top_privBroker = new cci::gs_cci_private_broker_accessor(*this, boost::assign::list_of("ModuleA.int_param")("ModuleA2.int_param"), cci::cci_originator(*this));
    
    // Private broker for Module A
    //   Parameter "ModuleA.int_param" is public
    moduleA_privBroker = new cci::gs_cci_private_broker_accessor(*this, boost::assign::list_of("ModuleA.int_param"));
    a = new ModuleA("ModuleA", moduleA_privBroker);
    
    // Private broker for Module A2
    //   Parameter "ModuleA2.int_param" is public
    moduleA2_privBroker = new cci::gs_cci_private_broker_accessor(*this, boost::assign::list_of("ModuleA2.int_param"));
    a2 = new ModuleA("ModuleA2", moduleA2_privBroker);
    
    observer = new ObserverModule("Observer");    
  }
  
  ~Top() {
    delete observer;
    delete a2;
    delete moduleA2_privBroker;
    delete a;
    delete moduleA_privBroker;
  }
  
protected:
  
  ModuleA* a;
  ModuleA* a2;
  ObserverModule* observer;
  
  cci::cci_broker_if* moduleA_privBroker;
  cci::cci_broker_if* moduleA2_privBroker;
  
};


/// Testbench for the CCI example application which uses the GreenSocs demo implemenation
int sc_main(int argc, char *argv[]) {
  //sc_core::sc_report_handler::set_actions(sc_core::SC_WARNING, sc_core::SC_ABORT);
  sc_core::sc_report_handler::set_actions(sc_core::SC_ERROR, sc_core::SC_ABORT);
  //sc_core::sc_report_handler::set_actions("/ASI/CCI/set_param_failed",  sc_core::SC_DISPLAY);
  //sc_core::sc_report_handler::set_actions("/ASI/CCI/cci_value_failure", sc_core::SC_DISPLAY);


  Top top("Top");

  std::cout << std::endl << "------ sc_start() ----------------" << std::endl << std::endl;
  sc_core::sc_start(); 
  std::cout << std::endl << "------ sc_start() returned -------" << std::endl << std::endl;
  
  return EXIT_SUCCESS; 
  
}


