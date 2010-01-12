// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
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
    /**
     *
     */
    virtual ~cci_base_param() { }
    
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////   Set and Get with JSON String Representation   //////////// //
    
    
    /// Sets the value of this parameter given by a JSON string. @todo Alternative name: function set_json_string
    /** 
     * @exception cci_exception_set_param Setting value failed
     * @param json_string the new value, represented as a JSON string.
     */
    virtual void json_deserialize(const std::string& json_string) = 0;
    
    /// Get the JSON string representation of this parameter's value. @todo Alternative function name: get_json_string
    /**
     * @exception cci_exception_get_param Getting value failed
     * @return  The value of this parameter represented as a JSON string.
     */
    virtual const std::string& json_serialize() const = 0;
    
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   JSON Data Type and access   ////////////////////// //

    
    /// Returns a basic type this parameter can be converted to or from (which is not necessarily the actual parameter type)
    /**
     * @return Type
     */
    virtual const basic_param_type get_basic_type() const { return partype_not_available; }
    
#define CCI_NOT_SUPPORTED_WRN SC_REPORT_WARNING(CCI_SC_REPORT_MSG_TYPE_PREFIX, "Not supported for this parameter type!")

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
     * @exception cci_exception_set_param Setting value failed
     * @param value  Value which the parameter should be set to.
     * @return       If the setting was successfull.
     */
    //virtual void set_number(const sc_dt::uint64 value) { CCI_NOT_SUPPORTED_WRN; }
    /// Set the parameter value by trying to convert the given number to the param value.
    /** Details @see cci_base_param::set_number(const sc_dt::uint64 value) If the parameter does not implement this, it will call the set_number function. */
    //virtual void set_double(const double value)        { sc_dt::uint64 llval = (sc_dt::uint64)value; set_number(llval); }
    /// Set the parameter value by trying to convert the given string to the param value.
    /** Details @see cci_base_param::set_number(const sc_dt::uint64 value) */
    //virtual void set_string(const std::string& value)   { CCI_NOT_SUPPORTED_WRN; }
    /// Set the parameter value by trying to convert the given bool to the param value.
    /** Details @see cci_base_param::set_number(const sc_dt::uint64 value) */
    //virtual void set_bool(const bool value)            { CCI_NOT_SUPPORTED_WRN; }

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
     * @exception cci_exception_get_param Getting value failed
     * @param retvalue  Value which the parameter should be copied to.
     */
    //virtual void get_number(sc_dt::uint64& retvalue) { CCI_NOT_SUPPORTED_WRN; }
    /// Get the parameter value by trying to convert it to the given number.
    /** Details @see cci_base_param::get_number(sc_dt::uint64& retvalue). If the parameter does not implement this, it will call the get_number function. */
    //virtual void get_double(double& retvalue)        { sc_dt::uint64 llval; get_number(llval); retvalue=(double)llval; }
    /// Get the parameter value by trying to convert it to the given string.
    /** Details @see cci_base_param::get_number(sc_dt::uint64& retvalue) */
    //virtual void get_string(std::string& retvalue)   { CCI_NOT_SUPPORTED_WRN; }
    /// Get the parameter value by trying to convert it to the given bool.
    /** Details @see cci_base_param::get_number(sc_dt::uint64& retvalue) */
    //virtual void get_bool(bool& retvalue)            { CCI_NOT_SUPPORTED_WRN; }
    
    //
    // Alternatively??
    //
    
    /// Set the parameter's value to the given one
    /**
     * @exception cci_exception_set_param Setting value failed
     * @param val This value is either (in the case of a pure basic param) converted into a JSON string and stored in the base param or (in the case of a typed parameter) into the actual data type
     */
    virtual void set_value(const cci_value& val) = 0;
    
    /// Get the parameter's value
    /**
     * @exception cci_exception_get_param Getting value failed
     * @return This value is either (in the case of a pure basic param) converted from the JSON string or (in the case of a typed parameter) from the actual data type
     */
    virtual cci_value get_value() = 0;
    
    
    // //////////////////////////////////////////////////////////////////// //
    // /////////////////////   Documentation   //////////////////////////// //

    
    /// Set parameter meta data/documentation
    /**
     * Adds parameter meta data which should describe the 
     * intended use, allowed value range etc. in a human readable way.
     *
     * @param doc Human readable documentation
     */
    virtual void set_documentation(const std::string& doc) = 0;

    /// Get the parameter's meta data/documentation
    /**
     * return Documentation
     */
    virtual std::string get_documentation() const = 0;
    
    
    // //////////////////////////////////////////////////////////////////// //
    // ////////////////   Parameter Value Status   //////////////////////// //

    
    /// Returns if the current value is the default one being set by the constructor and never been modified
    /**
     * True if the value had never been set but only by the constructor.
     *
     * Note that this returns false even if the current value is 
     * actually the same as the default one. This returns also false
     * if there had not been set a constructor default value.
     *
     * @return If the parameter's current value is the default one
     */
    virtual bool is_default_value() = 0;
    
    /// Returns if the current value is invalid
    /**
     * Returns true if 
     *  - the parameter has no constructor default value and never been set or
     *  - OPTIONAL (currently implemented): the value has manually been
     *    set to be invalid (using function set_invalid_value() ) @see set_invalid_value()
     *
     * @return  If the parameter's current value is invalid
     */
    virtual bool is_invalid_value() = 0;

    /// Marks the value to be invalid. (Does not impact the actual value.)
    virtual void set_invalid_value() = 0;
    
    /// OPTIONAL: Returns if the current value is an initial value being set by the database (OPTIONAL:) and not been modified
    /**
     * True if the value has been set using the cnf_api's
     * set_initial_value function (OPTIONAL:) and not been modified since then.
     *
     * @return If the parameter's current value is an initial value being set by the database
     */
    virtual bool is_initial_value() = 0;
    
    
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
     *   cci::callback_reason config_callback(cci_base_param& changed_param, const callback_type& cb_reason) {
     *     // some action
     *     return cci::return_nothing;
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
    
    /// Function registering a callback object (should not be called by user)
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
    virtual bool unregister_param_callback(boost::shared_ptr<cci::callb_adapt_b> callb) = 0;
    /// @see unregister_param_callback(boost::shared_ptr<cci::callb_adapt_b>) 
    virtual bool unregister_param_callback(cci::callb_adapt_b* callb) = 0;
    
    /// Returns if the parameter has registered callbacks
    virtual bool has_callbacks() = 0;
    

    // //////////////////////////////////////////////////////////////////// //
    // ////////////////////   Parameter Lock   //////////////////////////// //
    
    // OPTIONAL! Could use a pre write callback instead which is able to reject a param change
    
    /// Locking this parameter, optionally with a password
    /**
     * Makes a parameter read-only.
     *
     * Returns false
     * - if this parameter was already locked with a different password than pwd. Then it is still locked but not with this given/without password.
     *
     * Returns true 
     * - if the parameter was not locked (and is locked now) or 
     * - if the parameter was locked without a password. Then it is locked now with the given password
     * - if the parameter was locked with the given password pwd. Then it is still locked now with the given password.
     *
     * @param pwd Password needed to unlock the param, ideally any pointer address known only by the locking entity, default = NULL.
     * @return If the lock was successfull. .
     */
    virtual bool lock(const void* pwd = NULL) = 0;

    /// Unlocking this parameter, optionally with a password if needed
    /**
     * @param pwd Password to unlock the param (if needed), default = NULL.
     * @return If the parameter is unlocked now.
     */
    virtual bool unlock(const void* pwd = NULL) = 0;

    /// If this parameter is locked
    /**
     * @return If this parameter is locked
     */
    virtual bool locked() = 0;
    
  };

} // namespace cci

#endif
