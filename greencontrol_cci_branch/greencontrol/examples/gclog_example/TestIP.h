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
//   This program is free software.
// 
//   If you have no applicable agreement with GreenSocs Ltd, this software
//   is licensed to you, and you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   If you have a applicable agreement with GreenSocs Ltd, the terms of that
//   agreement prevail.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//   02110-1301  USA 
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
  gs::cnf::cnf_api *gc_api;

  void timeTest(void);
};

#endif
