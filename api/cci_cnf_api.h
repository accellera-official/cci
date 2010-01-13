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



#ifndef __CCI_CNF_API_H__
#define __CCI_CNF_API_H__


#include <boost/shared_ptr.hpp>


namespace cci {


  // forward declaration 
  class cci_base_param;
    
  // forward declaration 
  template <class T, param_mutable_type TM> 
  class cci_param;

  
  /// CCI configuration API interface.
  /**
   * This can be used by a tool to access the database or parameter objects, set initial values etc.
   * or can be used by the model itself to get access to configuration objects etc.
   */
  class cci_cnf_api
  {
    
  public:
    
    // Destructor
    virtual ~cci_cnf_api() { };

    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////   Access Parameters and Values   /////////////////////// //

    
    /// Set a parameter's init value. 
    /**
     * The init value has priority to the default value being set by the owner!
     *
     * @exception        cci::cci_report_types::set_param_failed Setting parameter object failed
     * @param parname    Full hierarchical parameter name.
     * @param json_value JSON string representation of the init value the parameter has to be set to.
     */
    virtual void set_init_value(const std::string &parname, const std::string &json_value) = 0;
    
    /// Get a parameter's value (JSON string representation). Independent of the implicit or explicit status.
    /**
     * This accesses the parameter's NVP and works
     * for implicit and explicit parameters.
     *
     * @param parname  Full hierarchical name of the parameter whose value should be returned.
     * @return  JSON string of the parameter's value
     */
    virtual const std::string get_json_string(const std::string &parname) = 0;

    /// Get a parameter pointer. (TODO: maybe drop this because of Many-to-one Mapping, this returns only one (which one?))
    /**
     * @param   parname   Full hierarchical parameter name.
     * @return  Pointer to the parameter object (NULL if not existing). @todo return a vector/iterator over param objects, because there might be more than one
     */ 
    virtual cci_base_param* get_param(const std::string &parname) = 0;
    
    /// Return a list of all parameters (implicit and explicit) for the given scope (matching the pattern)
    /**
     * TODO:
     * e.g.
     * - parameters of the specified module, pattern = module_name, e.g. "mymod.mysubmod";
     * - pattern = module_name+'.*' to include the parameters of the childs, e.g. "mymod.mysubmod.*"
     * - pattern = '*'.param_name to get all parameters with the name, e.g. "*.cache_size"
     * - pattern = '*'.hierarchical.param_name to get all parameters with the name, e.g. "*.mysubmod.cache_size"
     *
     * @param pattern Specifies the parameters to be returned.
     * @return Vector with full hierarchical parameter names.
     */
    virtual const std::vector<std::string> get_param_list(const std::string& pattern) = 0;
    
    /// Checks whether a parameter exists (implicit or explicit).
    /**
     * @param parname  Full hierarchical parameter name.
     * @return Whether the parameter < parname > exists in the registry.
     */
    virtual bool exists_param(const std::string &parname) = 0;
    
    /// Returns if the parameter has ever been used.
    /**
     * A parameter has been used if there is either a parameter object
     * or handle mapped to the NVP, or the NVP's value has ever been read.
     *
     * @param parname  Full hierarchical parameter name.
     * @return If the parameter is used.
     */
    virtual bool is_used(const std::string &parname) = 0;

    
    // //////////////////////////////////////////////////////////////////// //
    // ////////////////   Get Parameter List   //////////////////////////// //

    
    /// Returns a list of all parameters existing (implicit and explicit) in the registry.
    /**
     * @return Vector with full hierarchical parameter names.
     */
    virtual const std::vector<std::string> get_param_list() = 0;

    
    // //////////////////////////////////////////////////////////////////// //
    // /////////////////   Callback Handling   //////////////////////////// //
    
    
    /// Registers an observer callback function (with the signature of callback_func_ptr).
    /**
     * Same as @see cci::cci_base_param::register_callback but additional
     * - create param callbacks
     * - callbacks to not yet existing params
     * - (optionally) callbacks for regex names
     * 
     * This will forward callback registration to the parameter object.
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
     *     // will unregister the callbacks being registered within this module for all parameters
     *     m_api.unregister_all_callbacks(this);
     *   }
     *
     *   // Example code to register callback function
     *   void main_action() {
     *     // some code, parameters etc...
     *
     *     my_callbacks.push_back( my_param.register_callback(cci::post_write,  this, boost::bind(&MyIP_Class::config_callback, this, _1, _2)) );
     *     //   equal to
     *     my_callbacks.push_back( m_api.register_callback(cci::post_write   , my_param.get_name(), this, boost::bind(&MyIP_Class::config_callback, this, _1, _2)) );
     *
     *     my_callbacks.push_back( my_param.register_callback(cci::destroy_param, this, boost::bind(&MyIP_Class::config_callback, this, _1, _2)) );
     *     //   equal to
     *     my_callbacks.push_back( m_api.register_callback(cci::destroy_param, my_param.get_name(), this, boost::bind(&MyIP_Class::config_callback, this, _1, _2)) );
     *
     *     my_callbacks.push_back( m_api.register_callback(cci::create_param , "*"                , this, boost::bind(&MyIP_Class::config_callback, this, _1, _2)) );
     *  OPTIONAL:
     *     my_callbacks.push_back( m_api.register_callback(cci::post_write, "*.my_param"  , this, boost::bind(&MyIP_Class::config_callback, this, _1, _2)) );
     *     my_callbacks.push_back( m_api.register_callback(cci::post_write, "MyIP_Class.*", this, boost::bind(&MyIP_Class::config_callback, this, _1, _2)) );
     *   }
     *
     *   // Callback function with default signature.
     *   void config_callback(cci_base_param& changed_param, const callback_type& cb_reason) {
     *     // some action
     *   }
     *
     *   std::vector< boost::shared_ptr<callb_adapt_b> > my_callbacks;
     * };
     * \endcode
     *
     * @param type        Type of the callback.
     * @param parname     Parameter name or pattern the callback should be applied to.
     * @param observer    Pointer to the observing object (the one being called).
     * @param function    Boost function pointer to the function being called.
     * @return            Boost shared pointer to the callback adapter (e.g. to be used for unregister calls).
     */
    boost::shared_ptr<callb_adapt_b> register_callback(const callback_type type, const std::string& parname, void* observer, callb_func_ptr function) {
      // call the pure virtual function performing the registration
      return register_callback(parname, type, boost::shared_ptr< callb_adapt_b>(new callb_adapt_b(observer, function, NULL)));
    }
    
    /// Function handling the callback (should not be called by user)
    virtual boost::shared_ptr< callb_adapt_b> register_callback(const std::string& parname, const callback_type type, boost::shared_ptr< callb_adapt_b> callb) = 0;

    
    /// Unregisters all callbacks (within all existing parameters) for the specified observer object (e.g. sc_module). 
    /**
     * @param observer   Pointer to the observer module who did register parameter callbacks. NULL shall be an error.
     */
    virtual void unregister_all_callbacks(void* observer) = 0;
    
    // TODO: If we want to provide create_param callbacks for initial values 
    //       being set for not yet existing parameters (implicit parameters),
    //       we will need another register_callback()-function with a different 
    //       callback function signature getting <stringName,stringValue>-pair
    //       instead of cci_base_param.
    
    /// Unregisters the callback and (default) deletes the callback adapter.
    /**
     * Unregisters (only) the given callback.
     *
     * The callback adapter is NOT deleted because this should be done by the
     * surrounding shared pointer!
     *
     * This may be used by a user module which stored the shared pointer to a
     * specific callback or by the destructor of the param callback adapter.
     *
     * @param callb  Parameter callback adapter
     * @return       If the callback adapter existed in this parameter.
     */
    virtual bool unregister_param_callback(callb_adapt_b* callb) = 0;
    
    /// Returns if the parameter has registered callbacks
    virtual bool has_callbacks(const std::string& parname) = 0;
    
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Registry Functions   ///////////////////////////// //

    
  //protected:
  //  friend class cci_base_param;
    
    /// Adds a parameter to the registry.
    /** 
     * Note: addPar (and all related methods) must not call any of the 
     *       pure virtual functions in cci_base_param because this method 
     *       may be called by the cci_base_param constructor.
     *
     * @exception cci::cci_report_types::add_param_failed Adding parameter object failed
     * @param par Parameter (including name and value).
     */
    virtual void add_param(cci_base_param* par) = 0;
    
    /// Removes a parameter from the registry. May only be called by the parameter destructor, must not be called by anone else.
    /**
     * It should be ensured this is not being called from elsewhere than the parameter destructor (e.g. by user).
     *
     * @exception cci::cci_report_types::remove_param_failed Remove parameter object failed
     * @param par Parameter pointer.
     */
    virtual void remove_param(cci_base_param* par) = 0;
    

  public:
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////    Optional functions   //////////////////////////// //
    // TODO: Optional Config API functions to be discussed

    /// Return a pointer list of all (explicit) parameters in the given scope (matching the pattern)
    /**
     * pattern @see get_param_list
     * + (in the case the Many-to-one Mapping should work):
     *   pattern = full_param_name to get all param objects/handles (PH/PO) being mapped to the NVP
     * @todo use iterator instead of vector?
     *
     * @param pattern Specifies the parameters to be returned.
     * @return Vector with parameter base object pointers.
     */
    virtual const std::vector<cci_base_param*> get_params(const std::string& pattern = "") = 0;
    
    
    /// Set an alias to a parameter name
    /**
     * TODO: Guideline for other CCI functions, e.g.:
     * - get_param_list will NOT return any alias names
     * - all parameter access functions will take an alias
     *
     * @param orig_parname    Full hierarchical name of the original parameter (or another alias).
     * @param alias_parname   Full hierarchical (full user chosen) alias name.
     */
    virtual void set_alias(std::string& orig_parname, std::string& alias_parname) = 0;
        
    /// Convenience function to get a typed parameter pointer.
    /**
     * @param   parname   Full hierarchical parameter name.
     * @return  Pointer to the parameter object (NULL if not existing).
     */ 
    template<class T, param_mutable_type TM>
    cci_param<T, TM>* get_cci_param(const std::string &parname) {
      return dynamic_cast<cci_param<T, TM>*>(get_param(parname));
    }
    
  };

      
} // end namespace cci

#endif
