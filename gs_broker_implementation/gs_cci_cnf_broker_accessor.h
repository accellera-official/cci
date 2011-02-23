// LICENSETEXT
//
//   Copyright (C) 2009-2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT



// NOTE: The broker implementation must not include any parameter vendor dependentent headers!

#include <cci.h>
#include "greencontrol/config.h"
#include "gs_cci_cnf_broker_if.h"

#ifndef __GS_CCI_CNF_BROKER_ACCESSOR_H__
#define __GS_CCI_CNF_BROKER_ACCESSOR_H__

#define GS_CCI_IMPL_UNKNOWN_ORIGINATOR_STRING "DEFAULT_BROKER_UNKNOWN_ORIGINATOR"

namespace cci {
  namespace cnf {
    
   
    class cci_base_param;
    
    /// Objects to handle the originator information and forwarding calls to the actual (private or non-private) gs broker implementation
    /**
     * This is created and returned by the global get_cnf_broker_implementation
     * function. One accessor per originator. Internally holds the originator
     * information (as sc_object* or string) and forwards it on each call to the
     * actual broker to make it possible to log.
     *
     * Furthermore it maps between the original parameters -being returned by the
     * brokers- and the parameter accessors -which are created locally and returned
     * to the users.
     */
    class gs_cci_cnf_broker_accessor
    : public cci::cnf::cci_cnf_broker_if // end user interface
    , public gs::cnf::gs_cnf_api_accessor // internal accessor
    {
    protected:
      
      /// Typedef for parameter accessor map
      typedef std::map<std::string, cci::cnf::cci_base_param*> param_accessor_map;
      
    public:
      
      /// Used by global get_cnf_broker_instance function
      gs_cci_cnf_broker_accessor(const cci_originator& originator);
      
      /// Internally called by global get_cnf_broker_instance function for creating the default broker instance
      gs_cci_cnf_broker_accessor(bool called_internally_for_creating_global_broker);
      
      ~gs_cci_cnf_broker_accessor();
      
      // ////////////////////////////////////////////////////////////// //
      // /////////////// cci_cnf_broker_if functions ////////////////// //
      // ////////////////////////////////////////////////////////////// //

      void set_init_value(const std::string &parname, const std::string &value);
      
      void lock_init_value(const std::string &parname);
      
      const std::string get_json_string(const std::string &parname);
      
      const std::string get_json_string_keep_unused(const std::string &parname);
      
      cci_base_param* get_param(const std::string &parname);
      
      bool exists_param(const std::string &parname);
      
      bool is_used(const std::string &parname);
      
      const std::vector<std::string> get_param_list();
      
      shared_ptr< cci::cnf::callb_adapt_b> register_callback(const std::string& parname, const cci::cnf::callback_type type, shared_ptr< cci::cnf::callb_adapt_b> callb);
      
      void unregister_all_callbacks(void* observer);
      
      bool unregister_param_callback(cci::cnf::callb_adapt_b* callb);
      
      bool has_callbacks(const std::string& parname);
      
      void add_param(cci::cnf::cci_base_param* par);
      
      void remove_param(cci::cnf::cci_base_param* par);
      
      //void set_alias(std::string& orig_parname, std::string& alias_parname);
      
      const std::vector<std::string> get_param_list(const std::string& pattern);
      
      const std::vector<cci::cnf::cci_base_param*> get_params(const std::string& pattern = "");

      // ////////////////////////////////////////////////////////////// //
      // ////////////////////////////////////////////////////////////// //
      // ////////////////////////////////////////////////////////////// //

      /// Returns the originator
      /**
       * To be used by the global get function to identify for whom this accessor has been created.
       *
       * @return The originator this accessor has been created for.
       */
      cci_originator* get_originator();
      
      gs_cci_cnf_broker_if* get_gs_broker();

      /// Returns the underlying gs_GCnf_Api (implements gs_cnf_api_accessor)
      gs::cnf::cnf_api_if* get_gs_cnf_api();

    protected:

      /// Convenience access to local parameter accessor map
      /**
       * Returns the parameter accessor belonging to the orig_param. Creates
       * a new one if not yet created.
       *
       * @param orig_param  Original (not-accessor) parameter
       * @return parameter accessor
       */
      cci::cnf::cci_base_param* get_param_accessor(cci::cnf::cci_base_param& orig_param);
      
    protected:
      
      /// the appropriate (private or non-private) broker with originator function calls
      gs_cci_cnf_broker_if* m_broker;
      
      /// Originator information
      cci_originator m_originator;

      /// Map for parameter accessors that have already been createated and can be returned (key=parameter name, value=parameter accessor)
      param_accessor_map m_param_accessor_map;

    };
    
    
  } // end namespace cci
} // end namespace cci

#endif
