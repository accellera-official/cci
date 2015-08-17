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

#include <cci_configuration>
#include "gs_cci_broker.h" // Broker vendor dependent

namespace cci {

  /// Internal pointer to the one global default fallback broker instance 
  /// used in the case the appropriate broker is not known (or is this one)
  cci_broker_if* singleton_broker = new gs_cci_cnf_broker(true);
  
  /// Map containing all broker accessors so they can be found by get_cnf_broker_instance
  //std::map<std::string, gs_cci_cnf_broker_accessor> all_broker_accessors;

  /// Internal help function used by get_cnf_broker_instance
  /**
   * Organizes the broker accessors for the global broker instance
   * Note: private brokers and their accessors are not handled here!
   */
  /*static cci_broker_if* get_cnf_broker_instance_helper(const cci_originator& originator) {
    static bool entrance_guard = false; // debug information, guard to identify user calls with originator=NULL
    std::string originator_name = originator.name();
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
    if (originator_name.length() == 0 || originator_name.compare(__CCI_UNKNOWN_ORIGINATOR_STRING__) == 0) {
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
      gs_cci_cnf_broker_accessor *br = new gs_cci_cnf_broker_accessor(originator);
      all_broker_accessors.insert(make_pair(originator_name, *br));
      CCI_CNF_IMPL_DUMP("get_cnf_broker_instance returns NEW broker accessor for originator "<<originator_name);
      entrance_guard = false; // reset guard
      return dynamic_cast<cci_broker_if*>(br);
    }
  }*/
  
  /// Implements declaration from cci_config.h
  /**
   * Creates one broker accessor for each originator (stored in all_broker_accessors).
   * The broker accessor (constructor) checks itself for the correct (non-private / private) 
   * broker instance to be used.
   * Returns the broker accessor being responsible for the originator. 
   *
   * @param originator Object the returned broker shall be responsible for (or NULL)
   * @param Config broker accessor
   */
  /*cci_broker_if* get_cnf_broker_instance(cci_originator* originator) {
    assert(singleton_broker && "To be created during static initialization!");
    if (originator) return get_cnf_broker_instance_helper(*originator);
    else return get_cnf_broker_instance_helper(__CCI_UNKNOWN_ORIGINATOR_STRING__);
  }
  cci_broker_if* get_cnf_broker_instance(const cci_originator& originator) {
    assert(singleton_broker && "To be created during static initialization!");
    return get_cnf_broker_instance_helper(originator);
  }
  
  cci_broker_if* get_cnf_broker_instance(const char* TODO_REMOVE) {
    assert(false && "TODO: remove this functione!");
    return get_cnf_broker_instance_helper(__CCI_UNKNOWN_ORIGINATOR_STRING__);
  }*/
  
  /// Creates or returns the one non-private global config broker provided by the broker implementation
  /// Called by the header function get_current_broker (TODO: check if renamed?), NEVER call as a user!
  /**
   * This returns the raw broker, not an accessor, thus this 
   * shall not be returned directy to the user!
   * @return The one non-private global config broker (not wrapped with an accessor)
   */
  cci_broker_if& create_global_cnf_broker() {
    //static bool added_to_registry = false;
    //if (!added_to_registry) { 
    //  cci_broker_registry::registry().insert(*singleton_broker); // TODO: when remove?
    //  added_to_registry = true;
    //}
    return *singleton_broker;
  }
  
} // end namespace
