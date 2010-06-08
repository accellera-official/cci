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


/// Test IP with 2 parameters
class Test_IP
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(Test_IP);
	
  /// Constructor
  Test_IP(sc_core::sc_module_name name)
    : sc_core::sc_module(name)
    , param1("param1", 1000)
    , param2("param2", "hello")
    , param3("param3", "hello")
    , param4("param4", "hello")
    , param5("param5", "hello")
    , param6("param6", "hello")
  { 
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests.
  void main_action();

private:
  gs::gs_param<unsigned int> param1;
  gs::gs_param<std::string> param2;
  gs::gs_param<std::string> param3;
  gs::gs_param<std::string> param4;
  gs::gs_param<std::string> param5;
  gs::gs_param<std::string> param6;
  
};


#endif

