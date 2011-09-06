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



#ifndef __CCI_CALLBACKS_H__
#define __CCI_CALLBACKS_H__

#include "core/cci_function.h"

__CCI_OPEN_CONFIG_NAMESPACE__
  
  /// Callback type
  /**
   * Used e.g. for callback registration and callbacks specifying the type.
   */
  enum callback_type {
    /// Callback prior to the value being read from
    pre_read,
    //post_read, // difficult/impossible to provide?
    /// Callback announcing someone tries to write the value (can be rejected by observers)
    reject_write,
    /// Callback prior to the value being written to (write will definetly be done - not being rejected)
    pre_write,
    /// Callback after to the value has been written to
    post_write,
    /// Callback for a param being created
    create_param,
    /// Callback for a param being destroyed
    destroy_param
  };
  
  /// Return status for returning callback functions
  enum callback_return_type {
    /// No special return status
    return_nothing,
    /// The callback function rejects a value change; may only be used in cci::cnf::reject_write callbacks
    /**
     * The calling parameter code must not apply the write. It should be an error if callback type is different from cci::cnf::reject_write.
     */
    return_value_change_rejected,
    /// Some other error @todo specify reaction to be performed in calling parameter code
    return_other_error
  };

  class cci_base_param;
  
  /// Typedef for the member function pointer for parameter callbacks.
  /**
   * Callback functions must have the signature: callback_return_type method_name(cci_base_param& changed_param, const callback_type& cb_reason)
   */
  typedef function2<callback_return_type, cci_base_param&, const callback_type&> param_callb_func_ptr;  // boost function portable syntax
  //typedef function<callback_return_type (cci_base_param&, const callback_type&)> callb_func_ptr;  // boost function prefered syntax
  
  /// Typedef for the member function pointer for broker callbacks.
  /**
   * Callback functions must have the signature: callback_return_type method_name(cci_base_param& changed_param, const callback_type& cb_reason)
   */
  typedef function2<callback_return_type, const std::string&, const callback_type&> broker_callb_func_ptr;  // boost function portable syntax
  //typedef function<callback_return_type (const std::string&, const callback_type&)> broker_callb_func_ptr;  // boost function prefered syntax


  /// Adapter base class which can be used to call a callback function.
  /**
   * Callback adapter to call function types param_callb_func_ptr (with param argument)
   * or broker_callb_func_ptr (with param name argument).
   *
   * This class also stores a param pointer to the caller (param or broker). 
   * When unregistering this callback with the function <code>unregister_at_parameter</code>
   * at the caller parameter, the pointer is set to NULL to avoid repeated unregistration
   * during destruction.
   *
   * Note the templates are only needed for compiling (to break
   * cyclic includes with cci_base_param and broker), 
   * Use the typedef callb_adapt to use this class.
   */
  template<class cci_base_param_T, class cci_cnf_broker_if_T>
  class callb_adapt_T
  {
  protected:

    /// Allows cci::cnf::cci_base_param to access the caller_param to set to NULL.
    //friend class cci_base_param_if; 
        
  public:
    
    /// Constructor to be used for registering functions of type param_callb_func_ptr.
    /**
     * @param _observer_ptr  Pointer to the object where the callback method should be called.
     * @param _func          Member function pointer to the callback method (must match signature cci::cnf::param_callb_func_ptr).
     * @param _caller_param  Pointer to the param that calls this adapter.
     */
    callb_adapt_T(void* _observer_ptr, param_callb_func_ptr _func, cci_base_param_T* _caller_param);

    /// Constructor to be used for registering functions of type broker_callb_func_ptr.
    /**
     * @param _observer_ptr   Pointer to the object where the callback method should be called.
     * @param _func           Member function pointer to the callback method (must match signature cci::cnf::broker_callb_func_ptr).
     * @param _caller_broker  Pointer to the broker that calls this adapter.
     */
    callb_adapt_T(void* _observer_ptr, broker_callb_func_ptr _func, cci_cnf_broker_if_T* _caller_object);
    
    ~callb_adapt_T();
    
    /// Unregisters at caller (e.g. parameter) and sets it to NULL to avoid unregistering during destruction.
    void unregister_at_parameter();
    
    /// Makes the callback.
    callback_return_type call(cci_base_param_T& changed_param, const callback_type& cb_reason);

    /// Makes the callback.
    callback_return_type call(const std::string& changed_param_name, const callback_type& cb_reason);

    /// Returns the observer (pointer to the object where the method should be called)
    void* get_observer();
    
    /// Returns the caller param pointer
    /**
     * @return Caller parameter. NULL if not existing.
     */
    cci_base_param_T* get_caller_param();

    /// Returns the caller broker pointer
    /**
     * @return Caller broker. NULL if not existing.
     */
    cci_base_param_T* get_caller_broker();
    
  protected:
    /// Pointer to the observer
    void* observer_ptr;
  public:   // TODO: more elegant; needs to be set to NULL when calling parameter is deleted
    /// Caller parameter
    cci_base_param_T *caller_param;
    cci_cnf_broker_if_T *caller_broker;
  protected:
    /// Function pointer to the callback method (must match signature cci::cnf::callb_func_ptr).
    param_callb_func_ptr  par_func;
    broker_callb_func_ptr bro_func;
    
  };

  typedef callb_adapt_T<cci_base_param, cci_cnf_broker_if> callb_adapt;
  
      
__CCI_CLOSE_CONFIG_NAMESPACE__

#include "cci_callbacks.hpp"

#endif
