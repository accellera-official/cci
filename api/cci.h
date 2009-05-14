//   OSCI CCI WG
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// 
// ENDLICENSETEXT


#ifndef __CCI_H__
#define __CCI_H__


#define OSCI_CCI_SC_REPORT_IDENT "/OSCI/CCI"

#include <systemc>

namespace cci {

  
  // forward declaration
  class cci_cnf_api;
  
  // forward declaration, to be implemented by tool
  cci_cnf_api* get_cnf_api_instance(sc_core::sc_module* = NULL);
  
  
} // end namespace cci

#include "cci_datatypes.h"
#include "cci_callbacks.h"
#include "cci_param_base.h"
#include "cci_param.h"
#include "cci_cnf_api.h"



#endif
