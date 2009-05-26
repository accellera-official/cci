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


#ifndef __CCI_PARAM_BASE_H__
#define __CCI_PARAM_BASE_H__


#include <string>
#include <iostream>
#include <map>
#include <set>

#include <boost/shared_ptr.hpp>


namespace cci {

  template <typename T> 
  class cci_param;

  /// Base class for all cci_param template specializations.
  /**
   * Features:
   * - Name of the parameter,
   * - JSON (de)serialize functions
   * - Callback handling
   */
  class cci_param_base
  : public sc_core::sc_object
  {

  public:

    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Construction / Destruction   ///////////////////// //

    /// Proposed constructor with (local or hierarchical) name
    /**
     * - Sets the hierarchical parameter name (default name if name is empty).
     *   (If the parent_array is != NULL: 
     *            hierarchical name is the parent's name + local name)
     * - Sets the API pointer.
     * - Explicit constructor to avoid implicit construction of parameters.
     *
     * @param n  The local (or full hierarchical) parameter name (local: should not but may include points) 
     *           (local: unique inside a module, hierarchical: unique in the system).
     *           May be empty: name will be chosen automatically.
     * @param force_top_level_name If the given name n should be a top-level name (then no prefeix is attached to the name),
     *                             default false,
     *                             Be carefull in using this.
     */
    //explicit cci_param_base(const std::string& n,
    //                        const bool force_top_level_name = false);
    
    /// Destructor.
    virtual ~cci_param_base() { }
    
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////   Set and Get with JSON String Representation   //////////// //
    
    
    /// Sets the value of this parameter given by a JSON string.
    /** 
     * @param str the new value represented as a JSON string.
     * @return If setting was successful.
     */
    virtual bool json_deserialize(const std::string& str) = 0;
    
    /// Get the JSON string representation of this parameter's value.
    /**
     * @return  The value of this parameter represented as a JSON string.
     */
    virtual const std::string& json_serialize() const = 0;
    
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   JSON Data Type and access   ////////////////////// //

    
    /// Returns a type this parameter can be converted to (which is not the actual parameter type)
    virtual const Param_JSON_type get_JSON_type() const { return partype_not_available; }
    
#define CCI_NOT_SUPPORTED_WRN SC_REPORT_WARNING(OSCI_CCI_SC_REPORT_IDENT, "Not supported for this parameter type!")

    /// Set the parameter value by trying to convert the given number to the param value.
    /**
     * This function is optionally implemented, check get_JSON_type() 
     * if supported for this param.
     *
     * The function should return true only if the value was 
     * applied successfully to the parameter, e.g without an overflow. 
     * In cases of overflows and not other mismatches the function 
     * should return false and shall not change the parameter!
     *
     * @param value  Value which the parameter should be set to.
     * @return       If the setting was successfull.
     */
    virtual bool set_number(const long long value)   { CCI_NOT_SUPPORTED_WRN; return false; }
    /// Set the parameter value by trying to convert the given number to the param value.
    /** Details @see cci_param_base::set_number(const long long value) If the parameter does not implement this, it will call the set_number function. */
    virtual bool set_double(const double value)      { long long llval = (long long)value; return set_number(llval); }
    /// Set the parameter value by trying to convert the given string to the param value.
    /** Details @see cci_param_base::set_number(const long long value) */
    virtual bool set_string(const std::string value) { CCI_NOT_SUPPORTED_WRN; return false; }
    /// Set the parameter value by trying to convert the given bool to the param value.
    /** Details @see cci_param_base::set_number(const long long value) */
    virtual bool set_bool(const bool value)          { CCI_NOT_SUPPORTED_WRN; return false; }

    /// Get the parameter value by trying to convert it to the given number.
    /**
     * This function is optionally implemented, check get_JSON_type() 
     * if supported for this param.
     *
     * The function should return true only if the value was copied/converted
     * successfully to the given retvalue parameter, e.g without an overflow. 
     * In cases of overflows and not other mismatches the function 
     * should return false and shall not change the retalue!
     *
     * @param value  Value which the parameter should be copied to.
     * @return       If the getting was successfull.
     */
    virtual bool get_number(long long& retvalue)     { CCI_NOT_SUPPORTED_WRN; return false; }
    /// Get the parameter value by trying to convert it to the given number.
    /** Details @see cci_param_base::get_number(long long& retvalue). If the parameter does not implement this, it will call the get_number function. */
    virtual bool get_double(double& retvalue)        { long long llval; bool res=set_number(llval); if (res) retvalue=(double)llval; return res; }
    /// Get the parameter value by trying to convert it to the given string.
    /** Details @see cci_param_base::get_number(long long& retvalue) */
    virtual bool get_string(std::string& retvalue)   { CCI_NOT_SUPPORTED_WRN; return false; }
    /// Get the parameter value by trying to convert it to the given bool.
    /** Details @see cci_param_base::get_number(long long& retvalue) */
    virtual bool get_bool(bool& retvalue)            { CCI_NOT_SUPPORTED_WRN; return false; }
    
    
    // //////////////////////////////////////////////////////////////////// //
    // /////////////////////   Miscellaneous   //////////////////////////// //

    
    /// Get the name of this parameter.
    /**
     * @return   Name of the parameter.
     */
    virtual const std::string& get_name() const = 0;
   
    
    // //////////////////////////////////////////////////////////////////// //
    // /////////////////   Callback Handling   //////////////////////////// //
    
    
    /// Registers an observer callback function (with the signature of callback_func_ptr).
    /**
     * For further callback types @see cci::cci_cnf_api::register_callback
     *
     * Inside the callback functions no waits, next_trigger, get_current_process_handle are allowed!
     *
     * The user may register any methods as callback functions which have
     * the following signature:
     * \code
     * void method_name(cci_param_base& changed_param);
     * \endcode
     *
     * The registered callbacks should be unregistered latest during destruction.
     *
     * Usage example:
     * \code
     * class MyIP_Class
     * : public sc_core::sc_module
     * {
     * public:
     *   // some code [...]
     *   
     *   cci_param<int> my_param;
     *
     *   MyIP_Class(sc_core::sc_module_name name)
     *    : sc_core::sc_module(name),
     *      my_param("my_param", 10) 
     *   { //...
     *   }
     *
     *   ~MyIP_Class() {
     *     // In this example the callback adapters are deleted automatically 
     *     // because the shared pointers are destroyed here.
     *   }
     *
     *   clean_my_callbacks() {
     *     // will unregister the callbacks being registered within this module for the parameter my_param
     *     my_param.unregister_all_callbacks(this);
     *   }
     *
     *   // Example code to register callback function
     *   void main_action() {
     *     // some code, parameters etc...
     *     p1cb = my_param.register_callback(cci::post_write   , this, &MyIP_Class::config_callback);
     *     p2cb = my_param.register_callback(cci::destroy_param, this, &MyIP_Class::config_callback);
     *   }
     *
     *   // Callback function with default signature.
     *   void config_callback(cci_param_base& changed_param) {
     *     // some action
     *   }
     * protected:
     *   boost::shared_ptr<callb_adapt_b> p1cb;
     *   boost::shared_ptr<callb_adapt_b> p2cb;
     * };
     * \endcode
     *
     * @param type        Type of the callback.
     * @param observer    Pointer to the observing object (the one being called).
     * @param callb_func  Function pointer to the function being called.
     * @return            boost shared pointer to the callback adapter (e.g. to be used for unregister calls).
     */
    template <typename T>
    boost::shared_ptr<callb_adapt_b> register_callback(const callback_types type, T* observer, void (T::*callb_func_ptr)(cci_param_base& changed_param)) {
      // call the pure virtual function, independent from template T
      return register_callback(boost::shared_ptr< callb_adapt_b>(new callb_adapt<T>(observer, callb_func_ptr, this)));
    }
    
    /// Function handling the callback (without template)
    virtual boost::shared_ptr<callb_adapt_b> register_callback(boost::shared_ptr< callb_adapt_b> callb) = 0;
    
    
    /// Unregisters all callbacks (within this parameter) for the specified observer object (e.g. sc_module). 
    /**
     * observer = NULL should be an error (and not mean unregister all callbacks) 
     *                 to avoid accidental unregistration of e.g. callbacks initiated 
     *                 by the/a tool.
     * @param observer   Pointer to the observer module who did register parameter callbacks.
     */
    virtual void unregister_all_callbacks(void* observer) = 0;
    
    /// Unregisters the callback and (default) deletes the callback adapter.
    /**
     * Unregisters (only) the given callback.
     *
     * The callback adapter is NOT deleted explicitely because this
     * should be done by the surrounding shared pointer!
     *
     * This may be used by a user module which stored the shared pointer to a
     * specific callback or by the destructor of the param callback adapter.
     *
     * @param callb  Parameter callback adapter
     * @return       If the callback adapter existed in this parameter.
     */
    virtual bool unregister_param_callback(callb_adapt_b* callb) = 0;
    
    /// Returns if the parameter has registered callbacks
    virtual bool has_callbacks() = 0;
    
  };

} // namespace cci

#endif
