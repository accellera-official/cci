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

cci::gs_cci_cnf_api::~cci_cnf_api() { 
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
}

bool cci::gs_cci_cnf_api::set_init_value(const std::string &parname, const std::string &value) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false;
}

const std::string cci::gs_cci_cnf_api::get_string(const std::string &parname) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return std::string("default");
}

cci::cci_param_base* cci::gs_cci_cnf_api::get_param(const std::string &parname) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return NULL;
}

bool cci::gs_cci_cnf_api::exists_param(const std::string &parname) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false; 
}

const std::vector<std::string> cci::gs_cci_cnf_api::get_param_list() {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  std::vector<std::string> vec;
  return vec;
}

boost::shared_ptr< cci::callb_adapt_b> cci::gs_cci_cnf_api::register_callback(const std::string& parname, boost::shared_ptr< cci::callb_adapt_b> callb) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return callb;
}


void cci::gs_cci_cnf_api::unregister_all_callbacks(void* observer) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
}

bool cci::gs_cci_cnf_api::unregisterParamCallback(callb_adapt_b* callb) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false;
}

bool cci::gs_cci_cnf_api::has_callbacks(std::string& parname) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false;
}

bool cci::gs_cci_cnf_api::add_param(cci_param_base* par) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false;
}

bool cci::gs_cci_cnf_api::remove_param(cci_param_base* par) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false;
}


void cci::gs_cci_cnf_api::set_alias(std::string& orig_parname, std::string& alias_parname) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
}
    
const std::vector<std::string> cci::gs_cci_cnf_api::get_param_list(const std::string& pattern) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  std::vector<std::string> vec;
  return vec;
}

const std::vector<cci::cci_param_base*> cci::gs_cci_cnf_api::get_params(const std::string& pattern) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  std::vector<cci_param_base*> vec;
  return vec;
}

