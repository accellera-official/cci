// LICENSETEXT
//
//   Copyright (C) 2011 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#include <systemc>
#include "cci"


/// Testbench for the CCI development example testing the compare functionality of parameters of different mutability
int sc_main(int argc, char *argv[]) {

  cci::cnf::cci_param<int, cci::cnf::mutable_param>          int_param_mutable("int_param_mutable", cci::cnf::cci_value(10));
  cci::cnf::cci_param<int, cci::cnf::mutable_param>          int_param_mutable2 ("int_param_mutable2",  100);
  cci::cnf::cci_param<int, cci::cnf::immutable_param>        int_param_immutable("int_param_immutable", cci::cnf::cci_value(11));
  cci::cnf::cci_param<int, cci::cnf::elaboration_time_param> int_param_elab     ("int_param_elab",      11);

  cci::cnf::cci_param<std::string> str_param("str_param", cci::cnf::cci_value("This is a \nstring"));

  std::cout << "int_param_mutable2 == int_param_mutable: " << 
  ((int_param_mutable2 == int_param_mutable) ? "true" : "false") << std::endl;

  std::cout << "int_param_elab == int_param_mutable: " << 
  ((int_param_elab == int_param_mutable) ? "true" : "false") << std::endl;

  std::cout << "int_param_elab == int_param_immutable: " << 
  ((int_param_elab == int_param_immutable) ? "true" : "false") << std::endl;

  std::cout << "int_param_elab != int_param_immutable: " << 
  ((int_param_elab != int_param_immutable) ? "true" : "false") << std::endl;
  
  return EXIT_SUCCESS; 
  
}
