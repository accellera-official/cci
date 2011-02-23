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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#ifndef __GS_CCI_CNF_PRIVATE_BROKER_H__
#define __GS_CCI_CNF_PRIVATE_BROKER_H__


#include <cci.h>
#include "greencontrol/config.h"
#include "gs_cci_cnf_broker_accessor.h"
#include "helpers.h"


namespace cci {
  namespace cnf {
    
    class cci_base_param_if;
    
    class gs_cci_private_broker
    : public gs_cci_cnf_broker_if // used internally by broker accessors
    , public gs::cnf::GCnf_private_Api
    , public gs::cnf::gs_cnf_api_accessor
    {
      
    public:
      
      gs_cci_private_broker(sc_core::sc_module* owner, std::vector<const char*> pub_params);
      gs_cci_private_broker(sc_core::sc_module* owner, std::vector<std::string> pub_params);
      //gs_cci_private_broker(sc_core::sc_module* owner_module, const char* pub_par ...);
            
      ~gs_cci_private_broker();
      
      void set_init_value(const std::string &parname, const std::string &json_value);
      
      void lock_init_value(const std::string &parname);
      
      const std::string get_json_string(const std::string &parname);
      
      const std::string get_json_string_keep_unused(const std::string &parname);
      
      cci_base_param* get_param(const std::string &parname);
      
      const std::vector<std::string> get_param_list();
      
      const std::vector<std::string> get_param_list(const std::string& pattern);
      
      const std::vector<cci::cnf::cci_base_param*> get_params(const std::string& pattern);
      
      bool exists_param(const std::string &parname);
      
      bool is_used(const std::string &parname);
      
      shared_ptr< callb_adapt_b> register_callback(const std::string& parname, const callback_type type, shared_ptr< callb_adapt_b> callb);
      
      void unregister_all_callbacks(void* observer);
      
      bool unregister_param_callback(callb_adapt_b* callb);
      
      bool has_callbacks(const std::string& parname);
      
      void add_param(cci_base_param* par);
      
      void remove_param(cci_base_param* par);
      
      //void set_alias(std::string& orig_parname, std::string& alias_parname);
      
      /// Returns the underlying gs GCnf_Api (implements gs_cnf_api_accessor)
      gs::cnf::cnf_api_if* get_gs_cnf_api();
      
    protected:
      
      //gs::cnf::cnf_api_if* m_gcnf_api;
      
      std::map<std::string, cci_base_param*> m_mirrored_registry;
      
    };
    
    
  } // end namespace cci
} // end namespace cci

#endif
