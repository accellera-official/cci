//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Marcus Bartholomeu <bartho@greensocs.com>
//     GreenSocs
//     http://www.greensocs.com
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doxygen comments

#ifndef __TEST_IP_H__
#define __TEST_IP_H__

#include "greencontrol/config.h"


/// Test IP which uses the lua config reader
class Test_IP
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(Test_IP);
	
  /// Constructor
  Test_IP(sc_core::sc_module_name name)
    : sc_core::sc_module(name), param1("param1", 1000), param2("param2", "hello")
  { 
    api = gs::cnf::GCnf_Api::getApiInstance(this);
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests.
  void main_action();

private:
  gs::cnf::cnf_api_if *api;

  gs::gs_param<unsigned int> param1;
  gs::gs_param<std::string> param2;
  
};


#endif

