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

#include <cci.h>
#include "gs_cci_cnf_api.h"


namespace cci {


  cci_cnf_api* singleton_api = NULL;

  cci_cnf_api* get_cnf_api_instance(sc_core::sc_module*) {
    if (!singleton_api) singleton_api = new gs_cci_cnf_api();
    return singleton_api;
  }

  
} // end namespace cci
