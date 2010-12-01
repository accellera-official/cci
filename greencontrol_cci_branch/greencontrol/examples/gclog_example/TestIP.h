//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Michael Ruetz <m.ruetz@tu-bs.de>,
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>
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

#ifndef __TESTIP_H__
#define __TESTIP_H__

#include <systemc>

#include "greencontrol/gcnf/apis/toolApi/tool_gcnf_api.h"

using namespace std;
using namespace sc_core;
using namespace sc_dt;

class TestIP : public sc_module
{
public:

  SC_HAS_PROCESS(TestIP);

  /// Constructor
  TestIP(sc_module_name name)
    : sc_module(name),
    int_param ("int_param", 50)
  {
    SC_THREAD(main_action);

    gc_api = gs::cnf::GCnf_Api::getApiInstance(this);
  }

  void main_action();

private:
  gs::gs_param<sc_int<32> >  int_param;
  gs::cnf::cnf_api_if *gc_api;

  void timeTest(void);
};

#endif
