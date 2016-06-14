/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010-2011 GreenSocs Ltd
  All rights reserved.
  Author(s): Mark Burton <mark@greensocs.com>
             Christian Schroeder <schroeder@eis.cs.tu-bs.de>

  Copyright 2016 Ericsson
  All rights reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 *****************************************************************************/


#include "gs_cci_cnf_private_broker.h"
#include "cci_core/cci_gen_unique_name.h"	// HACK; needs a proper home


/*cci::gs_cci_private_broker::gs_cci_private_broker(const char* name, sc_core::sc_module& owner, std::vector<const char*> pub_params) 
: gs::cnf::GCnf_private_Api(&owner, pub_params)
, m_upper_broker(&cci_broker_manager::get_current_broker(cci_originator(owner))) 
//, m_upper_broker(&cci_broker_manager::get_current_parent_broker(cci_originator(owner))) // at this point in time this is not yet pushed to stack, so use the top of stack as upper broker
, m_name(cci::cci_gen_unique_name(name))
{ 
  //m_gcnf_api = new gs::cnf::GCnf_private_Api(&owner, pub_params);
  std::cout << "Created new PRIVATE \""<<m_name<<"\" broker for Module \""<<owner.name()<<"\" (with GCnf_private_Api \""<< getName() << "\")" << std::endl;
}*/

cci::gs_cci_private_broker::gs_cci_private_broker(const char* name, sc_core::sc_module& owner, std::vector<std::string> pub_params) 
: gs::cnf::GCnf_private_Api(&owner, pub_params)
, m_upper_broker(&cci_broker_manager::get_current_broker(cci_originator(owner))) 
//, m_upper_broker(&cci_broker_manager::get_current_parent_broker(cci_originator(owner))) // at this point in time this is not yet pushed to stack, so use the top of stack as upper broker 
, m_name(cci::cci_gen_unique_name(name))
{ 
  //m_gcnf_api = new gs::cnf::GCnf_private_Api(&owner, pub_params);
  CCI_CNF_DUMP("Created new PRIVATE broker \""<<m_name<<"\" for Module \""<<owner.name()<<"\" (with GCnf_private_Api \""<< getName() << "\")");
  CCI_CNF_DUMP("  using broker \"" << m_upper_broker->name() << "\" to access upper hierarchy");
}

//cci::gs_cci_private_broker::gs_cci_private_broker(const char* name, sc_core::sc_module& owner_module, const char* pub_par ...)
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

cci::gs_cci_private_broker::~gs_cci_private_broker() { 
  //delete m_gcnf_api; m_gcnf_api = NULL;
}

const std::string &cci::gs_cci_private_broker::name() const {
  return m_name;
}

void cci::gs_cci_private_broker::json_deserialize_initial_value(const std::string &parname, const std::string &json_value) {
  // TODO: use JSON
  if ( !gs::cnf::GCnf_private_Api::setInitValue(parname, json_value) ) {
    cci_report_handler::set_param_failed("Setting initial value failed.");
  } else {
    m_implicit_originator_map.insert( std::pair<std::string, cci::cci_originator>(parname, *cci_originator::get_global_originator()));
  }
}

const cci::cci_originator* cci::gs_cci_private_broker::get_latest_write_originator(const std::string &parname) const {
  cci::cci_base_param* p = get_param_const(parname);
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

void cci::gs_cci_private_broker::lock_initial_value(const std::string &parname) {
  if ( !gs::cnf::GCnf_private_Api::lockInitValue(parname) ) {
    cci_report_handler::set_param_failed("Locking initial value failed.");
  }
}

const std::string cci::gs_cci_private_broker::json_serialize(const std::string &parname) {
  // TODO: use JSON
  return gs::cnf::GCnf_private_Api::getValue(parname);
}

const std::string cci::gs_cci_private_broker::json_serialize_keep_unused(const std::string &parname) {
  // TODO: use JSON
  return gs::cnf::GCnf_private_Api::getValue(parname, "", true);
}

cci::cci_base_param* cci::gs_cci_private_broker::get_param(const std::string &parname) {
  return get_param_const(parname);
}
cci::cci_base_param* cci::gs_cci_private_broker::get_param_const(const std::string &parname) const {
  std::map<std::string,cci_base_param*>::const_iterator iter = m_mirrored_registry.find(parname);
  if( iter != m_mirrored_registry.end() )
    return iter->second;
  else {
    // and get from hierarchically upper broker
    return m_upper_broker->get_param(parname);
  }
  return NULL;
}

bool cci::gs_cci_private_broker::param_exists(const std::string &parname) {
  return gs::cnf::GCnf_private_Api::existsParam(parname);
}

bool cci::gs_cci_private_broker::is_used(const std::string &parname) {
  return gs::cnf::GCnf_private_Api::is_used(parname);
}

const std::vector<std::string> cci::gs_cci_private_broker::get_param_list() {
  return gs::cnf::GCnf_private_Api::getParamList();
}

cci::shared_ptr<cci::callb_adapt> cci::gs_cci_private_broker::register_callback(const std::string& parname, const callback_type type, cci::shared_ptr<cci::callb_adapt> callb) {
  // TODO
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return callb;
}

void cci::gs_cci_private_broker::unregister_all_callbacks(void* observer) {
  // TODO
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
}

bool cci::gs_cci_private_broker::unregister_callback(cci::callb_adapt* callb) {
  // TODO
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false;
}

bool cci::gs_cci_private_broker::has_callbacks(const std::string& parname) {
  // TODO
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false;
}

void cci::gs_cci_private_broker::add_param(cci_base_param* par) {
  //cci::cci_base_param* bpar = dynamic_cast<cci::cci_base_param*>(par);
  //assert(bpar != NULL && "This should never happen, a cci_base_param shall be a cci_base_param!");
  
  if (public_params.find(par->get_name()) == public_params.end()) {
    m_mirrored_registry.insert(std::pair<std::string, cci_base_param*>(par->get_name(), par));
    //std::cout << name() << " (gs_cci_cnf_broker) add param to PRIVATE broker " << par->get_name() << std::endl;
  // or add to hierarchically upper broker if public
  } else {
    m_upper_broker->add_param(par);
  }
}

void cci::gs_cci_private_broker::remove_param(cci::cci_base_param* par) {
  if (m_mirrored_registry.find(par->get_name()) != m_mirrored_registry.end())
    m_mirrored_registry.erase(par->get_name());
  // or remove from hierarchically upper broker if public
  else {
    m_upper_broker->remove_param(par);
  }
}

const std::vector<std::string> cci::gs_cci_private_broker::get_param_list(const std::string& pattern) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "pattern limited to GreenConfig capabilities");
  return gs::cnf::GCnf_private_Api::getParamList(pattern);
}

const std::vector<cci::cci_base_param*> cci::gs_cci_private_broker::get_params(const std::string& pattern) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "pattern limited to GreenConfig capabilities");
  const std::vector<std::string> strvec = get_param_list(pattern);
  std::vector<cci::cci_base_param*> parvec;
  for (std::vector<std::string>::const_iterator striter = strvec.begin(); striter != strvec.end(); ++striter) {
    cci::cci_base_param* p = get_param(*striter);
    if (p) parvec.push_back(p);
  }
  return parvec;
}

bool cci::gs_cci_private_broker::is_private_broker() const {
  return true;
}

gs::cnf::cnf_api_if* cci::gs_cci_private_broker::get_gs_cnf_api() {
  return this;
}

