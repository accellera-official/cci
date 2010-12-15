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


#include "gs_cci_cnf_private_broker.h"


cci::cnf::gs_cci_private_broker::gs_cci_private_broker(sc_core::sc_module* owner, std::vector<const char*> pub_params) 
: gs::cnf::GCnf_private_Api(owner, pub_params) { 
  //m_gcnf_api = new gs::cnf::GCnf_private_Api(owner, pub_params);
  std::cout << "Created new PRIVATE broker for Module \""<<owner->name()<<"\" and GCnf_private_Api \""<< getName() << "\"" << std::endl;
}

cci::cnf::gs_cci_private_broker::gs_cci_private_broker(sc_core::sc_module* owner, std::vector<std::string> pub_params) 
: gs::cnf::GCnf_private_Api(owner, pub_params) { 
  //m_gcnf_api = new gs::cnf::GCnf_private_Api(owner, pub_params);
  std::cout << "Created new PRIVATE broker for Module \""<<owner->name()<<"\" and GCnf_private_Api \""<< getName() << "\"" << std::endl;
}

//cci::cnf::gs_cci_private_broker::gs_cci_private_broker(sc_core::sc_module* owner_module, const char* pub_par ...)
//: gs::cnf::GCnf_private_Api(owner_module, vector_factory(pub_par)) {
/*  std::vector<std::string> pub_param_lst;
  va_list list;
  va_start(list, pub_par);
  const char* p = pub_par;
  for(;;) { 
    if(std::string(p) == std::string(END_OF_PUBLIC_PARAM_LIST))
      break;
    pub_param_lst.push_back(p);
    p = va_arg(list, char*);
  }
  va_end(list);

  m_gcnf_api = new gs::cnf::GCnf_private_Api(owner_module, pub_param_lst);*/
//}

cci::cnf::gs_cci_private_broker::~gs_cci_private_broker() { 
  //delete m_gcnf_api; m_gcnf_api = NULL;
}

void cci::cnf::gs_cci_private_broker::set_init_value(const std::string &parname, const std::string &json_value, const std::string& originator) {
  // TODO: use JSON
  if ( !gs::cnf::GCnf_private_Api::setInitValue(parname, json_value) ) {
    CCI_THROW_ERROR(cci::cnf::cci_report::set_param_failed().get_type(), "Setting initial value failed.");
  }
}
void cci::cnf::gs_cci_private_broker::set_init_value(const std::string &parname, const std::string &json_value, sc_core::sc_object* originator) {
  assert(originator);
  set_init_value(parname, json_value, originator->name());
}

void cci::cnf::gs_cci_private_broker::lock_init_value(const std::string &parname, const std::string& originator) {
  if ( !gs::cnf::GCnf_private_Api::lockInitValue(parname) ) {
    CCI_THROW_ERROR(cci::cnf::cci_report::set_param_failed().get_type(), "Locking initial value failed.");
  }
}
void cci::cnf::gs_cci_private_broker::lock_init_value(const std::string &parname, sc_core::sc_object* originator) {
  assert(originator);
  lock_init_value(parname, originator->name());
}

const std::string cci::cnf::gs_cci_private_broker::get_json_string(const std::string &parname, const std::string& originator) {
  // TODO: use JSON
  return gs::cnf::GCnf_private_Api::getValue(parname);
}
const std::string cci::cnf::gs_cci_private_broker::get_json_string(const std::string &parname, sc_core::sc_object* originator) {
  assert(originator);
  return get_json_string(parname, originator->name());
}

const std::string cci::cnf::gs_cci_private_broker::get_json_string_keep_unused(const std::string &parname, const std::string& originator) {
  // TODO: use JSON
  return gs::cnf::GCnf_private_Api::getValue(parname, "", true);
}
const std::string cci::cnf::gs_cci_private_broker::get_json_string_keep_unused(const std::string &parname, sc_core::sc_object* originator) {
  assert(originator);
  return get_json_string_keep_unused(parname, originator->name());
}

cci::cnf::cci_base_param* cci::cnf::gs_cci_private_broker::get_param(const std::string &parname, const std::string& originator) {
  std::map<std::string,cci_base_param*>::iterator iter = m_mirrored_registry.find(parname);
  if( iter != m_mirrored_registry.end() )
    return iter->second;
  else {
    // and get from hierarchically upper broker
    cci_cnf_broker_if* a = cci_broker_manager::search_for_broker(owner_module->get_parent_object());
    cci::cnf::gs_cci_cnf_broker_accessor* b = dynamic_cast<cci::cnf::gs_cci_cnf_broker_accessor*>(a);
    cci::cnf::gs_cci_cnf_broker_if* gs_br = b->get_gs_broker();
    assert(gs_br && gs_br != this);
    return gs_br->get_param(parname, originator);      
  }
  return NULL;
}
cci::cnf::cci_base_param* cci::cnf::gs_cci_private_broker::get_param(const std::string &parname, sc_core::sc_object* originator) {
  assert(originator);
  return get_param(parname, originator->name());
}

bool cci::cnf::gs_cci_private_broker::exists_param(const std::string &parname, const std::string& originator) {
  return gs::cnf::GCnf_private_Api::existsParam(parname);
}
bool cci::cnf::gs_cci_private_broker::exists_param(const std::string &parname, sc_core::sc_object* originator) {
  assert(originator);
  return exists_param(parname, originator->name());
}

bool cci::cnf::gs_cci_private_broker::is_used(const std::string &parname, const std::string& originator) {
  return gs::cnf::GCnf_private_Api::is_used(parname);
}
bool cci::cnf::gs_cci_private_broker::is_used(const std::string &parname, sc_core::sc_object* originator) {
  assert(originator);
  return is_used(parname, originator->name());
}

const std::vector<std::string> cci::cnf::gs_cci_private_broker::get_param_list(const std::string& originator) {
  return gs::cnf::GCnf_private_Api::getParamList();
}
const std::vector<std::string> cci::cnf::gs_cci_private_broker::get_param_list(sc_core::sc_object* originator) {
  assert(originator);
  return get_param_list(originator->name());
}

cci::shared_ptr<cci::cnf::callb_adapt_b> cci::cnf::gs_cci_private_broker::register_callback(const std::string& parname, const callback_type type, cci::shared_ptr<cci::cnf::callb_adapt_b> callb, const std::string& originator) {
  // TODO
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return callb;
}
cci::shared_ptr<cci::cnf::callb_adapt_b> cci::cnf::gs_cci_private_broker::register_callback(const std::string& parname, const callback_type type, cci::shared_ptr<cci::cnf::callb_adapt_b> callb, sc_core::sc_object* originator) {
  assert(originator);
  return register_callback(parname, type, callb, originator->name());
}

void cci::cnf::gs_cci_private_broker::unregister_all_callbacks(void* observer, const std::string& originator) {
  // TODO
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
}
void cci::cnf::gs_cci_private_broker::unregister_all_callbacks(void* observer, sc_core::sc_object* originator) {
  assert(originator);
  return unregister_all_callbacks(observer, originator->name());
}

bool cci::cnf::gs_cci_private_broker::unregister_param_callback(cci::cnf::callb_adapt_b* callb, const std::string& originator) {
  // TODO
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false;
}
bool cci::cnf::gs_cci_private_broker::unregister_param_callback(cci::cnf::callb_adapt_b* callb, sc_core::sc_object* originator) {
  assert(originator);
  return unregister_param_callback(callb, originator->name());
}

bool cci::cnf::gs_cci_private_broker::has_callbacks(const std::string& parname, const std::string& originator) {
  // TODO
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false;
}
bool cci::cnf::gs_cci_private_broker::has_callbacks(const std::string& parname, sc_core::sc_object* originator) {
  assert(originator);
  return has_callbacks(parname, originator->name());
}

void cci::cnf::gs_cci_private_broker::add_param(cci_base_param* par, const std::string& originator) {
  //cci::cnf::cci_base_param* bpar = dynamic_cast<cci::cnf::cci_base_param*>(par);
  //assert(bpar != NULL && "This should never happen, a cci_base_param shall be a cci_base_param!");
  
  if (public_params.find(par->get_name()) == public_params.end()) {
    m_mirrored_registry.insert(std::pair<std::string, cci_base_param*>(par->get_name(), par));
    //std::cout << name() << " (gs_cci_cnf_broker) add param to PRIVATE broker " << par->get_name() << std::endl;
  // or add to hierarchically upper broker if public
  } else {
    cci_cnf_broker_if* a = cci_broker_manager::search_for_broker(owner_module->get_parent_object());
    cci::cnf::gs_cci_cnf_broker_accessor* b = dynamic_cast<cci::cnf::gs_cci_cnf_broker_accessor*>(a);
    cci::cnf::gs_cci_cnf_broker_if* gs_br = b->get_gs_broker();
    assert(gs_br && gs_br != this);
    gs_br->add_param(par, originator);
  }
}
void cci::cnf::gs_cci_private_broker::add_param(cci_base_param* par, sc_core::sc_object* originator) {
  assert(originator);
  add_param(par, originator->name());
}

void cci::cnf::gs_cci_private_broker::remove_param(cci::cnf::cci_base_param* par, const std::string& originator) {
  if (m_mirrored_registry.find(par->get_name()) != m_mirrored_registry.end())
    m_mirrored_registry.erase(par->get_name());
  // or remove from hierarchically upper broker if public
  else {
    cci_cnf_broker_if* a = cci_broker_manager::search_for_broker(owner_module->get_parent_object());
    cci::cnf::gs_cci_cnf_broker_accessor* b = dynamic_cast<cci::cnf::gs_cci_cnf_broker_accessor*>(a);
    cci::cnf::gs_cci_cnf_broker_if* gs_br = b->get_gs_broker();
    assert(gs_br && gs_br != this);
    gs_br->remove_param(par, originator);      
  }
}
void cci::cnf::gs_cci_private_broker::remove_param(cci::cnf::cci_base_param* par, sc_core::sc_object* originator) {
  assert(originator);
  remove_param(par, originator->name());
}

const std::vector<std::string> cci::cnf::gs_cci_private_broker::get_param_list(const std::string& pattern, const std::string& originator) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "pattern limited to GreenConfig capabilities");
  return gs::cnf::GCnf_private_Api::getParamList(pattern);
}
const std::vector<std::string> cci::cnf::gs_cci_private_broker::get_param_list(const std::string& pattern, sc_core::sc_object* originator) {
  assert(originator);
  return get_param_list(pattern, originator->name());
}

const std::vector<cci::cnf::cci_base_param*> cci::cnf::gs_cci_private_broker::get_params(const std::string& pattern, const std::string& originator) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "pattern limited to GreenConfig capabilities");
  std::vector<cci::cnf::cci_base_param*> parvec;
  std::vector<std::string> strvec;
  strvec = get_param_list(pattern);
  std::vector<std::string>::iterator striter;
  for (striter = strvec.begin(); striter != strvec.end(); striter++) {
    cci::cnf::cci_base_param* p = get_param(*striter, originator);
    if (p) parvec.push_back(p);
  }
  return parvec;
}
const std::vector<cci::cnf::cci_base_param*> cci::cnf::gs_cci_private_broker::get_params(const std::string& pattern, sc_core::sc_object* originator) {
  assert(originator);
  return get_params(pattern, originator->name());
}

gs::cnf::cnf_api_if* cci::cnf::gs_cci_private_broker::get_gs_cnf_api() {
  return this;
}

