// LICENSETEXT
//
//   Copyright (C) 2011 : GreenSocs Ltd
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
#include "cci"


/// Shows a parameter list with information about value, explicit/implicit status and is_initial_value status
void show_param_list() {
  static cci::cnf::cci_cnf_broker_if* mBroker = &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator("PARAM_LIST"));

  std::cout << std::endl << "**** Parameter list: ";
  std::vector<std::string> vec = mBroker->get_param_list();
  std::vector<std::string>::iterator iter;
  std::stringstream ss_show;
  cci::cnf::cci_base_param* p;
  for (iter = vec.begin() ; iter < vec.end(); iter++) {
    ss_show << std::endl;
    ss_show << *iter << " : ";
    p = mBroker->get_param(*iter);
    if (p) {
      ss_show << "expl.";
      if (p->is_initial_value())       ss_show << ", initial";
      ss_show << ", val=" << p->json_serialize();
    } else {
      ss_show << "impl. (is always initial)";
      ss_show << ", val=" << mBroker->json_serialize(*iter);
    }
  }
  std::cout << "   " << ss_show.str() << std::endl<<std::endl;
}

/// Testbench for the CCI development example testing the is_initial_value functionality
int sc_main(int argc, char *argv[]) {
  
  cci::cnf::cci_cnf_broker_if* mBroker = &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator("SCMAIN"));
  
  mBroker->json_deserialize_initial_value("int_param_with_initial_value", "100");
  mBroker->json_deserialize_initial_value("int_param_only_implicit", "130");
  
  cci::cnf::cci_param<int> int_param_with_initial_value     ("int_param_with_initial_value",      10);
  cci::cnf::cci_param<int> int_param_without_initial_value  ("int_param_without_initial_value",   11);
  cci::cnf::cci_param<int> int_param_with_late_initial_value("int_param_with_late_initial_value", 12);
  
  show_param_list();  

  // This does NOT change the status to initial because this write to an explicit parameter is translated 
  // 'just' into a usual write
  mBroker->json_deserialize_initial_value("int_param_with_late_initial_value", "120");
  
  show_param_list();

  std::cout << std::endl << "------ sc_start() ----------------" << std::endl << std::endl;
  sc_core::sc_start(); 
  std::cout << std::endl << "------ sc_start() returned -------" << std::endl << std::endl;
  
  return EXIT_SUCCESS; 
  
}
