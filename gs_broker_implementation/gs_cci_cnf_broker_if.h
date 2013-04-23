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



#ifndef __GS_CCI_CNF_BROKER_IF_H__
#define __GS_CCI_CNF_BROKER_IF_H__

#include <cci>


__CCI_OPEN_CONFIG_NAMESPACE__


  /// Configuration broker interface with originator information.
  class gs_cci_cnf_broker_if
  {

  public:

    virtual cci_cnf_broker_if& get_accessor(const cci_originator& originator) = 0;

    // Destructor
    virtual ~gs_cci_cnf_broker_if() { };
    
    virtual const char* name() const = 0;

    virtual void set_init_value(const std::string &parname, const std::string &json_value) = 0;

    virtual const cci_originator* get_latest_write_originator(const std::string &parname) const = 0;

    virtual void lock_init_value(const std::string &parname) = 0;
    
    virtual const std::string get_json_string(const std::string &parname) = 0;
    
    virtual const std::string get_json_string_keep_unused(const std::string &parname) = 0;
    
    virtual cci_base_param* get_param(const std::string &parname) = 0;
    
    virtual const std::vector<std::string> get_param_list() = 0;
    
    virtual const std::vector<std::string> get_param_list(const std::string& pattern) = 0;
    
    virtual bool exists_param(const std::string &parname) = 0;
    
    virtual bool is_used(const std::string &parname) = 0;

    virtual shared_ptr< callb_adapt> register_callback(const std::string& parname, const callback_type type, shared_ptr< callb_adapt> callb) = 0;

    virtual void unregister_all_callbacks(void* observer) = 0;

    virtual bool unregister_param_callback(callb_adapt* callb) = 0;
    
    virtual bool has_callbacks(const std::string& parname) = 0;
    
    virtual void add_param(cci_base_param* par) = 0;
    
    virtual void remove_param(cci_base_param* par) = 0;
    
    virtual const std::vector<cci_base_param*> get_params(const std::string& pattern) = 0;
    
    //virtual void set_alias(std::string& orig_parname, std::string& alias_parname) = 0;
    
    virtual bool is_private_broker() const = 0;

  };

      
__CCI_CLOSE_CONFIG_NAMESPACE__

#endif
