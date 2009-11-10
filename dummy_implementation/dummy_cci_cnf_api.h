
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



#ifndef __DUMMY_CCI_CNF_API_H__
#define __DUMMY_CCI_CNF_API_H__


#include <cci.h>


namespace cci {


  class dummy_cci_cnf_api
  : public cci_cnf_api
  {
    
  public:
    
    ~dummy_cci_cnf_api();

    set_param_error_type set_init_value(const std::string &parname, const std::string &value);
    
    const std::string get_json_string(const std::string &parname);
    
    cci_base_param* get_param(const std::string &parname);
    
    bool exists_param(const std::string &parname);
    
    const std::vector<std::string> get_param_list();
    
    boost::shared_ptr< callb_adapt_b> register_callback(const std::string& parname, const callback_type type, boost::shared_ptr< callb_adapt_b> callb);
    
    void unregister_all_callbacks(void* observer);
    
    bool unregister_param_callback(callb_adapt_b* callb);
    
    bool has_callbacks(const std::string& parname);
    
    add_param_error_type add_param(cci_base_param* par);
    
    remove_param_error_type remove_param(cci_base_param* par);
    
    void set_alias(std::string& orig_parname, std::string& alias_parname);
    
    const std::vector<std::string> get_param_list(const std::string& pattern);
    
    const std::vector<cci_base_param*> get_params(const std::string& pattern = "");
    
  };

      
} // end namespace cci

#endif
