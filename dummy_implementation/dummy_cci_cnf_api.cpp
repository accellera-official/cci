// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
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



#include "dummy_cci_cnf_api.h"

cci::dummy_cci_cnf_api::~cci_cnf_api() { 
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
}

cci::set_param_error_type cci::dummy_cci_cnf_api::set_init_value(const std::string &parname, const std::string &value) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return set_param_failed;
}

const std::string cci::dummy_cci_cnf_api::get_json_string(const std::string &parname) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return std::string("default");
}

cci::cci_base_param* cci::dummy_cci_cnf_api::get_param(const std::string &parname) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return NULL;
}

bool cci::dummy_cci_cnf_api::exists_param(const std::string &parname) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false; 
}

const std::vector<std::string> cci::dummy_cci_cnf_api::get_param_list() {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  std::vector<std::string> vec;
  return vec;
}

boost::shared_ptr< cci::callb_adapt_b> cci::dummy_cci_cnf_api::register_callback(const std::string& parname, const callback_type type, boost::shared_ptr< cci::callb_adapt_b> callb) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return callb;
}


void cci::dummy_cci_cnf_api::unregister_all_callbacks(void* observer) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
}

bool cci::dummy_cci_cnf_api::unregister_param_callback(callb_adapt_b* callb) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false;
}

bool cci::dummy_cci_cnf_api::has_callbacks(const std::string& parname) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false;
}

cci::add_param_error_type cci::dummy_cci_cnf_api::add_param(cci_base_param* par) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return cci::add_param_failed;
}

cci::remove_param_error_type cci::dummy_cci_cnf_api::remove_param(cci_base_param* par) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return cci::remove_param_failed;
}


void cci::dummy_cci_cnf_api::set_alias(std::string& orig_parname, std::string& alias_parname) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
}
    
const std::vector<std::string> cci::dummy_cci_cnf_api::get_param_list(const std::string& pattern) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  std::vector<std::string> vec;
  return vec;
}

const std::vector<cci::cci_base_param*> cci::dummy_cci_cnf_api::get_params(const std::string& pattern) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  std::vector<cci_base_param*> vec;
  return vec;
}

