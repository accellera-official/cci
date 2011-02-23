// LICENSETEXT
//
//   Copyright (C) 2009-2011 : GreenSocs Ltd
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


cci::cnf::gs_cci_cnf_broker_accessor::gs_cci_cnf_broker_accessor(const cci_originator& originator) 
: m_broker(NULL)
, m_originator(originator) 
{ 
  // This will return the appropriate private or non-private broker
  cci::cnf::cci_cnf_broker_if *br = cci::cnf::cci_broker_manager::search_for_broker(originator.get()); // TODO
  gs_cci_cnf_broker_accessor *gs_br = dynamic_cast<gs_cci_cnf_broker_accessor*>(br);
  if (gs_br) {
    m_broker = gs_br->get_gs_broker();
    CCI_CNF_IMPL_DUMP("Created new cci broker accessor \""<< m_originator.name() << "\"" << 
                      " using existing broker \""<< dynamic_cast<gs::ctr::command_if*>(m_broker)->getName() << "\"");
  }
  else {
    gs_cci_private_broker_accessor *gs_pbr = dynamic_cast<gs_cci_private_broker_accessor*>(br);
    if (gs_pbr) {
      m_broker = gs_pbr->get_gs_broker();
      CCI_CNF_IMPL_DUMP("Created new cci broker accessor \""<< m_originator.name() << "\"" << 
                        " using existing PRIVATE broker \""<< dynamic_cast<gs::ctr::command_if*>(m_broker)->getName() << "\"");
    }
  }
  assert(m_broker && "Must be a (non-)private gs_cci_cnf_broker_if (which means eather a gs_cci_cnf_broker or a gs_cci_private_broker)!");
}

cci::cnf::gs_cci_cnf_broker_accessor::gs_cci_cnf_broker_accessor(bool called_internally_for_creating_global_broker) 
: m_broker(NULL)
, m_originator(GS_CCI_IMPL_UNKNOWN_ORIGINATOR_STRING)
{ 
  // This will return the appropriate (non-private) broker
  cci::cnf::gs_cci_cnf_broker *br = new cci::cnf::gs_cci_cnf_broker(std::string(GS_CCI_IMPL_UNKNOWN_ORIGINATOR_STRING));
  m_broker = dynamic_cast<gs_cci_cnf_broker_if*>(br);
  assert(m_broker && "Must be a (non-)private gs_cci_cnf_broker_if (which means eather a gs_cci_cnf_broker or a gs_cci_private_broker)!");
  CCI_CNF_IMPL_DUMP("Created global cci broker accessor \""<< m_originator.name() << "\"");
}

cci::cnf::gs_cci_cnf_broker_accessor::~gs_cci_cnf_broker_accessor() { 
  // delete param accessors
  for ( param_accessor_map::iterator it = m_param_accessor_map.begin() ; it != m_param_accessor_map.end(); it++ ) {
    delete it->second;
  }
}

void cci::cnf::gs_cci_cnf_broker_accessor::set_init_value(const std::string &parname, const std::string &json_value) {
  cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_broker->set_init_value(parname, json_value);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

void cci::cnf::gs_cci_cnf_broker_accessor::lock_init_value(const std::string &parname) {
  cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_broker->lock_init_value(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

const std::string cci::cnf::gs_cci_cnf_broker_accessor::get_json_string(const std::string &parname) {
  // TODO: use JSON
  cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  std::string ret = m_broker->get_json_string(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

const std::string cci::cnf::gs_cci_cnf_broker_accessor::get_json_string_keep_unused(const std::string &parname) {
  // TODO: use JSON
  cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::string& ret = m_broker->get_json_string_keep_unused(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

cci::cnf::cci_base_param* cci::cnf::gs_cci_cnf_broker_accessor::get_param(const std::string &parname) {
  cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  cci::cnf::cci_base_param* orig_param;
  orig_param = m_broker->get_param(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  if (orig_param) return get_param_accessor(*orig_param);
  else return NULL;
}

bool cci::cnf::gs_cci_cnf_broker_accessor::exists_param(const std::string &parname) {
  cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = m_broker->exists_param(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

bool cci::cnf::gs_cci_cnf_broker_accessor::is_used(const std::string &parname) {
  cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = m_broker->is_used(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

const std::vector<std::string> cci::cnf::gs_cci_cnf_broker_accessor::get_param_list() {
  cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::vector<std::string>& ret = m_broker->get_param_list();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

cci::shared_ptr<cci::cnf::callb_adapt_b> cci::cnf::gs_cci_cnf_broker_accessor::register_callback(const std::string& parname, const callback_type type, cci::shared_ptr<cci::cnf::callb_adapt_b> callb) {
  cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const cci::shared_ptr<cci::cnf::callb_adapt_b>& ret = m_broker->register_callback(parname, type, callb);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

void cci::cnf::gs_cci_cnf_broker_accessor::unregister_all_callbacks(void* observer) {
  cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_broker->unregister_all_callbacks(observer);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

bool cci::cnf::gs_cci_cnf_broker_accessor::unregister_param_callback(callb_adapt_b* callb) {
  cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = m_broker->unregister_param_callback(callb);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

bool cci::cnf::gs_cci_cnf_broker_accessor::has_callbacks(const std::string& parname) {
  cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = m_broker->has_callbacks(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

void cci::cnf::gs_cci_cnf_broker_accessor::add_param(cci_base_param* par) {
  cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_broker->add_param(par);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

void cci::cnf::gs_cci_cnf_broker_accessor::remove_param(cci_base_param* par) {
  cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_broker->remove_param(par);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

const std::vector<std::string> cci::cnf::gs_cci_cnf_broker_accessor::get_param_list(const std::string& pattern) {
  cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::vector<std::string>& ret = m_broker->get_param_list(pattern);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

const std::vector<cci::cnf::cci_base_param*> cci::cnf::gs_cci_cnf_broker_accessor::get_params(const std::string& pattern) {
  std::vector<cci::cnf::cci_base_param*> orig_params;
  cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  orig_params = m_broker->get_params(pattern);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  std::vector<cci::cnf::cci_base_param*> param_accessors;
  for (std::vector<cci::cnf::cci_base_param*>::iterator it; it != orig_params.end(); it++) {
    param_accessors.push_back(get_param_accessor(**it));
  }
  return param_accessors;
}

cci::cnf::cci_originator* cci::cnf::gs_cci_cnf_broker_accessor::get_originator() {
  return &m_originator;
}

cci::cnf::gs_cci_cnf_broker_if* cci::cnf::gs_cci_cnf_broker_accessor::get_gs_broker() {
  return m_broker;
}

gs::cnf::cnf_api_if* cci::cnf::gs_cci_cnf_broker_accessor::get_gs_cnf_api() {
  return dynamic_cast<gs::cnf::cnf_api_if*>(get_gs_broker());
}

cci::cnf::cci_base_param* cci::cnf::gs_cci_cnf_broker_accessor::get_param_accessor(cci::cnf::cci_base_param& orig_param) {
  assert (!orig_param.is_accessor() && "This function shall be called with an original parameter to return the related accessor parameter!");
  cci::cnf::cci_base_param* param_accessor = NULL;  
  
  // Choose or create the accessor for this parameter
  // Choose
  param_accessor_map::iterator iter_acc = m_param_accessor_map.find(orig_param.get_name());
  if( iter_acc != m_param_accessor_map.end() ) {
    param_accessor = iter_acc->second;
  }
  // or create
  else {
    cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
    param_accessor = orig_param.create_accessor(m_originator);
    cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
    assert(param_accessor!=NULL);
    bool new_element = m_param_accessor_map.insert(std::pair<std::string, cci_base_param*>(orig_param.get_name(), param_accessor)).second;
    assert(new_element && "The same accessor parameter had been added twice! Must not happen, to be caught by other if branch.");
  }    
  
  assert(param_accessor->is_accessor() && "The returned object must be a parameter accessor");
  return param_accessor;         
}

