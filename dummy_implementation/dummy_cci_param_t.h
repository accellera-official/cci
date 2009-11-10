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



#ifndef __DUMMY_CCI_PARAM_BASE_H__
#define __DUMMY_CCI_PARAM_BASE_H__


#include <string>
#include <iostream>
#include <map>
#include <set>
#include <boost/shared_ptr.hpp>

#include "cci.h"

namespace cci {

  template <typename T>
  class dummy_cci_param_t
  : public cci_param<T>
  {
  protected:
    /// Typedef for the value.
    typedef T val_type;
    /// Typedef for the param itself.
    typedef dummy_cci_param_t<T> my_type;
    
  public:

    using cci_param<T>::operator=;

    explicit dummy_cci_param_t(const std::string& n, const bool register_at_db = true,
                            const bool force_top_level_name = false) {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
    }
    
    ~dummy_cci_param_t() {
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
   
    void set(const val_type& val) {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
    }
    
    const val_type& get() const {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
      return my_value;
    }
    
    boost::shared_ptr<callb_adapt_b> register_callback(const callback_type type, boost::shared_ptr< callb_adapt_b> callb) {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
      return callb;
    }
    
    void unregister_all_callbacks(void* observer) {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
    }
    
    bool unregister_param_callback(callb_adapt_b* callb)  {
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
