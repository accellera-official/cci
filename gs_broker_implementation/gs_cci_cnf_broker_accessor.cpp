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


cci::cci_broker_if& cci::gs_cci_cnf_broker_accessor::get_accessor(const cci_originator& originator) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  cci::cci_broker_if &broker = m_broker->get_accessor(originator);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return broker;
}

cci::gs_cci_cnf_broker_accessor::gs_cci_cnf_broker_accessor(const cci_originator& originator, cci::cci_broker_if& orig_broker) 
: m_broker(NULL)
, m_originator(originator) 
{ 
  assert(!dynamic_cast<gs_cci_cnf_broker_accessor*>(&orig_broker)       && "The broker given to this contructor shall not be another accessor but the original broker!");
  assert(!dynamic_cast<gs_cci_private_broker_accessor*>(&orig_broker)   && "The broker given to this contructor shall not be another (private) accessor but the original broker!");
  gs_cci_cnf_broker_if *gs_pbr = dynamic_cast<gs_cci_cnf_broker_if*>(&orig_broker);
  assert(gs_pbr && "Here another gs implementation is expected! Mixing broker implementations is not supported!!");
  m_broker = gs_pbr;
  CCI_CNF_IMPL_DUMP("Created new cci broker accessor for originator \""<< m_originator.name() << "\"" << 
                    " using existing "<< (is_private_broker() ? "PRIVATE " : "") <<"broker \""<< name() << "\"");
  assert(m_broker && "The given broker shall be a (non-)private gs_cci_cnf_broker_if (which means either a gs_cci_cnf_broker or a gs_cci_private_broker)!");
}

cci::gs_cci_cnf_broker_accessor::~gs_cci_cnf_broker_accessor() { 
  // delete param accessors
  for ( param_accessor_map::iterator it = m_param_accessor_map.begin() ; it != m_param_accessor_map.end(); it++ ) {
    delete it->second;
  }
}

const std::string &cci::gs_cci_cnf_broker_accessor::name() const {
  return m_broker->name();
}

void cci::gs_cci_cnf_broker_accessor::json_deserialize_initial_value(const std::string &parname, const std::string &json_value) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_broker->json_deserialize_initial_value(parname, json_value);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

const std::string cci::gs_cci_cnf_broker_accessor::json_serialize_initial_value(const std::string &parname) {
  return m_broker->json_serialize_initial_value(parname);
}

void cci::gs_cci_cnf_broker_accessor::lock_initial_value(const std::string &parname) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_broker->lock_initial_value(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

const std::string cci::gs_cci_cnf_broker_accessor::json_serialize(const std::string &parname) {
  // TODO: use JSON
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  std::string ret = m_broker->json_serialize(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

const std::string cci::gs_cci_cnf_broker_accessor::json_serialize_keep_unused(const std::string &parname) {
  // TODO: use JSON
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::string& ret = m_broker->json_serialize_keep_unused(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

cci::cci_base_param* cci::gs_cci_cnf_broker_accessor::get_param(const std::string &parname) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  cci::cci_base_param* orig_param;
  orig_param = m_broker->get_param(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  if (orig_param) return get_param_accessor(*orig_param);
  else return NULL;
}

bool cci::gs_cci_cnf_broker_accessor::param_exists(const std::string &parname) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = m_broker->param_exists(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

bool cci::gs_cci_cnf_broker_accessor::is_used(const std::string &parname) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = m_broker->is_used(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

const std::vector<std::string> cci::gs_cci_cnf_broker_accessor::get_param_list() {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::vector<std::string>& ret = m_broker->get_param_list();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

cci::shared_ptr<cci::callb_adapt> cci::gs_cci_cnf_broker_accessor::register_callback(const std::string& parname, const callback_type type, cci::shared_ptr<cci::callb_adapt> callb) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const cci::shared_ptr<cci::callb_adapt>& ret = m_broker->register_callback(parname, type, callb);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

void cci::gs_cci_cnf_broker_accessor::unregister_all_callbacks(void* observer) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_broker->unregister_all_callbacks(observer);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

bool cci::gs_cci_cnf_broker_accessor::unregister_callback(callb_adapt* callb) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = m_broker->unregister_callback(callb);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

bool cci::gs_cci_cnf_broker_accessor::has_callbacks(const std::string& parname) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = m_broker->has_callbacks(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

void cci::gs_cci_cnf_broker_accessor::add_param(cci_base_param* par) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_broker->add_param(par);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

void cci::gs_cci_cnf_broker_accessor::remove_param(cci_base_param* par) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_broker->remove_param(par);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

const std::vector<std::string> cci::gs_cci_cnf_broker_accessor::get_param_list(const std::string& pattern) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::vector<std::string>& ret = m_broker->get_param_list(pattern);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

const std::vector<cci::cci_base_param*> cci::gs_cci_cnf_broker_accessor::get_params(const std::string& pattern) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  std::vector<cci::cci_base_param*> orig_params = m_broker->get_params(pattern);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  std::vector<cci::cci_base_param*> param_accessors;
  for (std::vector<cci::cci_base_param*>::iterator it=orig_params.begin(); it != orig_params.end(); ++it) {
    cci::cci_base_param *p = *it;
    if (p) param_accessors.push_back(get_param_accessor(*p));
  }
  return param_accessors;
}

bool cci::gs_cci_cnf_broker_accessor::is_private_broker() const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const bool& ret = m_broker->is_private_broker();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

const cci::cci_originator* cci::gs_cci_cnf_broker_accessor::get_originator() const {
  return &m_originator;
}

const cci::cci_originator* cci::gs_cci_cnf_broker_accessor::get_latest_write_originator(const std::string &parname) const {
  return m_broker->get_latest_write_originator(parname);
}

cci::gs_cci_cnf_broker_if* cci::gs_cci_cnf_broker_accessor::get_gs_broker() {
  return m_broker;
}

gs::cnf::cnf_api_if* cci::gs_cci_cnf_broker_accessor::get_gs_cnf_api() {
  return dynamic_cast<gs::cnf::cnf_api_if*>(get_gs_broker());
}

cci::cci_base_param* cci::gs_cci_cnf_broker_accessor::get_param_accessor(cci::cci_base_param& orig_param) {
  assert (!orig_param.is_accessor() && "This function shall be called with an original parameter to return the related accessor parameter!");
  cci::cci_base_param* param_accessor = NULL;  
  
  // Choose or create the accessor for this parameter
  // Choose
  param_accessor_map::iterator iter_acc = m_param_accessor_map.find(orig_param.get_name());
  if( iter_acc != m_param_accessor_map.end() ) {
    param_accessor = iter_acc->second;
  }
  // or create
  else {
    const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
    param_accessor = orig_param.create_accessor(m_originator);
    cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
    assert(param_accessor!=NULL);
    bool new_element = m_param_accessor_map.insert(std::pair<std::string, cci_base_param*>(orig_param.get_name(), param_accessor)).second;
    assert(new_element && "The same accessor parameter had been added twice! Must not happen, to be caught by other if branch.");
  }    
  
  assert(param_accessor->is_accessor() && "The returned object must be a parameter accessor");
  return param_accessor;         
}

