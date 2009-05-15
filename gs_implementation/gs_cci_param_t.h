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


#ifndef __GS_CCI_PARAM_BASE_H__
#define __GS_CCI_PARAM_BASE_H__


#include <string>
#include <iostream>
#include <map>
#include <set>
#include <boost/shared_ptr.hpp>

#include "cci.h"

namespace cci {

  template <typename T>
  class gs_cci_param_t
  : public cci_param<T>
  {
  protected:
    /// Typedef for the value.
    typedef T val_type;
    /// Typedef for the param itself.
    typedef gs_cci_param_t<T> my_type;
    
  public:

    explicit gs_cci_param_t(const std::string& n, const bool register_at_db = true,
                            const bool force_top_level_name = false) {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
    }
    
    ~gs_cci_param_t() {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
    }
    
    void init(const val_type& default_val) {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
    }
    
    void init() {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
    }
    
    const std::string& get_name() const {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
      return parname;
    }
   
    const bool is_destructing() const {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
      return false;
    }

    void set(const val_type& val) {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
    }
    
    const val_type& get() const {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
      return my_value;
    }
    
    boost::shared_ptr<callb_adapt_b> register_callback(boost::shared_ptr< callb_adapt_b> callb) {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
      return callb;
    }
    
    void unregister_all_callbacks(void* observer) {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
    }
    
    bool unregisterParamCallback(callb_adapt_b* callb)  {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
      return false;
    }
    
    bool has_callbacks() {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
      return false;
    }
    
  protected:
    std::string parname;

    val_type my_value;
    
    /// String whose reference can be returned as string value
    mutable std::string return_string;
  };

} // namespace cci

#endif
