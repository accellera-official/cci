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
//   This program is free software.
// 
//   If you have no applicable agreement with GreenSocs Ltd, this software
//   is licensed to you, and you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   If you have a applicable agreement with GreenSocs Ltd, the terms of that
//   agreement prevail.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//   02110-1301  USA 
// 
// ENDLICENSETEXT



#ifndef __CCI_CALLBACKS_H__
#define __CCI_CALLBACKS_H__

#include "cci_function.h"

namespace cci {
  
  /// Callback type
  /**
   * Used e.g. for callback registration and callbacks specifying the type.
   */
  enum callback_type {
    /// Callback prior to the value being read from
    pre_read,
    //post_read, // difficult/impossible to provide?
    /// Callback prior to the value being written to
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
    /// The callback function rejects a value change; may only be used in cci::pre_write callbacks
    /**
     * The calling parameter code must not apply the write. It should be an error if callback type is different from cci::pre_write.
     */
    return_value_change_rejected,
    /// Some other error @todo specify reaction to be performed in calling parameter code
    return_other_error
  };

  class cci_base_param;
  
  /// Typedef for the member function pointer.
  /**
   * Callback functions must have the signature: callback_return_type method_name(cci_base_param& changed_param, const callback_type& cb_reason)
   */
  typedef function2<callback_return_type, cci_base_param&, const callback_type&> callb_func_ptr;  // boost function portable syntax
  //typedef function<callback_return_type (cci_base_param&, const callback_type&)> callb_func_ptr;  // boost function prefered syntax
  
  /// Adapter base class which can be used to call an arbitrary parameter callback function (independently from template parameters).
  /**
   * Callback base class to allow access by anyone without
   * knowing the template parameter of the actually called object (callee).
   *
   * This class also stores a param pointer to the caller. When unregistering this 
   * callback with the function <code>unregister_at_parameter</code> at the caller 
   * parameter, the pointer is set to NULL to avoid repeated unregistration during
   * destruction.
   *
   * Note the template T_cci_base_param_if is only needed for compiling (to break
   * cyclic includes with cci_base_param), 
   * Use the typedef cci::callb_adapt_b to access this class.
   */
  template<class T_cci_base_param_if>
  class callb_adapt_B
  {
  protected:

    /// Allows cci::cci_base_param to access the caller_param to set to NULL.
    friend class cci_base_param; 
        
  public:
    
    /// Constructor with pointers to be saved in this object.
    /**
     * @param _observer_ptr    Pointer to the object where the callback method should be called.
     * @param _func  Member function pointer to the callback method (must match signature cci::callb_adapt::callb_func_ptr).
     * @param _caller_param  Pointer to the param that calls this adapter.
     */
    callb_adapt_B(void* _observer_ptr, callb_func_ptr _func, T_cci_base_param_if* _caller_param)
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
        /*bool succ = */caller_param->unregister_param_callback(this);
        //assert (succ && "Unregistering this callback at parameter failed!");
      }
    }
    
    /// Makes the callback, called by the base class callb_adapt_b.
    callback_return_type call(cci_base_param& changed_param, const callback_type& cb_reason) {
      if (func) {
        return func(changed_param, cb_reason);
      } else {
        SC_REPORT_ERROR(CCI_SC_REPORT_MSG_TYPE_PREFIX, "No callback registered yet.");
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
    T_cci_base_param_if* get_caller_param() {
      return caller_param;
    }
    
  protected:
    /// Pointer to the observer
    void* observer_ptr;
    
    /// Caller parameter
    T_cci_base_param_if *caller_param;
    
    /// Function pointer to the callback method (must match signature cci::callb_adapt::callb_func_ptr).
    callb_func_ptr func;
    
  };

  typedef callb_adapt_B<cci_base_param> callb_adapt_b;
  
      
} // end namespace cci

#endif
