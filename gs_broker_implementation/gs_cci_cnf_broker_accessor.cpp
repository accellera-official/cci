// LICENSETEXT
//
//   Copyright (C) 2009-2010 : GreenSocs Ltd
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


#include "gs_cci_cnf_broker_accessor.h"
#include "gs_cci_cnf_private_broker_accessor.h"
#include "gs_cci_broker.h"

cci::cnf::gs_cci_cnf_broker_accessor::gs_cci_cnf_broker_accessor(sc_core::sc_object* originator) 
: m_broker(NULL)
, m_originator_obj(originator) 
, m_originator_str("") 
{ 
  if (originator == NULL) {
    m_originator_str = GS_CCI_IMPL_UNKNOWN_ORIGINATOR_STRING;
  } else {
    m_originator_str = originator->name();
  }
  // This will return the appropriate private or non-private broker
  cci::cnf::cci_cnf_broker_if *br = cci::cnf::cci_broker_manager::search_for_broker(originator);
  gs_cci_cnf_broker_accessor *gs_br = dynamic_cast<gs_cci_cnf_broker_accessor*>(br);
  if (gs_br) {
    m_broker = gs_br->get_gs_broker();
    CCI_CNF_IMPL_DUMP("Created new cci broker accessor \""<< m_originator_str << "\"" << 
                      " using existing broker \""<< dynamic_cast<gs::ctr::command_if*>(m_broker)->getName() << "\"");
  }
  else {
    gs_cci_private_broker_accessor *gs_pbr = dynamic_cast<gs_cci_private_broker_accessor*>(br);
    if (gs_pbr) {
      m_broker = gs_pbr->get_gs_broker();
      CCI_CNF_IMPL_DUMP("Created new cci broker accessor \""<< m_originator_str << "\"" << 
                        " using existing PRIVATE broker \""<< dynamic_cast<gs::ctr::command_if*>(m_broker)->getName() << "\"");
    }
  }
  assert(m_broker && "Must be a (non-)private gs_cci_cnf_broker_if (which means eather a gs_cci_cnf_broker or a gs_cci_private_broker)!");
}

cci::cnf::gs_cci_cnf_broker_accessor::gs_cci_cnf_broker_accessor(const std::string& originator) 
: m_broker(NULL)
, m_originator_obj(NULL) 
, m_originator_str(originator)
{ 
  if (originator == "") {
    m_originator_str = GS_CCI_IMPL_UNKNOWN_ORIGINATOR_STRING;
  }
  // This will return the appropriate (non-private) broker
  cci::cnf::cci_cnf_broker_if *br = cci::cnf::cci_broker_manager::search_for_broker(sc_core::sc_find_object(originator.c_str())); // TODO: this might be deactivated because it could be used to hack
  //cci::cnf::cci_cnf_broker_if *br = cci::cnf::cci_broker_manager::search_for_broker(NULL);
  // DEBUG WARNING if there is an object existing with the originator name but not being used:
  //if (originator.length() > 0 && sc_find_object(originator))
  //  SC_REPORT_WARNING(TODO);
  gs_cci_cnf_broker_accessor *gs_br = dynamic_cast<gs_cci_cnf_broker_accessor*>(br);
  if (gs_br) {
    m_broker = gs_br->get_gs_broker();
    CCI_CNF_IMPL_DUMP("Created new cci broker accessor \""<< m_originator_str << "\"" << 
                      " using existing broker \""<< dynamic_cast<gs::ctr::command_if*>(m_broker)->getName() << "\"");
  }
  else {
    gs_cci_private_broker_accessor *gs_pbr = dynamic_cast<gs_cci_private_broker_accessor*>(br);
    if (gs_pbr) {
      m_broker = gs_pbr->get_gs_broker();
      CCI_CNF_IMPL_DUMP("Created new cci broker accessor \""<< m_originator_str << "\"" << 
                        " using existing PRIVATE broker \""<< dynamic_cast<gs::ctr::command_if*>(m_broker)->getName() << "\"");
    }
  }
  
}

cci::cnf::gs_cci_cnf_broker_accessor::gs_cci_cnf_broker_accessor(bool called_internally_for_creating_global_broker) 
: m_broker(NULL)
, m_originator_obj(NULL) 
, m_originator_str(GS_CCI_IMPL_UNKNOWN_ORIGINATOR_STRING)
{ 
  // This will return the appropriate (non-private) broker
  cci::cnf::gs_cci_cnf_broker *br = new cci::cnf::gs_cci_cnf_broker(std::string(GS_CCI_IMPL_UNKNOWN_ORIGINATOR_STRING));
  m_broker = dynamic_cast<gs_cci_cnf_broker_if*>(br);
  assert(m_broker && "Must be a (non-)private gs_cci_cnf_broker_if (which means eather a gs_cci_cnf_broker or a gs_cci_private_broker)!");
  CCI_CNF_IMPL_DUMP("Created global cci broker accessor \""<< m_originator_str << "\"");
}

cci::cnf::gs_cci_cnf_broker_accessor::~gs_cci_cnf_broker_accessor() { 
}

void cci::cnf::gs_cci_cnf_broker_accessor::set_init_value(const std::string &parname, const std::string &json_value) {
  if (m_originator_obj)
    m_broker->set_init_value(parname, json_value, m_originator_obj);
  else
    m_broker->set_init_value(parname, json_value, m_originator_str);
}

void cci::cnf::gs_cci_cnf_broker_accessor::lock_init_value(const std::string &parname) {
  if (m_originator_obj)
    m_broker->lock_init_value(parname, m_originator_obj);
  else
    m_broker->lock_init_value(parname, m_originator_str);
}

const std::string cci::cnf::gs_cci_cnf_broker_accessor::get_json_string(const std::string &parname) {
  // TODO: use JSON
  if (m_originator_obj)
    return m_broker->get_json_string(parname, m_originator_obj);
  else
    return m_broker->get_json_string(parname, m_originator_str);
}

const std::string cci::cnf::gs_cci_cnf_broker_accessor::get_json_string_keep_unused(const std::string &parname) {
  // TODO: use JSON
  if (m_originator_obj)
    return m_broker->get_json_string_keep_unused(parname, m_originator_obj);
  else
    return m_broker->get_json_string_keep_unused(parname, m_originator_str);
  
}

cci::cnf::cci_base_param* cci::cnf::gs_cci_cnf_broker_accessor::get_param(const std::string &parname) {
  if (m_originator_obj)
    return m_broker->get_param(parname, m_originator_obj);
  else
    return m_broker->get_param(parname, m_originator_str);
}

bool cci::cnf::gs_cci_cnf_broker_accessor::exists_param(const std::string &parname) {
  if (m_originator_obj)
    return m_broker->exists_param(parname, m_originator_obj);
  else
    return m_broker->exists_param(parname, m_originator_str);
}

bool cci::cnf::gs_cci_cnf_broker_accessor::is_used(const std::string &parname) {
  if (m_originator_obj)
    return m_broker->is_used(parname, m_originator_obj);
  else
    return m_broker->is_used(parname, m_originator_str);
}

const std::vector<std::string> cci::cnf::gs_cci_cnf_broker_accessor::get_param_list() {
  if (m_originator_obj)
    return m_broker->get_param_list(m_originator_obj);
  else
    return m_broker->get_param_list(m_originator_str);
}

cci::shared_ptr<cci::cnf::callb_adapt_b> cci::cnf::gs_cci_cnf_broker_accessor::register_callback(const std::string& parname, const callback_type type, cci::shared_ptr<cci::cnf::callb_adapt_b> callb) {
  if (m_originator_obj)
    return m_broker->register_callback(parname, type, callb, m_originator_obj);
  else
    return m_broker->register_callback(parname, type, callb, m_originator_str);
}

void cci::cnf::gs_cci_cnf_broker_accessor::unregister_all_callbacks(void* observer) {
  if (m_originator_obj)
    return m_broker->unregister_all_callbacks(observer, m_originator_obj);
  else
    return m_broker->unregister_all_callbacks(observer, m_originator_str);
}

bool cci::cnf::gs_cci_cnf_broker_accessor::unregister_param_callback(callb_adapt_b* callb) {
  if (m_originator_obj)
    return m_broker->unregister_param_callback(callb, m_originator_obj);
  else
    return m_broker->unregister_param_callback(callb, m_originator_str);
}

bool cci::cnf::gs_cci_cnf_broker_accessor::has_callbacks(const std::string& parname) {
  if (m_originator_obj)
    return m_broker->has_callbacks(parname, m_originator_obj);
  else
    return m_broker->has_callbacks(parname, m_originator_str);
}

void cci::cnf::gs_cci_cnf_broker_accessor::add_param(cci_base_param* par) {
  if (m_originator_obj)
    return m_broker->add_param(par, m_originator_obj);
  else
    return m_broker->add_param(par, m_originator_str);
}

void cci::cnf::gs_cci_cnf_broker_accessor::remove_param(cci_base_param* par) {
  if (m_originator_obj)
    return m_broker->remove_param(par, m_originator_obj);
  else
    return m_broker->remove_param(par, m_originator_str);
}

const std::vector<std::string> cci::cnf::gs_cci_cnf_broker_accessor::get_param_list(const std::string& pattern) {
  if (m_originator_obj)
    return m_broker->get_param_list(pattern, m_originator_obj);
  else
    return m_broker->get_param_list(pattern, m_originator_str);
}

const std::vector<cci::cnf::cci_base_param*> cci::cnf::gs_cci_cnf_broker_accessor::get_params(const std::string& pattern) {
  if (m_originator_obj)
    return m_broker->get_params(pattern, m_originator_obj);
  else
    return m_broker->get_params(pattern, m_originator_str);
}

const std::string& cci::cnf::gs_cci_cnf_broker_accessor::get_originator() {
  return m_originator_str;
}

cci::cnf::gs_cci_cnf_broker_if* cci::cnf::gs_cci_cnf_broker_accessor::get_gs_broker() {
  return m_broker;
}

gs::cnf::cnf_api_if* cci::cnf::gs_cci_cnf_broker_accessor::get_gs_cnf_api() {
  return dynamic_cast<gs::cnf::cnf_api_if*>(get_gs_broker());
}
