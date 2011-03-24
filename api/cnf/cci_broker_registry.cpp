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


#include "cci.h"

__CCI_OPEN_CONFIG_NAMESPACE__

void cci_broker_registry::insert(cci_cnf_broker_if& broker) {
#ifdef CCI_CNF_VERBOSE
  // check if broker is already inserted
  for(int i = size()-1; i>=0; --i) {
    if(&broker == m_broker_vec[i]) {
      CCI_THROW_ERROR(cci::cnf::cci_report::broker_failure().get_type(), "broker already inserted");
    }
  }
#endif
  m_broker_vec.push_back(&broker);
}

void cci_broker_registry::remove(cci_cnf_broker_if& broker) {
  int i;
  for(i=0; i<size(); ++i) {
    if(&broker == m_broker_vec[i]) {
	    break;
    }
  }
  if(i == size()) {
    CCI_THROW_ERROR(cci::cnf::cci_report::broker_failure().get_type(), "remove broker failed");
  }
  
  m_broker_vec[i] = m_broker_vec[size()-1];
  m_broker_vec.resize(m_broker_vec.size()-1);
  
  // TODO: remove all related broker accessors or disable their forwarding calls!
}

int cci_broker_registry::size() const { return m_broker_vec.size(); }


cci_broker_registry::cci_broker_registry() {
}

cci_broker_registry::~cci_broker_registry() {
}

// static 
cci_broker_registry& cci_broker_registry::registry() {
  static cci_broker_registry registry;
  return registry;
}


__CCI_CLOSE_CONFIG_NAMESPACE__

