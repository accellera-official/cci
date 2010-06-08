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


#ifndef __CCI_BASE_PARAM_H___
#define __CCI_BASE_PARAM_H___

__CCI_OPEN_CONFIG_NAMESPACE__

  // The shell class to make instantiation and access easier
  class cci_base_param : virtual public cci_base_param_if
  {
  public:
    //explicit cci_base_param() { }
    
    virtual ~cci_base_param() { }
    
    virtual void json_deserialize(const std::string& json_string) {get_pImpl()->json_deserialize(json_string); }
    virtual const std::string& json_serialize() const {return get_pImpl()->json_serialize();}
    virtual const basic_param_type get_basic_type() const {return get_pImpl()->get_basic_type();}
    virtual void set_value(const cci_value& val) {get_pImpl()->set_value(val);}
    virtual cci_value get_value() {return get_pImpl()->get_value();}
    virtual void set_documentation(const std::string& doc) {get_pImpl()->set_documentation(doc);}
    virtual std::string get_documentation() const {return get_pImpl()->get_documentation();}
    virtual bool is_default_value() {return get_pImpl()->is_default_value();}
    virtual bool is_invalid_value() {return get_pImpl()->is_invalid_value();}
    virtual void set_invalid_value() {get_pImpl()->set_invalid_value();}
    virtual bool is_initial_value() {return get_pImpl()->is_initial_value();};
    virtual const std::string& get_name() const {return get_pImpl()->get_name();}
    virtual cci::shared_ptr<callb_adapt_b> register_callback(const callback_type type, void* observer, callb_func_ptr function) {return get_pImpl()-> register_callback(type, observer, function);}
    virtual cci::shared_ptr<callb_adapt_b> register_callback(const callback_type type, cci::shared_ptr<callb_adapt_b> callb) {return get_pImpl()-> register_callback(type, callb);}
    virtual void unregister_all_callbacks(void* observer) {get_pImpl()->unregister_all_callbacks(observer);}
    virtual bool unregister_param_callback(cci::shared_ptr<callb_adapt_b> callb) {return get_pImpl()->unregister_param_callback(callb);}
    virtual bool unregister_param_callback(callb_adapt_b* callb) {return get_pImpl()->unregister_param_callback(callb);}
    virtual bool has_callbacks(){return get_pImpl()->has_callbacks();}
    virtual bool lock(void* pwd = NULL){return get_pImpl()->lock();}
    virtual bool unlock(void* pwd = NULL){return get_pImpl()->unlock();}
    virtual bool locked() const {return get_pImpl()->locked();}
    
  private:
    virtual cci_base_param_if* get_pImpl() const = 0;
  };

__CCI_CLOSE_CONFIG_NAMESPACE__
  
#endif
