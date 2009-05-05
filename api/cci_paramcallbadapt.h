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


#ifndef __CCI_PPARAMCALLBADAPT_H__
#define __CCI_PPARAMCALLBADAPT_H__

namespace cci {

  
class cci_param_base;

/// Adapter base class which can be used to call a arbitrary parameter callback function.
/**
 * Base class for CallbAdapt to allow access by the back calling API without
 * knowing the template parameter.
 *
 * This is the adapter for callback methods which get a cci_param_base reference
 * (instead of strings for name and value), see gs::cnf::CallbAdapt_b.
 *
 * This class stores a void* pointer to the observer. (Needed by arrays to identify
 * observers in the maps.)
 *
 * This class stores a param pointer to the caller. When unregistering this 
 * callback with the function <code>unregister_at_parameter</code> at the caller 
 * parameter, the pointer is set to NULL to avoid repeated unregisteration during
 * destruction.
 */
template<class T_cci_param_base>
class ParamCallbAdapt_B
{
  // allows cci_param_base to access the caller_param to set to NULL
  friend class cci_param_base; 
  
public:
  /// Constructor
  ParamCallbAdapt_B(void* _observer_ptr, T_cci_param_base *_caller_param)
  : observer_ptr(_observer_ptr),
    caller_param(_caller_param) {
#ifdef GS_PARAM_CALLBACK_VERBOSE
      printf("ParamCallbAdapt_b: Create parameter callback adapter %p for caller parameter '%s'.\n", (void*)this, caller_param->getName().c_str());
#endif
    }
  /// Virtual destructor, unregisters at caller parameter if not yet done.
  virtual ~ParamCallbAdapt_B() {
    unregister_at_parameter();
#ifdef GS_PARAM_CALLBACK_VERBOSE
    printf("ParamCallbAdapt_b: Deleting parameter callback adapter %p (shared pointer deleted)\n", (void*)this);
#endif
  }
  
  /// Unregisters at caller parameter and sets it to NULL to avoid unregistering during destruction.
  void unregister_at_parameter() {
    if (caller_param != NULL) {
#ifdef GS_PARAM_CALLBACK_VERBOSE
      printf("ParamCallbAdapt_b: Unregister parameter callback adapter %p at caller parameter '%s'.\n", (void*)this, caller_param->getName().c_str());
#endif
      // remove this out of the parameter's callback list
      // Avoid repeated call during destruction by automatically setting caller_param = NULL
      caller_param->unregisterParamCallback(this);
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

typedef ParamCallbAdapt_B<cci_param_base> ParamCallbAdapt_b;

  
/// Template specialized adapter class which can be used to call a arbitrary parameter callback function.
/**
 * This class saves an object pointer and a member function pointer to the object's
 * callback function and a pointer to the observer object. The call method is called
 * by the virtual call method of the base class when the API calls it.
 *
 * This is the adapter for callback methods which get a cci_param_base reference
 * (instead of strings for name and value), see gs::cnf::CallbAdapt.
 */
template<class T>
class ParamCallbAdapt
: public ParamCallbAdapt_b
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
  ParamCallbAdapt(T *ob, callb_func_ptr _ptr, void* _observer_ptr, cci_param_base *_caller_param)
  : ParamCallbAdapt_b(_observer_ptr, _caller_param) {
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
