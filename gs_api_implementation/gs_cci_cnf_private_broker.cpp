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
//   This program is free software.
// 
//   If you have no applicable agreement with GreenSocs Ltd, this software
//   is licensed to you, and you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   If you have a applicable agreement with GreenSocs Ltd, the terms of that
//   agreement prevail.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//   02110-1301  USA 
// 
// ENDLICENSETEXT


#include "gs_cci_cnf_private_broker.h"

cci::gs_cci_private_broker::gs_cci_private_broker(sc_core::sc_module* owner, std::vector<std::string> pub_params) 
: m_gcnf_api(NULL) { 
  m_gcnf_api = new gs::cnf::GCnf_private_Api(owner, pub_params);
}

cci::gs_cci_private_broker::gs_cci_private_broker(sc_core::sc_module* owner_module, const char* pub_par ...)
: m_gcnf_api(NULL) {
  std::vector<std::string> pub_param_lst;
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

  m_gcnf_api = new gs::cnf::GCnf_private_Api(owner_module, pub_param_lst);
}

cci::gs_cci_private_broker::~gs_cci_private_broker() { 
  delete m_gcnf_api; m_gcnf_api = NULL;
}

void cci::gs_cci_private_broker::set_init_value(const std::string &parname, const std::string &json_value) {
  // TODO: use JSON
  if ( !m_gcnf_api->setInitValue(parname, json_value) ) {
    CCI_THROW_ERROR(cci_report_types::type().set_param_failed, "Setting initial value failed.");
  }
}

const std::string cci::gs_cci_private_broker::get_json_string(const std::string &parname) {
  // TODO: use JSON
  return m_gcnf_api->getValue(parname);
}

cci::cci_base_param* cci::gs_cci_private_broker::get_param(const std::string &parname) {
  std::map<std::string,cci_base_param*>::iterator iter = m_mirrored_registry.find(parname);
  if( iter != m_mirrored_registry.end() )
    return iter->second;
  else return NULL;
}

bool cci::gs_cci_private_broker::exists_param(const std::string &parname) {
  return m_gcnf_api->existsParam(parname);
}

bool cci::gs_cci_private_broker::is_used(const std::string &parname) {
  return m_gcnf_api->is_used(parname);
}

const std::vector<std::string> cci::gs_cci_private_broker::get_param_list() {
  return m_gcnf_api->getParamList();
}

cci::shared_ptr< cci::callb_adapt_b> cci::gs_cci_private_broker::register_callback(const std::string& parname, const callback_type type, cci::shared_ptr< cci::callb_adapt_b> callb) {
  // TODO
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return callb;
}

void cci::gs_cci_private_broker::unregister_all_callbacks(void* observer) {
  // TODO
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
}

bool cci::gs_cci_private_broker::unregister_param_callback(callb_adapt_b* callb) {
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
  m_mirrored_registry.insert(std::pair<std::string, cci_base_param*>(par->get_name(), par));
}

void cci::gs_cci_private_broker::remove_param(cci_base_param* par) {
  m_mirrored_registry.erase(par->get_name());
}

const std::vector<std::string> cci::gs_cci_private_broker::get_param_list(const std::string& pattern) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "pattern limited to GreenConfig capabilities");
  return m_gcnf_api->getParamList(pattern);
}

const std::vector<cci::cci_base_param*> cci::gs_cci_private_broker::get_params(const std::string& pattern) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "pattern limited to GreenConfig capabilities");
  std::vector<cci::cci_base_param*> parvec;
  std::vector<std::string> strvec;
  strvec = get_param_list(pattern);
  std::vector<std::string>::iterator striter;
  for (striter = strvec.begin(); striter != strvec.end(); striter++) {
    cci::cci_base_param* p = get_param(*striter);
    if (p) parvec.push_back(p);
  }
  return parvec;
}

gs::cnf::cnf_api* cci::gs_cci_private_broker::get_gcnf_api() {
  return m_gcnf_api;
}

