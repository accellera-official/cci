//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doxygen comments

#ifndef __TESTTOOL_NEG_WRONG_API_H__
#define __TESTTOOL_NEG_WRONG_API_H__

#include <systemc>

#include "../demo_globals.h"
#include "wrong_transactions_user_api.h"
#include "greencontrol/config.h"



/// Test Tool which uses the Wrong_Transactions_User_Api to generate negative regression tests
class TestTool_Wrong_Api
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(TestTool_Wrong_Api);
	
  /// Constructor
  TestTool_Wrong_Api(sc_core::sc_module_name name)
    : sc_core::sc_module(name)
  { 
    gcnfUserApi = gs::cnf::GCnf_Api::getApiInstance(this);
    SC_THREAD(main_action);
  }
  
  /// Main action to make negative tests.
  void main_action();

private:
  gs::cnf::Wrong_Transactions_User_Api directUserApi;
  gs::cnf::cnf_api_if *gcnfUserApi;
  
};


#endif
