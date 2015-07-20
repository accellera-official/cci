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
#include "cci_configuration"


/// Testbench for the CCI development example testing the compare functionality of parameters of different mutability
int sc_main(int argc, char *argv[]) {

  cci::cci_param<int, cci::mutable_param>          int_param_mutable("int_param_mutable", cci::cci_value(10));
  cci::cci_param<int, cci::mutable_param>          int_param_mutable2 ("int_param_mutable2",  100);
  cci::cci_param<int, cci::immutable_param>        int_param_immutable("int_param_immutable", cci::cci_value(11));
  cci::cci_param<int, cci::elaboration_time_param> int_param_elab     ("int_param_elab",      11);

  cci::cci_param<std::string> str_param("str_param", cci::cci_value("This is a \nstring"));

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
