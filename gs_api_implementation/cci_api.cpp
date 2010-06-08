// LICENSETEXT
//
//   Copyright (C) 2009-2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//    Mark Burton, mark@greensocs.com
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#include <cci.h>
#include "cci_api.h"

namespace cci {
  namespace cnf {


  cci_cnf_broker* singleton_api = NULL;

  cci_cnf_broker* get_cnf_broker_instance(sc_core::sc_module* mod) {
    if (!singleton_api) singleton_api = new gs_cci_cnf_broker();
    if (mod != NULL) return cci_broker_manager::search_for_broker(mod);
    CCI_CNF_DUMP("   got global broker "<< typeid(singleton_api).name()<<" 0x"<<(std::hex)<<singleton_api<<(std::dec));
    return singleton_api;
  }

  
  } // end namespace cci
} // end namespace cci
