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


#ifndef __CCI_BASE_PARAM_H__
#define __CCI_BASE_PARAM_H__


#include <string>
#include <iostream>
#include <map>
#include <set>

#include <boost/shared_ptr.hpp>

#include "cci_value.h"


namespace cci {

  template <typename T> 
  class cci_param;

  /// Base param which stores string values (can be instantiated) or base class for all cci_param template specializations.
  /**
   * Features:
   * - Name of the parameter,
   * - stores value untyped as string representation
   * - allows generic typed and list access
   * - JSON (de)serialize functions
   * - Callback handling
   */
  class cci_base_param
  {

  public:

    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Construction / Destruction   ///////////////////// //

    /// Proposed constructor with (local or hierarchical) name, to be called by derived param
    /**
     * - Sets the hierarchical parameter name (default name if name is empty).
     * - Explicit constructor to avoid implicit construction of parameters.
     *
     * @param n  The local (or full hierarchical) parameter name (local: should not but may include points) 
     *           (local: unique inside a module, hierarchical: unique in the system).
     *           May be empty: name will be chosen automatically.
     * @param force_top_level_name If the given name n should be a top-level name (then no prefeix is attached to the name),
     *                             default false,
     *                             Be carefull in using this.
     */
    //explicit cci_base_param(const std::string& n,
    //                        const bool force_top_level_name = false);

    /// Proposed constructor with (local or hierarchical) name and string value, to be called by user
    /**
     * - Sets the hierarchical parameter name (default name if name is empty).
     * - Explicit constructor to avoid implicit construction of parameters.
     *
     * @param name  The local (or full hierarchical) parameter name (local: should not but may include points) 
     *              (local: unique inside a module, hierarchical: unique in the system).
     *              May be empty: name will be chosen automatically.
     * @param value The string value that should be stored by this base param (NOT use when having typed params!)
     * @param force_top_level_name If the given name n should be a top-level name (then no prefeix is attached to the name),
     *                             default false,
     *                             Be carefull in using this.
     */
    //explicit cci_base_param(const std::string& name,
    //                        const std::string& value,
    //                        const bool force_top_level_name = false);
    
    /// Destructor.
    virtual ~cci_base_param() { }
    
    
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

    
    /// Returns a basic type this parameter can be converted to (which is not necessarily the actual parameter type)
    virtual const basic_param_type get_basic_type() const { return partype_not_available; }
    
#define CCI_NOT_SUPPORTED_WRN SC_REPORT_WARNING(OSCI_CCI_SC_REPORT_IDENT, "Not supported for this parameter type!")

    /// Set the parameter value by trying to convert the given number to the param value.
    /**
     * This function is optionally implemented, check get_basic_type() 
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
    virtual set_param_error_type set_number(const sc_dt::uint64 value) { CCI_NOT_SUPPORTED_WRN; return set_param_failed; }
    /// Set the parameter value by trying to convert the given number to the param value.
    /** Details @see cci_base_param::set_number(const sc_dt::uint64 value) If the parameter does not implement this, it will call the set_number function. */
    virtual set_param_error_type set_double(const double value)        { sc_dt::uint64 llval = (sc_dt::uint64)value; return set_number(llval); }
    /// Set the parameter value by trying to convert the given string to the param value.
    /** Details @see cci_base_param::set_number(const sc_dt::uint64 value) */
    virtual set_param_error_type set_string(const std::string& value)   { CCI_NOT_SUPPORTED_WRN; return set_param_failed; }
    /// Set the parameter value by trying to convert the given bool to the param value.
    /** Details @see cci_base_param::set_number(const sc_dt::uint64 value) */
    virtual set_param_error_type set_bool(const bool value)            { CCI_NOT_SUPPORTED_WRN; return set_param_failed; }

    /// Get the parameter value by trying to convert it to the given number.
    /**
     * This function is optionally implemented, check get_basic_type() 
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
    virtual get_param_error_type get_number(sc_dt::uint64& retvalue) { CCI_NOT_SUPPORTED_WRN; return get_param_failed; }
    /// Get the parameter value by trying to convert it to the given number.
    /** Details @see cci_base_param::get_number(sc_dt::uint64& retvalue). If the parameter does not implement this, it will call the get_number function. */
    virtual get_param_error_type get_double(double& retvalue)        { sc_dt::uint64 llval; get_param_error_type res=get_number(llval); if (res) retvalue=(double)llval; return res; }
    /// Get the parameter value by trying to convert it to the given string.
    /** Details @see cci_base_param::get_number(sc_dt::uint64& retvalue) */
    virtual get_param_error_type get_string(std::string& retvalue)   { CCI_NOT_SUPPORTED_WRN; return get_param_failed; }
    /// Get the parameter value by trying to convert it to the given bool.
    /** Details @see cci_base_param::get_number(sc_dt::uint64& retvalue) */
    virtual get_param_error_type get_bool(bool& retvalue)            { CCI_NOT_SUPPORTED_WRN; return get_param_failed; }
    
    //
    // Alternatively??
    //
    
    /// Set the parameter's value to the given one
    /**
     * @param val This value is either (in the case of a pure basic param) converted into a JSON string and stored in the base param or (in the case of a typed parameter) into the actual data type
     * @return    If the setting was successfull.
     */
    virtual set_param_error_type set_value(const cci_value& val) = 0;
    
    /// Get the parameter's value
    /**
     * @return val This value is either (in the case of a pure basic param) converted from the JSON string or (in the case of a typed parameter) from the actual data type
     */
    virtual cci_value get_value() = 0;
    
    // //////////////////////////////////////////////////////////////////// //
    // /////////////////////   Miscellaneous   //////////////////////////// //

    
    /// Get the name of this parameter.
    /**
     * @return   Name of the parameter.
     */
    virtual const std::string& get_name() const = 0;
   
    
    // //////////////////////////////////////////////////////////////////// //
    // /////////////////   Callback Handling   //////////////////////////// //
    
    
    /// Registers an observer callback function (a boost function with the signature of callback_func_ptr).
    /**
     * For further callback types @see cci::cci_cnf_api::register_callback
     *
     * Inside the callback functions no waits, next_trigger, get_current_process_handle are allowed!
     *
     * The user may register any boost function for callbacks which have
     * the following signature:
     * \code
     * boost::function<void (cci_base_param& changed_param)>
     * \endcode
     * e.g.
     * \code
     * void method_name(cci_base_param& changed_param);
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
     *     p1cb = my_param.register_callback(cci::post_write   , this, boost::bind(&MyIP_Class::config_callback, this, _1, _2));
     *     p2cb = my_param.register_callback(cci::destroy_param, this, boost::bind(&MyIP_Class::config_callback, this, _1, _2));
     *   }
     *
     *   // Callback function with default signature.
     *   void config_callback(cci_base_param& changed_param, const callback_type& cb_reason) {
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
     * @param function    Boost function pointer to the function being called.
     * @return            Boost shared pointer to the callback adapter (e.g. to be used for unregister calls).
     */
    boost::shared_ptr<callb_adapt_b> register_callback(const callback_type type, void* observer, callb_func_ptr function) {
      // call the pure virtual function performing the registration
      return register_callback(type, boost::shared_ptr< callb_adapt_b>(new callb_adapt_b(observer, function, this)));
    }
    
    /// Function handling the callback
    virtual boost::shared_ptr<callb_adapt_b> register_callback(const callback_type type, boost::shared_ptr<callb_adapt_b> callb) = 0;
    
    
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
