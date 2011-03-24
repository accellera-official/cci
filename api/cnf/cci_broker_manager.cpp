// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
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


#include "cci.h"

__CCI_OPEN_CONFIG_NAMESPACE__


// Implementation cci_broker_manager

// static get function
cci_cnf_broker_if& cci_broker_manager::get_current_broker(const cci_originator& originator) {
  if (std::string(originator.name()).empty()) {
#ifdef CCI_CNF_VERBOSE 
    SC_REPORT_INFO("CCI/get_current_broker", "It is recommended not to get a broker without originator information (NULL pointer or empty string)!");
#endif
  }
  if (cci_broker_stack::stack().size() > 0) {
    return cci_broker_stack::stack().top()->get_accessor(originator);
  } 
  // else return global broker
  else {
    return cci::cnf::create_global_cnf_broker().get_accessor(originator);
  }
}

// static get function
cci_cnf_broker_if& cci_broker_manager::get_current_parent_broker(const cci_originator& originator) {
  if (std::string(originator.name()).empty()) {
#ifdef CCI_CNF_VERBOSE 
    SC_REPORT_INFO("CCI/get_current_broker", "It is recommended not to get a broker without originator information (NULL pointer or empty string)!");
#endif
  }
  if (cci_broker_stack::stack().size() > 1) {
    return cci_broker_stack::stack().second_top()->get_accessor(originator);
  } 
  // else return global broker
  else {
    return cci::cnf::create_global_cnf_broker().get_accessor(originator);
  }
}


cci_broker_manager::cci_broker_manager(cci_cnf_broker_if* broker)
: m_own_private_broker(broker) 
{
  // Try to cast this into an sc_object for originator information
  // TODO: Does this not work during construction???
  //sc_core::sc_object* o = dynamic_cast<sc_core::sc_object*>(this);
  //if (!o) SC_REPORT_INFO("CCI/cci_broker_manager", "It is recommended to use the cci_broker_manager as a base for sc_modules!");
  
  // Set m_broker either to own private broker or the one responsible upwards the hierarchy
  if (m_own_private_broker) {
    CCI_CNF_DUMP("This broker manager has an own private broker (\""<< m_own_private_broker->name() <<"\")");
    // register the own broker with the registry
    cci_broker_registry::registry().insert(*m_own_private_broker);
    // push to broker stack
    cci_broker_stack::stack().push(m_own_private_broker);
    if (m_own_private_broker->get_originator()) {
      m_broker = &m_own_private_broker->get_accessor(*m_own_private_broker->get_originator());
    } else {
      std::cout << "The private broker (\""<<m_own_private_broker->name()<<"\") given to this broker manager IS NOT an accessor." << std::endl;
      SC_REPORT_INFO("CCI/cci_broker_manager", "It is recommended to provide a broker accessor to the broker manager!");
      m_broker = &m_own_private_broker->get_accessor(cci_originator("unknown broker manager")); // TODO: what string is reasonable here? 
    }
    // @TODO move to somewhere else: where?
    // pop from broker stack
    //cci_broker_stack::stack().pop();
  } else {
    m_broker = &get_current_broker(cci_originator("unknown broker manager")); // TODO: what string is reasonable here? 
  }
}
  
cci_broker_manager::~cci_broker_manager() {
  // remove the own broker from the registry
  if (m_own_private_broker) cci_broker_registry::registry().remove(*m_own_private_broker);
}

// protected
cci_cnf_broker_if& cci_broker_manager::get_broker() const {
  return *m_broker;
}

/*void cci_broker_manager::register_private_broker(cci_cnf_broker_if* broker) {
  if (m_broker && broker) SC_REPORT_WARNING("CCI/broker_module", "Overwriting currently registered broker module!");
  m_broker = broker;
}*/


__CCI_CLOSE_CONFIG_NAMESPACE__
