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

#ifndef __TESTIP6_H__
#define __TESTIP6_H__

#include <systemc>

#include "greencontrol/config.h"


/// Test IP which uses the gs_param API with SystemC data types.
/**
 * This is a SystemC module which demonstrates the GreenControl parameter API.
 */
class TestIP6
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(TestIP6);
	
  /// Constructor
  TestIP6(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      scint_param ("scint_param", 50 )
  { 
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();

  /// Example parameter.
  gs::gs_param<sc_dt::sc_int<32> >      scint_param;

};


#endif

