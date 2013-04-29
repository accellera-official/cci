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


#ifndef __GS_CCI_CNF_PRIVATE_BROKER_ACCESSOR_H__
#define __GS_CCI_CNF_PRIVATE_BROKER_ACCESSOR_H__


#include <cci>
#include "greencontrol/config.h"
#include "gs_cci_cnf_private_broker.h"


namespace cci {
  namespace cnf {
        
    
    /// To be instantiated by the user to create a private broker.
    /**
     * This is not used, returned or created by any other function or instance,
     * including the global get_cnf_broker_instance function which creates a
     * gs_cci_cnf_broker_accessor for the private broker as well.
     */
    class gs_cci_private_broker_accessor
    : public cci::cnf::cci_cnf_broker_if // end user interface
    , public gs::cnf::gs_cnf_api_accessor // internal accessor
    {
    public:
      cci_cnf_broker_if& get_accessor(const cci_originator& originator);

      //gs_cci_private_broker_accessor(sc_core::sc_module& owner, std::vector<const char*> pub_params, const cci_originator& originator);

      /// Constructor for broker accessort. To be used by end user for creating private brokers.
      /// Will forward the owner and private param list to private broker constructor, creates an originator using the owner reference.
      gs_cci_private_broker_accessor(sc_core::sc_module& owner, std::vector<std::string> pub_params);

    public:
      ~gs_cci_private_broker_accessor();
      
      const std::string &name() const;

      // ////////////////////////////////////////////////////////////// //
      // /////////////// cci_cnf_broker_if functions ////////////////// //
      // ////////////////////////////////////////////////////////////// //
      
      void set_init_value(const std::string &parname, const std::string &value);
      
      void lock_init_value(const std::string &parname);
      
      const std::string get_json_string(const std::string &parname);
      
      const std::string get_json_string_keep_unused(const std::string &parname);
      
      cci::cnf::cci_base_param* get_param(const std::string &parname);
      
      bool exists_param(const std::string &parname);
      
      bool is_used(const std::string &parname);
      
      const std::vector<std::string> get_param_list();
      
      const std::vector<std::string> get_param_list(const std::string& pattern);
      
      const std::vector<cci::cnf::cci_base_param*> get_params(const std::string& pattern = "");
      
      shared_ptr< cci::cnf::callb_adapt> register_callback(const std::string& parname, const cci::cnf::callback_type type, shared_ptr< cci::cnf::callb_adapt> callb);
      
      void unregister_all_callbacks(void* observer);
      
      bool unregister_param_callback(cci::cnf::callb_adapt* callb);
      
      bool has_callbacks(const std::string& parname);
      
      void add_param(cci::cnf::cci_base_param* par);
      
      void remove_param(cci::cnf::cci_base_param* par);
      
      //void set_alias(std::string& orig_parname, std::string& alias_parname);
      
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
      
      /// the appropriate (private or non-private) broker with originator function calls
      gs_cci_cnf_broker_if* m_broker;
      
      /// Originator information
      cci_originator m_originator;

    };
    

  
  } // end namespace cnf
} // end namespace cci

#endif
