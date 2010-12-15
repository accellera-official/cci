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


#include "gs_cci_cnf_private_broker_accessor.h"


cci::cnf::gs_cci_private_broker_accessor::gs_cci_private_broker_accessor(sc_core::sc_module* owner, std::vector<const char*> pub_params) 
: m_originator_obj(owner)
, m_originator_str(owner->name())
{ 
  m_broker = new gs_cci_private_broker(owner, pub_params);
}

cci::cnf::gs_cci_private_broker_accessor::gs_cci_private_broker_accessor(sc_core::sc_module* owner, std::vector<std::string> pub_params) 
: m_originator_obj(owner)
, m_originator_str(owner->name())
{
  m_broker = new gs_cci_private_broker(owner, pub_params);
}

cci::cnf::gs_cci_private_broker_accessor::~gs_cci_private_broker_accessor() {
  delete m_broker; m_broker = NULL;
}

void cci::cnf::gs_cci_private_broker_accessor::set_init_value(const std::string &parname, const std::string &json_value) {
  if (m_originator_obj)
    m_broker->set_init_value(parname, json_value, m_originator_obj);
  else
    m_broker->set_init_value(parname, json_value, m_originator_str);
}

void cci::cnf::gs_cci_private_broker_accessor::lock_init_value(const std::string &parname) {
  if (m_originator_obj)
    m_broker->lock_init_value(parname, m_originator_obj);
  else
    m_broker->lock_init_value(parname, m_originator_str);
}

const std::string cci::cnf::gs_cci_private_broker_accessor::get_json_string(const std::string &parname) {
  // TODO: use JSON
  if (m_originator_obj)
    return m_broker->get_json_string(parname, m_originator_obj);
  else
    return m_broker->get_json_string(parname, m_originator_str);
}

const std::string cci::cnf::gs_cci_private_broker_accessor::get_json_string_keep_unused(const std::string &parname) {
  // TODO: use JSON
  if (m_originator_obj)
    return m_broker->get_json_string_keep_unused(parname, m_originator_obj);
  else
    return m_broker->get_json_string_keep_unused(parname, m_originator_str);
  
}

cci::cnf::cci_base_param* cci::cnf::gs_cci_private_broker_accessor::get_param(const std::string &parname) {
  if (m_originator_obj)
    return m_broker->get_param(parname, m_originator_obj);
  else
    return m_broker->get_param(parname, m_originator_str);
}

bool cci::cnf::gs_cci_private_broker_accessor::exists_param(const std::string &parname) {
  if (m_originator_obj)
    return m_broker->exists_param(parname, m_originator_obj);
  else
    return m_broker->exists_param(parname, m_originator_str);
}

bool cci::cnf::gs_cci_private_broker_accessor::is_used(const std::string &parname) {
  if (m_originator_obj)
    return m_broker->is_used(parname, m_originator_obj);
  else
    return m_broker->is_used(parname, m_originator_str);
}

const std::vector<std::string> cci::cnf::gs_cci_private_broker_accessor::get_param_list() {
  if (m_originator_obj)
    return m_broker->get_param_list(m_originator_obj);
  else
    return m_broker->get_param_list(m_originator_str);
}

cci::shared_ptr<cci::cnf::callb_adapt_b> cci::cnf::gs_cci_private_broker_accessor::register_callback(const std::string& parname, const callback_type type, cci::shared_ptr<cci::cnf::callb_adapt_b> callb) {
  if (m_originator_obj)
    return m_broker->register_callback(parname, type, callb, m_originator_obj);
  else
    return m_broker->register_callback(parname, type, callb, m_originator_str);
}

void cci::cnf::gs_cci_private_broker_accessor::unregister_all_callbacks(void* observer) {
  if (m_originator_obj)
    return m_broker->unregister_all_callbacks(observer, m_originator_obj);
  else
    return m_broker->unregister_all_callbacks(observer, m_originator_str);
}

bool cci::cnf::gs_cci_private_broker_accessor::unregister_param_callback(callb_adapt_b* callb) {
  if (m_originator_obj)
    return m_broker->unregister_param_callback(callb, m_originator_obj);
  else
    return m_broker->unregister_param_callback(callb, m_originator_str);
}

bool cci::cnf::gs_cci_private_broker_accessor::has_callbacks(const std::string& parname) {
  if (m_originator_obj)
    return m_broker->has_callbacks(parname, m_originator_obj);
  else
    return m_broker->has_callbacks(parname, m_originator_str);
}

void cci::cnf::gs_cci_private_broker_accessor::add_param(cci_base_param* par) {
  if (m_originator_obj)
    return m_broker->add_param(par, m_originator_obj);
  else
    return m_broker->add_param(par, m_originator_str);
}

void cci::cnf::gs_cci_private_broker_accessor::remove_param(cci_base_param* par) {
  if (m_originator_obj)
    return m_broker->remove_param(par, m_originator_obj);
  else
    return m_broker->remove_param(par, m_originator_str);
}

const std::vector<std::string> cci::cnf::gs_cci_private_broker_accessor::get_param_list(const std::string& pattern) {
  if (m_originator_obj)
    return m_broker->get_param_list(pattern, m_originator_obj);
  else
    return m_broker->get_param_list(pattern, m_originator_str);
}

const std::vector<cci::cnf::cci_base_param*> cci::cnf::gs_cci_private_broker_accessor::get_params(const std::string& pattern) {
  if (m_originator_obj)
    return m_broker->get_params(pattern, m_originator_obj);
  else
    return m_broker->get_params(pattern, m_originator_str);
}

const std::string& cci::cnf::gs_cci_private_broker_accessor::get_originator() {
  return m_originator_str;
}

cci::cnf::gs_cci_cnf_broker_if* cci::cnf::gs_cci_private_broker_accessor::get_gs_broker() {
  return m_broker;
}

gs::cnf::cnf_api_if* cci::cnf::gs_cci_private_broker_accessor::get_gs_cnf_api() {
  return dynamic_cast<gs::cnf::cnf_api_if*>(get_gs_broker());
}
