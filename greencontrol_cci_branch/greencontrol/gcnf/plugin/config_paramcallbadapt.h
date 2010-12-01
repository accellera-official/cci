//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doxygen comments

#ifndef __config_ParamCallbAdapt_h__
#define __config_ParamCallbAdapt_h__

#include "greencontrol/gcnf/plugin/config_globals.h"

namespace gs {
namespace cnf {

  
class gs_param_base;

/// Base class for callback adapters with unique ID for all virtually derived callback adapters
struct ParamCallbUniqueID
{
  /// Internal function returning a unique id
  unsigned int create_id() {
    static unsigned int global_cb_id = 0;
    return ++global_cb_id;
  }
};
  
/// Adapter base class which can be used to call a arbitrary parameter callback function (WITHOUT CALLBACK REASON).
/**
 * Base class for CallbAdapt to allow access by the back calling API without
 * knowing the template parameter.
 *
 * This is the adapter for callback methods which get a gs_param_base reference
 * (instead of strings for name and value), see gs::cnf::CallbAdapt_b.
 *
 * This class stores a void* pointer to the observer. (Needed by arrays to identify
 * observers in the maps.)
 *
 * This class stores a param pointer to the caller. When unregistering this 
 * callback with the function <code>unregister_at_parameter</code> at the caller 
 * parameter, the pointer is set to NULL to avoid repeated unregistration during
 * destruction.
 *
 * A unique ID per callback adapter can be used for debug outputs etc.
 */
template<class T_gs_param_base>
class ParamCallbAdapt_B
: protected virtual ParamCallbUniqueID
{
  // allows gs_param_base to access the caller_param to set to NULL
  friend class gs_param_base;
  
public:
  /// Constructor
  ParamCallbAdapt_B(void* _observer_ptr, T_gs_param_base *_caller_param)
  : observer_ptr(_observer_ptr),
    caller_param(_caller_param) {
      m_id = create_id();
#ifdef GS_PARAM_CALLBACK_VERBOSE
      printf("ParamCallbAdapt_b: Create parameter callback adapter %u for caller parameter '%s'.\n", m_id, caller_param->getName().c_str());
#endif
    }
  /// Virtual destructor, unregisters at caller parameter if not yet done.
  virtual ~ParamCallbAdapt_B() {
#ifdef GS_PARAM_CALLBACK_VERBOSE
    printf("ParamCallbAdapt_b: Deleting parameter callback adapter %u (shared pointer deleted)\n", m_id);
#endif
    unregister_at_parameter();
  }
  
  /// Unregisters at caller parameter and sets it to NULL to avoid unregistering during destruction.
  void unregister_at_parameter() {
    if (caller_param != NULL) {
#ifdef GS_PARAM_CALLBACK_VERBOSE
      printf("ParamCallbAdapt_b: Unregister parameter callback adapter %u at caller parameter '%s'.\n", m_id, caller_param->getName().c_str());
#endif
      // remove this out of the parameter's callback list
      // Avoid repeated call during destruction by automatically setting caller_param = NULL
      caller_param->unregisterParamCallback(this);
    }
  }
  
  /// Virtual call method to make the call in the template specialized class.
  virtual callback_return_type call(T_gs_param_base& changed_param, callback_type& reason) = 0;
  /// Returns the observer (pointer to the object where the method should be called)
  void* get_observer() {
    return (void*) observer_ptr;
  }
  
  /// Returns the caller param pointer
  /**
   * @return Caller parameter. NULL if not existing.
   */
  T_gs_param_base* get_caller_param() {
    return caller_param;
  }
  
  /// Get the unique ID of this callback adapter
  unsigned int get_id() {
    return m_id;
  }
  
protected:

  /// Pointer to the observer
  void* observer_ptr;
  
  /// Caller parameter
  T_gs_param_base *caller_param;
  
  unsigned int m_id;
};

/// Typedef for ParamCallbAdapt_B with the only allowed template specialization
typedef ParamCallbAdapt_B<gs_param_base> ParamCallbAdapt_b;

  
/// Template specialized adapter class which can be used to call a arbitrary parameter callback function WITH CALLBACK REASON.
/**
 * This class saves an object pointer and a member function pointer to the object's
 * callback function and a pointer to the observer object. The call method is called
 * by the virtual call method of the base class when the API calls it.
 *
 * This is the adapter for callback methods which get a gs_param_base reference
 * (instead of strings for name and value), see gs::cnf::CallbAdapt.
 */
template<class T>
class ParamCallbAdapt
: public ParamCallbAdapt_b
{
protected:
  
  /// Typedef for the member function pointer.
  /**
   * Callback functions must have the signature: void method_name(gs_param_base& changed_param)
   */
  typedef void (T::*callb_func_ptr)(gs_param_base& changed_param);

public:

  /// Constructor with pointers to be saved in this object.
  /**
   * @param ob    Pointer to the object where the callback method should be called.
   * @param _ptr  Member function pointer to the callback method (must match signature CallbAdapt::callb_func_ptr).
   * @param _observer_ptr  void* pointer to the observer object (needed for unregistering).
   * @param _caller_param  Pointer to the param that calls this adapter.
   */
  ParamCallbAdapt(T *ob, callb_func_ptr _ptr, void* _observer_ptr, gs_param_base *_caller_param)
  : ParamCallbAdapt_b(_observer_ptr, _caller_param) {
    obj = ob;
    ptr = _ptr;
  }

  /// Pointer to the object where the callback method should be called.
  T *obj;
  /// Member function pointer to the callback method (must match signature CallbAdapt::callb_func_ptr).
  callb_func_ptr ptr;

  /// Makes the callback, called by the base class CallbAdapt_b.
  callback_return_type call(gs_param_base& changed_param, callback_type& reason) {
    (obj->*ptr)(changed_param);
    return return_nothing;
  }
  
};

  /// Template specialized adapter class which can be used to call a arbitrary parameter callback function.
  /**
   * This class saves an object pointer and a member function pointer to the object's
   * callback function and a pointer to the observer object. The call method is called
   * by the virtual call method of the base class when the API calls it.
   *
   * This is the adapter for callback methods which get a gs_param_base reference
   * (instead of strings for name and value), see gs::cnf::CallbAdapt.
   */
  template<class T>
  class ParamTypedCallbAdapt
  : public ParamCallbAdapt_b
  {
  protected:
    
    /// Typedef for the member function pointer.
    /**
     * Callback functions must have the signature: void method_name(gs_param_base& changed_param, callback_type reason)
     */
    typedef callback_return_type (T::*callb_func_w_reason_ptr)(gs_param_base& changed_param, callback_type reason);
    
  public:
    
    /// Constructor with pointers to be saved in this object.
    /**
     * @param ob    Pointer to the object where the callback method should be called.
     * @param _ptr  Member function pointer to the callback method (must match signature CallbAdapt::callb_func_ptr).
     * @param _observer_ptr  void* pointer to the observer object (needed for unregistering).
     * @param _caller_param  Pointer to the param that calls this adapter.
     */
    ParamTypedCallbAdapt(T *ob, callb_func_w_reason_ptr _ptr, void* _observer_ptr, gs_param_base *_caller_param)
    : ParamCallbAdapt_b(_observer_ptr, _caller_param)
#ifdef GCNF_AVOID_NESTED_CALLBACKS_IN_CALLBACK_ADAPTERS
    , m_calling(false) 
#endif
    {
      obj = ob;
      ptr = _ptr;
    }
    
    /// Pointer to the object where the callback method should be called.
    T *obj;
    /// Member function pointer to the callback method (must match signature CallbAdapt::callb_func_ptr).
    callb_func_w_reason_ptr ptr;
// See comment in config_globals.h
#ifdef GCNF_AVOID_NESTED_CALLBACKS_IN_CALLBACK_ADAPTERS
    /// To avoid multiple (nested) callbacks 
    bool m_calling;
#endif
    
    /// Makes the callback, called by the base class CallbAdapt_b.
    callback_return_type call(gs_param_base& changed_param, callback_type& reason) {
      callback_return_type ret = return_nothing;
#ifdef GCNF_AVOID_NESTED_CALLBACKS_IN_CALLBACK_ADAPTERS
      if (!m_calling) {
        m_calling = true; // To avoid multiple (nested) callbacks
#endif
        ret = (obj->*ptr)(changed_param, reason);
#ifdef GCNF_AVOID_NESTED_CALLBACKS_IN_CALLBACK_ADAPTERS
        m_calling = false;
      }
      else {
        GS_PARAM_CALLBACK_DUMP_WITHNAME("call adapter "<<get_id(), " "<< callback_type_to_string(reason) <<" callback omitted.");
      }
#endif
      return ret;
    }
    
  };

} // end namespace cnf
} // end namespace gs

#endif
