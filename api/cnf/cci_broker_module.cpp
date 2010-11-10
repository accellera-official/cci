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

cci_broker_manager::cci_broker_manager(cci_cnf_broker_if* broker)
: m_broker(broker) 
{ 
}
  
cci_broker_manager::~cci_broker_manager() { }

cci_cnf_broker_if* cci_broker_manager::get_broker() {
  if (m_broker) return m_broker;
  CCI_CNF_DUMP("   "<<dynamic_cast<sc_core::sc_object*>(this)->name() << ".get_broker() has not (yet) a private broker, search for other one");
  return search_for_broker(dynamic_cast<sc_core::sc_object*>(this)->get_parent_object());
}

void cci_broker_manager::register_private_broker(cci_cnf_broker_if* broker) {
  if (m_broker && broker) SC_REPORT_WARNING("CCI/broker_module", "Overwriting currently registered broker module!");
  m_broker = broker;
}

cci_cnf_broker_if* cci_broker_manager::search_for_broker(sc_core::sc_object* ob) {
  if (ob == NULL) return cci::cnf::get_cnf_broker_instance("");
  CCI_CNF_DUMP("search_for_broker in: "<<ob->name());
  cci_broker_manager* m = dynamic_cast<cci_broker_manager*>(ob);
  if (m) {
    CCI_CNF_DUMP("   got private broker"<< typeid(m->get_broker()).name()<<" 0x"<<(std::hex)<<m->get_broker()<<(std::dec));
    return m->get_broker();
  }
  sc_core::sc_object* o = ob->get_parent_object();
  return search_for_broker(o);
}


// Implementation cci_broker_manager_module

cci_broker_manager_module::cci_broker_manager_module(sc_core::sc_module_name name)
: sc_core::sc_module(name)
, cci_broker_manager(NULL) 
{
}


__CCI_CLOSE_CONFIG_NAMESPACE__
