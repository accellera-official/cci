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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

//#define GS_PARAM_CALLBACK_VERBOSE

// GreenConfig
#include "greencontrol/config.h" 

#include "OwningModule.h"
#include "PrivateOwningModule.h"

/*
 GreenConfig is_used tests
 
*/

void is_used_test(gs::cnf::cnf_api_if* m_cnf_api) {
  std::vector<std::string> par;
  par.push_back("OM.int_param1");
  par.push_back("OM.int_param2");
  par.push_back("OM.int_param3");
  par.push_back("OM.int_param4");
  par.push_back("OM.int_param5");
  par.push_back("OMP.pint_param1_pub");

  std::cout << std::endl << "is_used check:" << std::endl; 
  for (std::vector<std::string>::iterator iter = par.begin(); iter!=par.end(); iter++) {
    //m_cnf_api->is_used(par);
    std::cout << "   " << *iter << " is_used = ";
    std::cout << m_cnf_api->is_used(*iter);
    std::cout << std::endl; 
  }
  std::cout << std::endl; 
}

/// Testbench for the GreenConfig regression test
int sc_main(int argc, char *argv[]) {
  
  /// GreenControl Core instance
  gs::ctr::GC_Core       core;
  gs::cnf::ConfigDatabase cnfdatabase("ConfigDatabase");
  gs::cnf::ConfigPlugin configPlugin(&cnfdatabase);

  gs::cnf::cnf_api_if* m_cnf_api = gs::cnf::GCnf_Api::getApiInstance(NULL);
  std::cout << "** set init OM.int_param2" << std::endl;
  m_cnf_api->setInitValue("OM.int_param2", "12");
  std::cout << "** set init OM.int_param3" << std::endl;
  m_cnf_api->setInitValue("OM.int_param3", "13");
  std::cout << "** set init OM.int_param4" << std::endl;
  m_cnf_api->setInitValue("OM.int_param4", "14");
  std::cout << "** set init OM.int_param5" << std::endl;
  m_cnf_api->setInitValue("OM.int_param5", "15");

  std::cout << "** set init OMP.pint_param1_pub" << std::endl;
  m_cnf_api->setInitValue("OMP.pint_param1_pub", "1001");

  is_used_test(m_cnf_api);
  gs::cnf::show_database();
  
  std::cout << "** create module OM" << std::endl;
  OwningModule om("OM");

  std::cout << "** create module OMP with private params" << std::endl;
  PrivateOwningModule *omp = new PrivateOwningModule("OMP");

  is_used_test(m_cnf_api);
  gs::cnf::show_database();

  // affect is_used status
  std::cout << "** get OM.int_param4" << std::endl;
  m_cnf_api->getValue("OM.int_param4");

  // NOT affect is_used status
  std::cout << "** get OM.int_param5 (not_impact_is_used_status=true)" << std::endl;
  m_cnf_api->getValue("OM.int_param5", "", true);
  
  is_used_test(m_cnf_api);
  gs::cnf::show_database();

  std::cout << std::endl;
  std::cout << "START SIMULATION" << std::endl;
  sc_core::sc_start();
  std::cout << "END SIMULATION" << std::endl;
  std::cout << std::endl;

  is_used_test(m_cnf_api);
  gs::cnf::show_database();
    
  delete omp; omp = NULL;

  return EXIT_SUCCESS; 
  
}
