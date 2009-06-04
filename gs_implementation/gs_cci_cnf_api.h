/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2009 by all Contributors.
  All Rights reserved.

  Developed by GreenSocs : http://www.greensocs.com/
   Christian Schroeder, schroeder@eis.cs.tu-bs.de
   Mark Burton, mark@greensocs.com

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/


#ifndef __GS_CCI_CNF_API_H__
#define __GS_CCI_CNF_API_H__


#include <cci.h>
#include "greencontrol/gcnf/apis/GCnf_Api/GCnf_Api.h"

namespace cci {


  class gs_cci_cnf_api
  : public cci_cnf_api
  {
    
  public:
    
    gs_cci_cnf_api();
    
    ~gs_cci_cnf_api();

    set_param_error_type set_init_value(const std::string &parname, const std::string &value);
    
    const std::string get_json_string(const std::string &parname);

    cci_param_base* get_param(const std::string &parname);
    
    bool exists_param(const std::string &parname);
    
    const std::vector<std::string> get_param_list();
    
    boost::shared_ptr< callb_adapt_b> register_callback(const std::string& parname, const callback_type type, boost::shared_ptr< callb_adapt_b> callb);

    void unregister_all_callbacks(void* observer);
    
    bool unregister_param_callback(callb_adapt_b* callb);
    
    bool has_callbacks(const std::string& parname);
    
    add_param_error_type add_param(cci_param_base* par);
    
    remove_param_error_type remove_param(cci_param_base* par);
    
    void set_alias(std::string& orig_parname, std::string& alias_parname);
        
    const std::vector<std::string> get_param_list(const std::string& pattern);
    
    const std::vector<cci_param_base*> get_params(const std::string& pattern = "");

  protected:
  
    gs::cnf::cnf_api* m_gcnf_api;
  };

      
} // end namespace cci

#endif
