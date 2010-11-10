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

// This file is broker vendor dependent but standardized file name!

#include <cci.h>
#include "cci_api.h" // Broker vendor dependent but standardized file name!

namespace cci {
namespace cnf {


  cci_cnf_broker_if* singleton_api = NULL;

  /// Implements declaration from cci_config.h
  cci_cnf_broker_if* get_cnf_broker_instance(sc_core::sc_object* originator) {
    // TODO: don't use a single because of originator being needed
    if (!singleton_api) singleton_api = new gs_cci_cnf_broker();
    if (originator != NULL) return cci_broker_manager::search_for_broker(originator);
    //CCI_CNF_DUMP("   got global broker "<< typeid(singleton_api).name()<<" 0x"<<(std::hex)<<singleton_api<<(std::dec));
    return singleton_api;
  }

  /// Implements declaration from cci_config.h
  cci_cnf_broker_if* get_cnf_broker_instance(const std::string& originator) {
    // TODO: don't use a single because of originator being needed
    if (!singleton_api) singleton_api = new gs_cci_cnf_broker();
    //if (mod != NULL) return cci_broker_manager::search_for_broker(ob);
    //CCI_CNF_DUMP("   got global broker "<< typeid(singleton_api).name()<<" 0x"<<(std::hex)<<singleton_api<<(std::dec));
    return singleton_api;
  }
  
  
} // end namespace cci
} // end namespace cci
