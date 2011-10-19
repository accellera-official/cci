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


#include "gs_cci_cnf_private_broker.h"


/*cci::cnf::gs_cci_private_broker::gs_cci_private_broker(const char* name, sc_core::sc_module& owner, std::vector<const char*> pub_params) 
: gs::cnf::GCnf_private_Api(&owner, pub_params)
, m_upper_broker(&cci_broker_manager::get_current_broker(cci_originator(owner))) 
//, m_upper_broker(&cci_broker_manager::get_current_parent_broker(cci_originator(owner))) // at this point in time this is not yet pushed to stack, so use the top of stack as upper broker
, m_name(cci::cci_gen_unique_name(name))
{ 
  //m_gcnf_api = new gs::cnf::GCnf_private_Api(&owner, pub_params);
  std::cout << "Created new PRIVATE \""<<m_name<<"\" broker for Module \""<<owner.name()<<"\" (with GCnf_private_Api \""<< getName() << "\")" << std::endl;
}*/

cci::cnf::gs_cci_private_broker::gs_cci_private_broker(const char* name, sc_core::sc_module& owner, std::vector<std::string> pub_params) 
: gs::cnf::GCnf_private_Api(&owner, pub_params)
, m_upper_broker(&cci_broker_manager::get_current_broker(cci_originator(owner))) 
//, m_upper_broker(&cci_broker_manager::get_current_parent_broker(cci_originator(owner))) // at this point in time this is not yet pushed to stack, so use the top of stack as upper broker 
, m_name(cci::cci_gen_unique_name(name))
{ 
  //m_gcnf_api = new gs::cnf::GCnf_private_Api(&owner, pub_params);
  CCI_CNF_DUMP("Created new PRIVATE broker \""<<m_name<<"\" for Module \""<<owner.name()<<"\" (with GCnf_private_Api \""<< getName() << "\")");
  CCI_CNF_DUMP("  using broker \"" << m_upper_broker->name() << "\" to access upper hierarchy");
}

//cci::cnf::gs_cci_private_broker::gs_cci_private_broker(const char* name, sc_core::sc_module& owner_module, const char* pub_par ...)
//: gs::cnf::GCnf_private_Api(owner_module, vector_factory(pub_par))
//, m_name(cci::cci_gen_unique_name(name)) {
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

const char* cci::cnf::gs_cci_private_broker::name() const {
  return m_name.c_str();
}

void cci::cnf::gs_cci_private_broker::set_init_value(const std::string &parname, const std::string &json_value) {
  // TODO: use JSON
  if ( !gs::cnf::GCnf_private_Api::setInitValue(parname, json_value) ) {
    cci_report_handler::set_param_failed("Setting initial value failed.");
  } else {
    m_implicit_originator_map.insert( std::pair<std::string, cci::cnf::cci_originator>(parname, *cci_originator::get_global_originator()));
  }
}

const cci::cnf::cci_originator* cci::cnf::gs_cci_private_broker::get_latest_write_originator(const std::string &parname) const {
  cci::cnf::cci_base_param* p = get_param_const(parname);
  if (p) {
    if (p->get_latest_write_originator()) 
      return p->get_latest_write_originator();
  }
  implicitOriginatorMap::const_iterator it;
  it = m_implicit_originator_map.find(parname);
  if (it != m_implicit_originator_map.end()) {
    return &(it->second);
  }
  return NULL;
}

void cci::cnf::gs_cci_private_broker::lock_init_value(const std::string &parname) {
  if ( !gs::cnf::GCnf_private_Api::lockInitValue(parname) ) {
    cci_report_handler::set_param_failed("Locking initial value failed.");
  }
}

const std::string cci::cnf::gs_cci_private_broker::get_json_string(const std::string &parname) {
  // TODO: use JSON
  return gs::cnf::GCnf_private_Api::getValue(parname);
}

const std::string cci::cnf::gs_cci_private_broker::get_json_string_keep_unused(const std::string &parname) {
  // TODO: use JSON
  return gs::cnf::GCnf_private_Api::getValue(parname, "", true);
}

cci::cnf::cci_base_param* cci::cnf::gs_cci_private_broker::get_param(const std::string &parname) {
  return get_param_const(parname);
}
cci::cnf::cci_base_param* cci::cnf::gs_cci_private_broker::get_param_const(const std::string &parname) const {
  std::map<std::string,cci_base_param*>::const_iterator iter = m_mirrored_registry.find(parname);
  if( iter != m_mirrored_registry.end() )
    return iter->second;
  else {
    // and get from hierarchically upper broker
    //cci_cnf_broker_if* a = cci_broker_manager::get_current_broker(owner_module->get_parent_object());
    cci::cnf::gs_cci_cnf_broker_accessor* b = dynamic_cast<cci::cnf::gs_cci_cnf_broker_accessor*>(m_upper_broker);
    cci::cnf::gs_cci_cnf_broker_if* gs_br = b->get_gs_broker();
    assert(gs_br && gs_br != this);
    return gs_br->get_param(parname);      
  }
  return NULL;
}

bool cci::cnf::gs_cci_private_broker::exists_param(const std::string &parname) {
  return gs::cnf::GCnf_private_Api::existsParam(parname);
}

bool cci::cnf::gs_cci_private_broker::is_used(const std::string &parname) {
  return gs::cnf::GCnf_private_Api::is_used(parname);
}

const std::vector<std::string> cci::cnf::gs_cci_private_broker::get_param_list() {
  return gs::cnf::GCnf_private_Api::getParamList();
}

cci::shared_ptr<cci::cnf::callb_adapt> cci::cnf::gs_cci_private_broker::register_callback(const std::string& parname, const callback_type type, cci::shared_ptr<cci::cnf::callb_adapt> callb) {
  // TODO
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return callb;
}

void cci::cnf::gs_cci_private_broker::unregister_all_callbacks(void* observer) {
  // TODO
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
}

bool cci::cnf::gs_cci_private_broker::unregister_param_callback(cci::cnf::callb_adapt* callb) {
  // TODO
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false;
}

bool cci::cnf::gs_cci_private_broker::has_callbacks(const std::string& parname) {
  // TODO
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false;
}

void cci::cnf::gs_cci_private_broker::add_param(cci_base_param* par) {
  //cci::cnf::cci_base_param* bpar = dynamic_cast<cci::cnf::cci_base_param*>(par);
  //assert(bpar != NULL && "This should never happen, a cci_base_param shall be a cci_base_param!");
  
  if (public_params.find(par->get_name()) == public_params.end()) {
    m_mirrored_registry.insert(std::pair<std::string, cci_base_param*>(par->get_name(), par));
    //std::cout << name() << " (gs_cci_cnf_broker) add param to PRIVATE broker " << par->get_name() << std::endl;
  // or add to hierarchically upper broker if public
  } else {
    //cci_cnf_broker_if* a = cci_broker_manager::get_current_broker(owner_module->get_parent_object());
    cci::cnf::gs_cci_cnf_broker_accessor* b = dynamic_cast<cci::cnf::gs_cci_cnf_broker_accessor*>(m_upper_broker);
    cci::cnf::gs_cci_cnf_broker_if* gs_br = b->get_gs_broker();
    assert(gs_br && gs_br != this);
    gs_br->add_param(par);
  }
}

void cci::cnf::gs_cci_private_broker::remove_param(cci::cnf::cci_base_param* par) {
  if (m_mirrored_registry.find(par->get_name()) != m_mirrored_registry.end())
    m_mirrored_registry.erase(par->get_name());
  // or remove from hierarchically upper broker if public
  else {
    //cci_cnf_broker_if* a = cci_broker_manager::get_current_broker(owner_module->get_parent_object());
    cci::cnf::gs_cci_cnf_broker_accessor* b = dynamic_cast<cci::cnf::gs_cci_cnf_broker_accessor*>(m_upper_broker);
    cci::cnf::gs_cci_cnf_broker_if* gs_br = b->get_gs_broker();
    assert(gs_br && gs_br != this);
    gs_br->remove_param(par);      
  }
}

const std::vector<std::string> cci::cnf::gs_cci_private_broker::get_param_list(const std::string& pattern) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "pattern limited to GreenConfig capabilities");
  return gs::cnf::GCnf_private_Api::getParamList(pattern);
}

const std::vector<cci::cnf::cci_base_param*> cci::cnf::gs_cci_private_broker::get_params(const std::string& pattern) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "pattern limited to GreenConfig capabilities");
  std::vector<cci::cnf::cci_base_param*> parvec;
  std::vector<std::string> strvec;
  strvec = get_param_list(pattern);
  std::vector<std::string>::iterator striter;
  for (striter = strvec.begin(); striter != strvec.end(); striter++) {
    cci::cnf::cci_base_param* p = get_param(*striter);
    if (p) parvec.push_back(p);
  }
  return parvec;
}

bool cci::cnf::gs_cci_private_broker::is_private_broker() const {
  return true;
}

gs::cnf::cnf_api_if* cci::cnf::gs_cci_private_broker::get_gs_cnf_api() {
  return this;
}

