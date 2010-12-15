// LICENSETEXT
//
//   Copyright (C) 2009-2010 : GreenSocs Ltd
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


#ifndef __GS_CCI_CNF_BROKER_H__
#define __GS_CCI_CNF_BROKER_H__

// NOTE: The broker implementation must not include any parameter vendor dependentent headers!

#include <cci.h>
#include "greencontrol/config.h"
#include "gs_cci_cnf_broker_if.h"

namespace cci {
  namespace cnf {
    
    class cci_base_param;
    
    /// (Non-private) broker implementation
    /**
     * Actually forwards to GCnf_Api.
     * Guarded by gs_cci_cnf_broker_accessor
     */
    class gs_cci_cnf_broker
    //: public cci::cnf::cci_cnf_broker_if
    : public gs_cci_cnf_broker_if
    , public gs::cnf::GCnf_Api
    , public gs::cnf::gs_cnf_api_accessor
    {
    public:
      
      gs_cci_cnf_broker(const std::string& name);
      
      ~gs_cci_cnf_broker();
      
      void set_init_value(const std::string &parname, const std::string &json_value, sc_core::sc_object* originator);
      void set_init_value(const std::string &parname, const std::string &json_value, const std::string& originator);
      
      void lock_init_value(const std::string &parname, sc_core::sc_object* originator);
      void lock_init_value(const std::string &parname, const std::string& originator);
      
      const std::string get_json_string(const std::string &parname, sc_core::sc_object* originator);
      const std::string get_json_string(const std::string &parname, const std::string& originator);
      
      const std::string get_json_string_keep_unused(const std::string &parname, sc_core::sc_object* originator);
      const std::string get_json_string_keep_unused(const std::string &parname, const std::string& originator);
      
      cci_base_param* get_param(const std::string &parname, sc_core::sc_object* originator);
      cci_base_param* get_param(const std::string &parname, const std::string& originator);
      
      const std::vector<std::string> get_param_list(sc_core::sc_object* originator);
      const std::vector<std::string> get_param_list(const std::string& originator);

      const std::vector<std::string> get_param_list(const std::string& pattern, sc_core::sc_object* originator);
      const std::vector<std::string> get_param_list(const std::string& pattern, const std::string& originator);

      const std::vector<cci::cnf::cci_base_param*> get_params(const std::string& pattern, sc_core::sc_object* originator);
      const std::vector<cci::cnf::cci_base_param*> get_params(const std::string& pattern, const std::string& originator);
      
      bool exists_param(const std::string &parname, sc_core::sc_object* originator);
      bool exists_param(const std::string &parname, const std::string& originator);
      
      bool is_used(const std::string &parname, sc_core::sc_object* originator);
      bool is_used(const std::string &parname, const std::string& originator);
      
      shared_ptr< callb_adapt_b> register_callback(const std::string& parname, const callback_type type, shared_ptr< callb_adapt_b> callb, sc_core::sc_object* originator);
      shared_ptr< callb_adapt_b> register_callback(const std::string& parname, const callback_type type, shared_ptr< callb_adapt_b> callb, const std::string& originator);
      
      void unregister_all_callbacks(void* observer, sc_core::sc_object* originator);
      void unregister_all_callbacks(void* observer, const std::string& originator);
      
      bool unregister_param_callback(callb_adapt_b* callb, sc_core::sc_object* originator);
      bool unregister_param_callback(callb_adapt_b* callb, const std::string& originator);
      
      bool has_callbacks(const std::string& parname, sc_core::sc_object* originator);
      bool has_callbacks(const std::string& parname, const std::string& originator);
      
      void add_param(cci_base_param* par, sc_core::sc_object* originator);
      void add_param(cci_base_param* par, const std::string& originator);
      
      void remove_param(cci_base_param* par, sc_core::sc_object* originator);
      void remove_param(cci_base_param* par, const std::string& originator);
      
      //void set_alias(std::string& orig_parname, std::string& alias_parname);
      
      gs::cnf::cnf_api_if* get_gs_cnf_api();
      
    protected:
      
      std::map<std::string, cci::cnf::cci_base_param*> m_mirrored_registry;
      
      std::string m_name;
      
    };
    
    
  } // end namespace cci
} // end namespace cci

#endif
