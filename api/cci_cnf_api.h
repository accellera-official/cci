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


#ifndef __CCI_CNF_API_H__
#define __CCI_CNF_API_H__


#include <boost/shared_ptr.hpp>


namespace cci {


  // forward declaration 
  class cci_param_base;
    
  // forward declaration 
  template <class T> 
  class cci_param;

  
  /// CCI configuration API interface.
  class cci_cnf_api
  {
    
  public:
    
    // Destructor
    virtual ~cci_cnf_api() { };

    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////   Access Parameters and Values   /////////////////////// //

    
    /// Set a parameter's init value. 
    /**
     * The init value has priority to the initial value set by the owner!
     *
     * @param parname Full hierarchical parameter name.
     * @param value   JSON string representation of the init value the parameter has to be set to.
     * @return        Success of the setting.
     */
    virtual set_param_error_type set_init_value(const std::string &parname, const std::string &json_value) = 0;
    
    /// Get a parameter's value (JSON string representation). Independent of the implicit or explicit status.
    /**
     * @param parname  Full hierarchical name of the parameter whose value should be returned.
     * @return  JSON string of the parameter's value
     */
    virtual const std::string get_json_string(const std::string &parname) = 0;

    /// Get a parameter pointer.
    /**
     * @param   parname   Full hierarchical parameter name.
     * @return  Pointer to the parameter object (NULL if not existing).
     */ 
    virtual cci_param_base* get_param(const std::string &parname) = 0;
    
    /// Checks whether a parameter exists (implicit or explicit).
    /**
     * @param parname  Full hierarchical parameter name.
     * @return Whether the parameter <parname> exists in the registry.
     */
    virtual bool exists_param(const std::string &parname) = 0;

    
    // //////////////////////////////////////////////////////////////////// //
    // ////////////////   Get Parameter List   //////////////////////////// //

    
    /// Return a list of all parameters existing (TODO implicit and explicit?) in the registry.
    /**
     * @return Vector with full hierarchical parameter names.
     */
    virtual const std::vector<std::string> get_param_list() = 0;
    // also see optional functions!

    
    // //////////////////////////////////////////////////////////////////// //
    // /////////////////   Callback Handling   //////////////////////////// //
    
    
    /// Registers an observer callback function (with the signature of callback_func_ptr).
    /**
     * Same as @see cci::cci_param_base::register_callback but additional
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
     *   void config_callback(cci_param_base& changed_param, const callback_type& cb_reason) {
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
    
    /// Function handling the callback
    virtual boost::shared_ptr< callb_adapt_b> register_callback(const std::string& parname, const callback_type type, boost::shared_ptr< callb_adapt_b> callb) = 0;

    
    /// Unregisters all callbacks (within all existing parameters) for the specified observer object (e.g. sc_module). 
    /**
     * @param observer   Pointer to the observer module who did register parameter callbacks.
     */
    virtual void unregister_all_callbacks(void* observer) = 0;
    
    // TODO: If we want to provide create_param callbacks for initial values 
    //       being set for not yet existing parameters (implicit parameters),
    //       we will need another register_callback()-function with a different 
    //       callback function signature getting <stringName,stringValue>-pair
    //       instead of cci_param_base.
    
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
  //  friend class cci_param_base;
    
    /// Add a parameter to the registry.
    /** 
     * Note: addPar (and all related methods) must not call any of the 
     *       pure virtual functions in cci_param_base because this method is 
     *       called by the cci_param_base constructor.
     *
     * @param par Parameter (including name and value).
     * @return Success of the adding.
     */
    virtual add_param_error_type add_param(cci_param_base* par) = 0;
    
    /// Remove a parameter from the registry. May only be called by the parameter destructor.
    /**
     * Should ensure not being called from elsewhere (e.g. by user).
     *
     * @param par Parameter pointer.
     * @return Success of remove.
     */
    virtual remove_param_error_type remove_param(cci_param_base* par) = 0;
    

  public:
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Optional functions   ///////////////////////////// //

    
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
    template<class T>
    cci_param<T>* get_cci_param(const std::string &parname) {
      return dynamic_cast<cci_param<T>*>(get_param(parname));
    }
    
    /// Return a list of all parameters (TODO implicit and explicit?) matching the pattern
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
    
    /// Return a pointer list of all (explicit) parameters matching the pattern
    /**
     * pattern @see get_param_list
     *
     * @param pattern Specifies the parameters to be returned.
     * @return Vector with parameter base object pointers.
     */
    virtual const std::vector<cci_param_base*> get_params(const std::string& pattern = "") = 0;

  };

      
} // end namespace cci

#endif
