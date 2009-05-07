//   OSCI CCI WG
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// 
// ENDLICENSETEXT


#ifndef __CCI_CALLBACKS_H__
#define __CCI_CALLBACKS_H__


namespace cci {
  
  enum callback_types {
    pre_read,
    post_read,
    pre_write,
    post_write,
    create_param,
    destroy_param
  };

    
  class cci_param_base;

  /// Adapter base class which can be used to call a arbitrary parameter callback function.
  /**
   * Base class for cci_callb_adapt to allow access by the back calling API without
   * knowing the template parameter.
   *
   * This class stores a void* pointer to the observer. (Needed by arrays to identify
   * observers in the maps.)
   *
   * This class stores a param pointer to the caller. When unregistering this 
   * callback with the function <code>unregister_at_parameter</code> at the caller 
   * parameter, the pointer is set to NULL to avoid repeated unregistration during
   * destruction.
   */
  template<class T_cci_param_base>
  class callb_adapt_B
  {
    // allows cci_param_base to access the caller_param to set to NULL
    friend class cci_param_base; 
    
  public:
    /// Constructor
    callb_adapt_B(void* _observer_ptr, T_cci_param_base *_caller_param)
    : observer_ptr(_observer_ptr),
      caller_param(_caller_param) {
#ifdef CCI_PARAM_CALLBACK_VERBOSE
        printf("callb_adapt_B: Create parameter callback adapter %p for caller parameter '%s'.\n", (void*)this, caller_param->get_name().c_str());
#endif
      }
    /// Virtual destructor, unregisters at caller parameter if not yet done.
    virtual ~callb_adapt_B() {
      unregister_at_parameter();
#ifdef CCI_PARAM_CALLBACK_VERBOSE
      printf("callb_adapt_B: Deleting parameter callback adapter %p (shared pointer deleted)\n", (void*)this);
#endif
    }
    
    /// Unregisters at caller parameter and sets it to NULL to avoid unregistering during destruction.
    void unregister_at_parameter() {
      if (caller_param != NULL) {
#ifdef CCI_PARAM_CALLBACK_VERBOSE
        printf("callb_adapt_B: Unregister parameter callback adapter %p at caller parameter '%s'.\n", (void*)this, caller_param->get_name().c_str());
#endif
        // remove this out of the parameter's callback list
        // Avoid repeated call during destruction by automatically setting caller_param = NULL
        caller_param->unregister_param_callback(this);
      }
    }
    
    /// Virtual call method to make the call in the template specialized class.
    virtual void call(T_cci_param_base& changed_param) = 0;
    /// Returns the observer (pointer to the object where the method should be called)
    void* get_observer() {
      return (void*) observer_ptr;
    }
    
    /// Returns the caller param pointer
    /**
     * @return Caller parameter. NULL if not existing.
     */
    T_cci_param_base* get_caller_param() {
      return caller_param;
    }
    
  protected:
    /// Pointer to the observer
    void* observer_ptr;
    
    /// Caller parameter
    T_cci_param_base *caller_param;
  };

  typedef callb_adapt_B<cci_param_base> callb_adapt_b;

    
  /// Template specialized adapter class which can be used to call an arbitrary parameter callback function.
  /**
   * This class saves an object pointer and a member function pointer to the object's
   * callback function and a pointer to the observer object. The call method is called
   * by the virtual call method of the base class when the API calls it.
   */
  template<class T>
  class callb_adapt
  : public callb_adapt_b
  {
  protected:
    
    /// Typedef for the member function pointer.
    /**
     * Callback functions must have the signature: void method_name(cci_param_base& changed_param)
     */
    typedef void (T::*callb_func_ptr)(cci_param_base& changed_param);

  public:

    /// Constructor with pointers to be saved in this object.
    /**
     * @param ob    Pointer to the object where the callback method should be called.
     * @param _ptr  Member function pointer to the callback method (must match signature CallbAdapt::callb_func_ptr).
     * @param _observer_ptr  void* pointer to the observer object (needed for unregistering).
     * @param _caller_param  Pointer to the param that calls this adapter.
     */
    callb_adapt(T* ob, callb_func_ptr _ptr, cci_param_base* _caller_param)
    : callb_adapt_b(ob, _caller_param) {
      obj = ob;
      ptr = _ptr;
    }

    /// Pointer to the object where the callback method should be called.
    T *obj;
    /// Member function pointer to the callback method (must match signature CallbAdapt::callb_func_ptr).
    callb_func_ptr ptr;

    /// Makes the callback, called by the base class CallbAdapt_b.
    void call(cci_param_base& changed_param) {
      (obj->*ptr)(changed_param);
    }
    
  };


} // end namespace cci

#endif
