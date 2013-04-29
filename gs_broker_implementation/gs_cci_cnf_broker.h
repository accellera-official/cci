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

#include <cci>
#include "greencontrol/config.h"
#include "gs_cci_cnf_broker_if.h"
#include "gs_cci_cnf_broker_accessor_handler.h"

namespace cci {
namespace cnf {
  
  class cci_base_param;
  
  /// (Non-private) broker implementation
  /**
   * Actually forwards to GCnf_Api.
   * Guarded by gs_cci_cnf_broker_accessor
   */
  class gs_cci_cnf_broker
  : public cci::cnf::cci_cnf_broker_if
  , public gs_cci_cnf_broker_if
  , public gs::cnf::GCnf_Api
  , public gs::cnf::gs_cnf_api_accessor
  , public cci::cnf::gs_cci_cnf_broker_accessor_handler
  {
  protected:
    
    /// Typedef for the param itself.
    typedef gs_cci_cnf_broker my_type;

    /// Callback forwarder class
    /**
     * This is instantiated and registered at the base param to forward a 
     * callback to the cci world when called by the gs_base_param.
     * @see also see similar gs_cci_base_param::internal_callback_forwarder
     */
    class internal_callback_forwarder {
    public:
      internal_callback_forwarder(cci::shared_ptr<cci::cnf::callb_adapt> _adapt, const gs::cnf::callback_type _type, my_type& _par);
      ~internal_callback_forwarder();
      
      // This gets called by the base gs_param
      gs::cnf::callback_return_type call(const std::string& parname, gs::cnf::callback_type cbtype);
      void call_create_param(const std::string parname, const std::string value);
      const gs::cnf::callback_type get_type();
      
      cci::cnf::callb_adapt* adapt;
      gs::cnf::callback_type type;
      my_type *param;
      cci::shared_ptr< gs::cnf::CallbAdapt_b> calling_gs_adapter;
    };
    
    /// Typedef for internal new param callback handling map
    typedef std::multimap<std::string, internal_callback_forwarder*> observerCallbackMap;

    /// Typedef for internal implicit param originator map
    typedef std::multimap<std::string, cci::cnf::cci_originator> implicitOriginatorMap;

  public:

    cci_cnf_broker_if& get_accessor(const cci_originator& originator) { return cci::cnf::gs_cci_cnf_broker_accessor_handler::get_accessor(originator, *this); }

    const cci_originator* get_originator() const { return NULL; }

    const cci_originator* get_latest_write_originator(const std::string &parname) const;

    gs_cci_cnf_broker(const std::string& name);
    
    /// Internally called by global get_cnf_broker_instance function for creating the default broker instance
    gs_cci_cnf_broker(bool called_internally_for_creating_global_broker);

    ~gs_cci_cnf_broker();
    
    const std::string &name() const;
    
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
    
    shared_ptr< callb_adapt> register_callback(const std::string& parname, const callback_type type, shared_ptr< callb_adapt> callb);
    
    void unregister_all_callbacks(void* observer);
    
    bool unregister_param_callback(callb_adapt* callb);
    
    bool has_callbacks(const std::string& parname);
    
    void add_param(cci_base_param* par);
    
    void remove_param(cci_base_param* par);
    
    //void set_alias(std::string& orig_parname, std::string& alias_parname);
    
    bool is_private_broker() const;

    gs::cnf::cnf_api_if* get_gs_cnf_api();
    
  protected:
    
    /// Iterates the observer callback map and makes the callbacks.
    /**
     * @param search     Search string for the map (parameter name or dummy for new parameters).
     * @param par_name   Name of the changed parameter.
     * @param value      New value of the changed parameter.
     */
    void make_create_param_callbacks(const std::string &search, const std::string &par_name, const std::string &value);

    /// Internal const function to be used by get_param and get_latest_write_originator
    cci::cnf::cci_base_param* get_param_const(const std::string &parname) const;

  protected:
      
    std::map<std::string, cci::cnf::cci_base_param*> m_mirrored_registry;
    
    std::string m_name;
    
    std::vector<internal_callback_forwarder*> fw_vec;

    /// Map to save an observer callback function pointer for each parameter (if needed).
    observerCallbackMap m_observer_callback_map;

    /// Map to save the latest write originator for implicit param sets (this is a HACK - should be within the database)
    implicitOriginatorMap m_implicit_originator_map;
  };
  
  
} // end namespace
} // end namespace

#endif
