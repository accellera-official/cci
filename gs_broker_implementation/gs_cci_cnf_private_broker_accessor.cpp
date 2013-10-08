// LICENSETEXT
//
//   Copyright (C) 2010-2011 : GreenSocs Ltd
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


cci::cnf::cci_cnf_broker_if& cci::cnf::gs_cci_private_broker_accessor::get_accessor(const cci_originator& originator) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  cci::cnf::cci_cnf_broker_if& broker = m_broker->get_accessor(originator);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return broker;
}

/*cci::cnf::gs_cci_private_broker_accessor::gs_cci_private_broker_accessor(sc_core::sc_module& owner, std::vector<const char*> pub_params, const cci_originator& originator) 
: m_originator(originator)
{
  if (dynamic_cast<sc_core::sc_object*>(&owner) != originator.get()) {
    SC_REPORT_WARNING(__CCI_SC_REPORT_MSG_TYPE_PREFIX__, "When creating a private broker, the given originator should be the same as the owner.");
  }
  m_broker = new gs_cci_private_broker(owner.name(), owner, pub_params);
}*/

cci::cnf::gs_cci_private_broker_accessor::gs_cci_private_broker_accessor(sc_core::sc_module& owner, std::vector<std::string> pub_params) 
: m_originator(cci::cnf::cci_originator(owner))
{
  //if (dynamic_cast<sc_core::sc_object*>(&owner) != originator.get())
  //  SC_REPORT_WARNING(__CCI_SC_REPORT_MSG_TYPE_PREFIX__, "When creating a private broker, the given originator should be the same as the owner.");
  m_broker = new gs_cci_private_broker(owner.name(), owner, pub_params);
}

cci::cnf::gs_cci_private_broker_accessor::~gs_cci_private_broker_accessor() {
  delete m_broker; m_broker = NULL;
}

const std::string &cci::cnf::gs_cci_private_broker_accessor::name() const {
  return m_broker->name();
}

void cci::cnf::gs_cci_private_broker_accessor::json_deserialize_initial_value(const std::string &parname, const std::string &json_value) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_broker->json_deserialize_initial_value(parname, json_value);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

void cci::cnf::gs_cci_private_broker_accessor::lock_initial_value(const std::string &parname) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_broker->lock_initial_value(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

const std::string cci::cnf::gs_cci_private_broker_accessor::json_serialize(const std::string &parname) {
  // TODO: use JSON
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::string& ret = m_broker->json_serialize(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

const std::string cci::cnf::gs_cci_private_broker_accessor::json_serialize_keep_unused(const std::string &parname) {
  // TODO: use JSON
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::string& ret = m_broker->json_serialize_keep_unused(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

cci::cnf::cci_base_param* cci::cnf::gs_cci_private_broker_accessor::get_param(const std::string &parname) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  cci::cnf::cci_base_param* ret = m_broker->get_param(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

bool cci::cnf::gs_cci_private_broker_accessor::param_exists(const std::string &parname) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = m_broker->param_exists(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

bool cci::cnf::gs_cci_private_broker_accessor::is_used(const std::string &parname) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = m_broker->is_used(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

const std::vector<std::string> cci::cnf::gs_cci_private_broker_accessor::get_param_list() {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::vector<std::string>& ret = m_broker->get_param_list();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

cci::shared_ptr<cci::cnf::callb_adapt> cci::cnf::gs_cci_private_broker_accessor::register_callback(const std::string& parname, const callback_type type, cci::shared_ptr<cci::cnf::callb_adapt> callb) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const cci::shared_ptr<cci::cnf::callb_adapt> ret = m_broker->register_callback(parname, type, callb);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

void cci::cnf::gs_cci_private_broker_accessor::unregister_all_callbacks(void* observer) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_broker->unregister_all_callbacks(observer);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

bool cci::cnf::gs_cci_private_broker_accessor::unregister_callback(callb_adapt* callb) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = m_broker->unregister_callback(callb);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

bool cci::cnf::gs_cci_private_broker_accessor::has_callbacks(const std::string& parname) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = m_broker->has_callbacks(parname);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

void cci::cnf::gs_cci_private_broker_accessor::add_param(cci_base_param* par) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_broker->add_param(par);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

void cci::cnf::gs_cci_private_broker_accessor::remove_param(cci_base_param* par) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_broker->remove_param(par);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

const std::vector<std::string> cci::cnf::gs_cci_private_broker_accessor::get_param_list(const std::string& pattern) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::vector<std::string> ret = m_broker->get_param_list(pattern);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

const std::vector<cci::cnf::cci_base_param*> cci::cnf::gs_cci_private_broker_accessor::get_params(const std::string& pattern) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::vector<cci::cnf::cci_base_param*> ret = m_broker->get_params(pattern);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

bool cci::cnf::gs_cci_private_broker_accessor::is_private_broker() const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const bool& ret = m_broker->is_private_broker();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

const cci::cnf::cci_originator* cci::cnf::gs_cci_private_broker_accessor::get_originator() const {
  return &m_originator;
}

const cci::cnf::cci_originator* cci::cnf::gs_cci_private_broker_accessor::get_latest_write_originator(const std::string &parname) const {
  return m_broker->get_latest_write_originator(parname);
}

cci::cnf::gs_cci_cnf_broker_if* cci::cnf::gs_cci_private_broker_accessor::get_gs_broker() {
  return m_broker;
}

gs::cnf::cnf_api_if* cci::cnf::gs_cci_private_broker_accessor::get_gs_cnf_api() {
  return dynamic_cast<gs::cnf::cnf_api_if*>(get_gs_broker());
}
