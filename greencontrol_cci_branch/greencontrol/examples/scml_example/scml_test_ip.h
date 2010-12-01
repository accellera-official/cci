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

#ifndef __SCML_TEST_IP_H__
#define __SCML_TEST_IP_H__

#include <boost/tokenizer.hpp> // Because of buggy boost integrated in SystemC 2.1
#include <systemc>

#include "globals.h"

#include "greencontrol/config.h"


#define DumpScmlTest(a) { std::cout << "Scml test: " << a <<std::endl; }
#define SPACE {std::cout << std::endl; }


/// Test IP which uses tests the scml database adaptor.
/**
 * This is a SystemC module which tests the scml database adaptor.
 */
class Scml_Test_IP
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(Scml_Test_IP);
	
  /// Constructor
  Scml_Test_IP(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  { 
    api = gs::cnf::GCnf_Api::getApiInstance(this);
    SC_THREAD(main_action);

    // Initial values
    DumpScmlTest("Set scml_property parameter 'ScmlOriginalPropertyIP.m_string_prop' = 'initial value written to registry prior to existence'");
    api->setInitValue("ScmlOriginalPropertyIP.m_string_prop", "initial value written to registry prior to existence");
    
    DumpScmlTest("Set scml_property parameter 'ScmlOriginalPropertyIP.m_int_prop' = 10");
    api->setInitValue("ScmlOriginalPropertyIP.m_int_prop", "10");
    
  }
  
  /// Main action to make tests.
  void main_action();

private:
  
  gs::cnf::cnf_api_if *api;
  
};


#endif

