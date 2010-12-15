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

// This file is broker vendor dependent

#include <cci.h>
#include "gs_cci_broker.h" // Broker vendor dependent

namespace cci {
namespace cnf {

  /// Internal pointer to the one global default fallback broker instance 
  /// used in the case the appropriate broker is not known (or is this one)
  cci_cnf_broker_if* singleton_broker = new gs_cci_cnf_broker_accessor(true);
  
  /// Map containing all broker accessors so they can be found by get_cnf_broker_instance
  std::map<std::string, gs_cci_cnf_broker_accessor> all_broker_accessors;

  /// Internal help function used by get_cnf_broker_instance
  static cci_cnf_broker_if* get_cnf_broker_instance_helper(const std::string& originator_name, sc_core::sc_object* originator_object) {
    static bool entrance_guard = false; // debug information, guard to identify user calls with originator=NULL
    if (originator_name.empty() && !entrance_guard) { // debug information, do not warn if nested call of this function
      CCI_CNF_IMPL_DUMP("get_cnf_broker_instance returns GLOBAL default broker accessor for originator NULL");
#ifdef CCI_CNF_IMPL_VERBOSE 
      SC_REPORT_INFO("CCI/get_cnf_broker_instance", "It is recommended not to call get_cnf_broker_instance without originator information (NULL pointer or empty string)!");
#endif
    }
    entrance_guard = true; // disable warning above on nested call of this function
    //if (!singleton_broker) {
    //  CCI_CNF_IMPL_DUMP("create GLOBAL default broker accessor");
    //  singleton_broker = new gs_cci_cnf_broker_accessor(true); // just to ensure it exists
    //}
    if (originator_name.length() == 0) {
      entrance_guard = false; // reset guard
      return singleton_broker;
    }
    // if there is already a broker accessor for this originator, return that
    std::map<std::string, gs_cci_cnf_broker_accessor>::iterator it = all_broker_accessors.find(originator_name);
    if (it != all_broker_accessors.end()) {
      CCI_CNF_IMPL_DUMP("get_cnf_broker_instance returns EXISTING broker accessor for originator "<<originator_name);
      entrance_guard = false; // reset guard
      return &(it->second);
    }
    // else create a broker accessor for this originator
    else {
      gs_cci_cnf_broker_accessor *br;
      if (originator_object)
        br = new gs_cci_cnf_broker_accessor(originator_object);
      else
        br = new gs_cci_cnf_broker_accessor(originator_name);
      all_broker_accessors.insert(make_pair(originator_name, *br));
      CCI_CNF_IMPL_DUMP("get_cnf_broker_instance returns NEW broker accessor for originator "<<originator_name);
      entrance_guard = false; // reset guard
      return dynamic_cast<cci_cnf_broker_if*>(br);
    }
  }
  
  /// Implements declaration from cci_config.h
  /**
   * Creates one broker accessor for each originator (stored in all_broker_accessors).
   * The broker accessor (constructor) checks itself for the correct (non-private / private) 
   * broker instance to be used.
   * Returns the broker accessor being responsible for the originator. 
   *
   * @param originator Object the returned broker shall be responsible for
   * @param Config broker accessor
   */
  cci_cnf_broker_if* get_cnf_broker_instance(sc_core::sc_object* originator) {
    assert(singleton_broker && "To be created during static initialization!");
    std::string originator_name = (originator)? originator->name() : "";
    return get_cnf_broker_instance_helper(originator_name, originator);
  }

  /// Implements declaration from cci_config.h
  cci_cnf_broker_if* get_cnf_broker_instance(const std::string& originator) {
    assert(singleton_broker && "To be created during static initialization!");
    return get_cnf_broker_instance_helper(originator, NULL);
  }
  
  
} // end namespace cci
} // end namespace cci
