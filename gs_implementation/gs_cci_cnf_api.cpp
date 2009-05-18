//   OSCI CCI WG
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// 
// ENDLICENSETEXT


#include "gs_cci_cnf_api.h"

cci::gs_cci_cnf_api::gs_cci_cnf_api() 
: m_gcnf_api(NULL) { 
  m_gcnf_api = gs::cnf::GCnf_Api::getApiInstance(NULL);
}

cci::gs_cci_cnf_api::~cci_cnf_api() { 
}

bool cci::gs_cci_cnf_api::set_init_value(const std::string &parname, const std::string &value) {
  return m_gcnf_api->setInitValue(parname, value);
}

const std::string cci::gs_cci_cnf_api::get_json_string(const std::string &parname) {
  return m_gcnf_api->getValue(parname);
}

cci::cci_param_base* cci::gs_cci_cnf_api::get_param(const std::string &parname) {
  return dynamic_cast<cci::cci_param_base*> (m_gcnf_api->getPar(parname));
}

bool cci::gs_cci_cnf_api::exists_param(const std::string &parname) {
  return m_gcnf_api->existsParam(parname);
}

const std::vector<std::string> cci::gs_cci_cnf_api::get_param_list() {
  return m_gcnf_api->getParamList();
}

boost::shared_ptr< cci::callb_adapt_b> cci::gs_cci_cnf_api::register_callback(const std::string& parname, boost::shared_ptr< cci::callb_adapt_b> callb) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return callb;
}


void cci::gs_cci_cnf_api::unregister_all_callbacks(void* observer) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
}

bool cci::gs_cci_cnf_api::unregister_param_callback(callb_adapt_b* callb) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false;
}

bool cci::gs_cci_cnf_api::has_callbacks(const std::string& parname) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false;
}

bool cci::gs_cci_cnf_api::add_param(cci_param_base* par) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false;
}

bool cci::gs_cci_cnf_api::remove_param(cci_param_base* par) {
  gs::gs_param_base* p = dynamic_cast<gs::gs_param_base*> (par);
  return m_gcnf_api->removePar(p);
}


void cci::gs_cci_cnf_api::set_alias(std::string& orig_parname, std::string& alias_parname) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
}
    
const std::vector<std::string> cci::gs_cci_cnf_api::get_param_list(const std::string& pattern) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "pattern limited to GreenSocs capabilities");
  return m_gcnf_api->getParamList(pattern);
}

const std::vector<cci::cci_param_base*> cci::gs_cci_cnf_api::get_params(const std::string& pattern) {
  std::vector<cci::cci_param_base*> parvec;
  std::vector<std::string> strvec;
  strvec = get_param_list(pattern);
  std::vector<std::string>::iterator striter;
  for (striter = strvec.begin(); striter != strvec.end(); striter++) {
    cci::cci_param_base* p = get_param(*striter);
    if (p) parvec.push_back(p);
  }
  return parvec;
}

