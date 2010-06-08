//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2008 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
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

#ifndef __ParamListIP_H__
#define __ParamListIP_H__

#include <systemc>

#include "../demo_globals.h"
#include "greencontrol/config.h"
#include "greencontrol/core/helpfunctions.h"

/// Test IP which tests parameter lists
/**
 */
class ParamListIP
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(ParamListIP);
	
  /// Constructor
  ParamListIP(sc_core::sc_module_name name)
    : sc_core::sc_module(name)
  { 
    SC_THREAD(main_action);
    mApi = gs::cnf::GCnf_Api::getApiInstance(this);
  }
  
  /// Destructor
  ~ParamListIP() {
  }

  /// Main action to make tests with parameters.
  void main_action();

  void show_params_vec(std::vector<gs::gs_param_base*> params_vec);
  
private:
  
  gs::cnf::cnf_api* mApi;
  
};


#endif

