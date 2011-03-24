// LICENSETEXT
//
//   Copyright (C) 2011 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#include "gs_cci_cnf_broker_accessor_handler.h"


/**
 * Organizes the broker accessors for this broker instance
 * Note: private brokers and their accessors are not handled here!
 * @see cci::cnf::gs_cci_private_broker::get_accessor
 * @param orig_broker The original broker this accessor shall be created for.
 */
cci::cnf::cci_cnf_broker_if& cci::cnf::gs_cci_cnf_broker_accessor_handler::get_accessor(const cci_originator& originator, cci::cnf::cci_cnf_broker_if& orig_broker) {
  std::string originator_name = originator.name();
  if (originator_name.empty()) {
    originator_name = __CCI_UNKNOWN_ORIGINATOR_STRING__;
  }
  // if there is already a broker accessor for this originator, return that
  std::map<std::string, gs_cci_cnf_broker_accessor>::iterator it = m_broker_accessors.find(originator_name);
  if (it != m_broker_accessors.end()) {
    if (it->second.is_private_broker()) {
      CCI_CNF_IMPL_DUMP("get_accessor returns EXISTING accessor to private broker \""<< orig_broker.name() <<"\" for originator \""<<originator_name<<"\"");
    } else {
      CCI_CNF_IMPL_DUMP("get_accessor returns EXISTING accessor to public  broker \""<< orig_broker.name() <<"\" for originator \""<<originator_name<<"\"");
    }
    return it->second;
  }
  // else create a broker accessor for this originator
  else {
    gs_cci_cnf_broker_accessor *br = new gs_cci_cnf_broker_accessor(originator, orig_broker);
    m_broker_accessors.insert(make_pair(originator_name, *br));
    if (br->is_private_broker()) {
      CCI_CNF_IMPL_DUMP("get_accessor returns   NEW    accessor to private broker \""<< orig_broker.name() <<"\" for originator \""<<originator_name<<"\"");
    } else {
      CCI_CNF_IMPL_DUMP("get_accessor returns   NEW    accessor to public  broker \""<< orig_broker.name() <<"\" for originator \""<<originator_name<<"\"");
    }
    return *br;
  }
}

