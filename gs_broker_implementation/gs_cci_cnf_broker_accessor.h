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

#include <cci>
#include "greencontrol/config.h"
#include "gs_cci_cnf_broker_if.h"

#ifndef __GS_CCI_CNF_BROKER_ACCESSOR_H__
#define __GS_CCI_CNF_BROKER_ACCESSOR_H__


namespace cci {
  namespace cnf {
    
   
    class cci_base_param;
    
    /// Objects to handle the originator information and forwarding calls to the actual (private or non-private) gs broker implementation
    /**
     * This is created and returned by the original broker.
     * One accessor per originator. Internally holds the originator
     * information and forwards it on each call to the actual broker to make it
     * possible to log.
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
      
      cci_cnf_broker_if& get_accessor(const cci_originator& originator);
      
      /// Used by global get_cnf_broker_instance function
      gs_cci_cnf_broker_accessor(const cci_originator& originator, cci::cnf::cci_cnf_broker_if& orig_broker);
      
      ~gs_cci_cnf_broker_accessor();
      
      const std::string &name() const;

      // ////////////////////////////////////////////////////////////// //
      // /////////////// cci_cnf_broker_if functions ////////////////// //
      // ////////////////////////////////////////////////////////////// //

      void json_deserialize_initial_value(const std::string &parname, const std::string &value);
      
      void lock_initial_value(const std::string &parname);
      
      const std::string json_serialize(const std::string &parname);
      
      const std::string json_serialize_keep_unused(const std::string &parname);
      
      cci_base_param* get_param(const std::string &parname);
      
      bool param_exists(const std::string &parname);
      
      bool is_used(const std::string &parname);
      
      const std::vector<std::string> get_param_list();
      
      shared_ptr< cci::cnf::callb_adapt> register_callback(const std::string& parname, const cci::cnf::callback_type type, shared_ptr< cci::cnf::callb_adapt> callb);
      
      void unregister_all_callbacks(void* observer);
      
      bool unregister_callback(cci::cnf::callb_adapt* callb);
      
      bool has_callbacks(const std::string& parname);
      
      void add_param(cci::cnf::cci_base_param* par);
      
      void remove_param(cci::cnf::cci_base_param* par);
      
      //void set_alias(std::string& orig_parname, std::string& alias_parname);
      
      const std::vector<std::string> get_param_list(const std::string& pattern);
      
      const std::vector<cci::cnf::cci_base_param*> get_params(const std::string& pattern = "");

      bool is_private_broker() const;

      // ////////////////////////////////////////////////////////////// //
      // ////////////////////////////////////////////////////////////// //
      // ////////////////////////////////////////////////////////////// //

      /// Returns the originator
      /**
       * To be used by the global get function to identify for whom this accessor has been created.
       *
       * @return The originator this accessor has been created for.
       */
      const cci_originator* get_originator() const;
      
      const cci_originator* get_latest_write_originator(const std::string &parname) const;

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
      const cci_originator m_originator;

      /// Map for parameter accessors that have already been createated and can be returned (key=parameter name, value=parameter accessor)
      param_accessor_map m_param_accessor_map;

    };
    
    
  } // end namespace cci
} // end namespace cci

#endif
