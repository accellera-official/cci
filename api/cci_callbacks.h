/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2009 by all Contributors.
  All Rights reserved.

  Developed by GreenSocs : http://www.greensocs.com/
   Christian Schroeder, schroeder@eis.cs.tu-bs.de
   Mark Burton, mark@greensocs.com

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/


#ifndef __CCI_CALLBACKS_H__
#define __CCI_CALLBACKS_H__

#include <boost/function.hpp>

namespace cci {
  
  enum callback_type {
    pre_read,
    //post_read, // difficult/impossible to provide?
    pre_write,
    post_write,
    create_param,
    destroy_param
  };
  
  enum callback_return_type {
    return_nothing,
    return_value_change_rejected, // may only be used in pre_write callbacks
    return_other_error
  };

  class cci_base_param;
  
  /// Typedef for the member function pointer.
  /**
   * Callback functions must have the signature: callback_return_type method_name(cci_base_param& changed_param, const callback_type& cb_reason)
   */
  typedef boost::function2<callback_return_type, cci_base_param&, const callback_type&> callb_func_ptr;  // boost function portable syntax
  //typedef boost::function<callback_return_type (cci_base_param&, const callback_type&)> callb_func_ptr;  // boost function prefered syntax
  
  /// Adapter base class which can be used to call an arbitrary parameter callback function.
  /**
   * Base class for cci_callb_adapt to allow access by anyone without
   * knowing the template parameter.
   *
   * This class stores a param pointer to the caller. When unregistering this 
   * callback with the function <code>unregister_at_parameter</code> at the caller 
   * parameter, the pointer is set to NULL to avoid repeated unregistration during
   * destruction.
   */
  template<class T_cci_base_param>
  class callb_adapt_B
  {
  protected:

    // allows cci_base_param to access the caller_param to set to NULL
    friend class cci_base_param; 
        
  public:
    
    /// Constructor with pointers to be saved in this object.
    /**
     * @param _observer_ptr    Pointer to the object where the callback method should be called.
     * @param _func  Member function pointer to the callback method (must match signature cci::callb_adapt::callb_func_ptr).
     * @param _caller_param  Pointer to the param that calls this adapter.
     */
    callb_adapt_B(void* _observer_ptr, callb_func_ptr _func, cci_base_param* _caller_param)
    : observer_ptr(_observer_ptr)
    , caller_param(_caller_param)
    , func(_func)
    {
#ifdef CCI_PARAM_CALLBACK_VERBOSE
      if (caller_param) printf("callb_adapt_B: Create parameter callback adapter %p for caller parameter '%s'.\n", (void*)this, caller_param->get_name().c_str());
      else printf("callb_adapt_B: Create parameter callback adapter %p for no parameter.\n", (void*)this);
#endif
    }
    
    ~callb_adapt_B() {
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
        bool succ = caller_param->unregister_param_callback(this);
        //assert (succ && "Unregistering this callback at parameter failed!");
      }
    }
    
    /// Makes the callback, called by the base class callb_adapt_b.
    callback_return_type call(cci_base_param& changed_param, const callback_type& cb_reason) {
      if (func) {
        return func(changed_param, cb_reason);
      } else {
        SC_REPORT_ERROR(OSCI_CCI_SC_REPORT_IDENT, "No callback registered yet.");
      }      
      return return_nothing;
    }

    /// Returns the observer (pointer to the object where the method should be called)
    void* get_observer() {
      return (void*) observer_ptr;
    }
    
    /// Returns the caller param pointer
    /**
     * @return Caller parameter. NULL if not existing.
     */
    T_cci_base_param* get_caller_param() {
      return caller_param;
    }
    
  protected:
    /// Pointer to the observer
    void* observer_ptr;
    
    /// Caller parameter
    T_cci_base_param *caller_param;
    
    /// Boost function pointer to the callback method (must match signature cci::callb_adapt::callb_func_ptr).
    callb_func_ptr func;
    
  };

  typedef callb_adapt_B<cci_base_param> callb_adapt_b;
  
      
} // end namespace cci

#endif
