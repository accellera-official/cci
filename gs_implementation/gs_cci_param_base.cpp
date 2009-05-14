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


#include "gs_cci_param_base.h"


cci::gs_cci_param_base::gs_cci_param_base(const std::string& n, const bool register_at_db,
                                          const bool force_top_level_name) 
//: cci::cci_param_base(n, register_at_db, force_top_level_name) 
{
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
}

cci::gs_cci_param_base::~gs_cci_param_base() {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
}

const std::string& cci::gs_cci_param_base::get_name() const {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return parname;
}

const bool cci::gs_cci_param_base::is_destructing() const {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false;
}

boost::shared_ptr<cci::callb_adapt_b> cci::gs_cci_param_base::register_callback(boost::shared_ptr< cci::callb_adapt_b> callb) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return callb;
}

void cci::gs_cci_param_base::unregister_all_callbacks(void* observer) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
}

bool cci::gs_cci_param_base::unregisterParamCallback(callb_adapt_b* callb) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false;
}

bool cci::gs_cci_param_base::has_callbacks() {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
  return false;
}
