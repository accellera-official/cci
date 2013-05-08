// LICENSETEXT
//
//   Copyright (C) 2009-2011 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//    Mark Burton, mark@greensocs.com
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT



#ifndef __CCI_CNF_BROKER_IF_H__
#define __CCI_CNF_BROKER_IF_H__


#include "core/cci_shared_ptr.h"    


__CCI_OPEN_CONFIG_NAMESPACE__


  // forward declaration 
  class cci_base_param;
    
  // forward declaration 
  template <class T, param_mutable_type TM> 
  class cci_param_accessor;

  // forward declaration for friend class
  class cci_broker_manager;
  
  /// CCI configuration broker interface.
  /**
   * This can be used by a tool to access the database or parameter objects, set initial values etc.
   * or can be used by the model itself to get access to configuration objects etc.
   *
   * This always returns not the owner's parameter objects but parameter accessor wrappers.
   */
  class cci_cnf_broker_if
  {
  protected:
    friend class cci_broker_manager;
   
    /// Creates or returns existing responsible accessor for this broker for the given originator
    /**
     * This is called by the broker manager when the static search functions shall return a broker accessor.
     * It is possible (but "senseless") to call this as a user. Returned instances shall be deleted together 
     * with the original broker.
     *
     * It is ok to nest calls with the same originator.
     *
     * @param originator   Originator for which the broker accessor shall be returned
     * @return Broker accessor
     */
    virtual cci_cnf_broker_if& get_accessor(const cci_originator& originator) = 0;

    /// If this is an accessor, returns the originator this broker accessor is responsible for, otherwise returns NULL
    /**
     * @return Originator pointer in the case this is an accessor; NULL if this is a raw broker
     */
    virtual const cci_originator* get_originator() const = 0;

  public:

    /// Destructor
    virtual ~cci_cnf_broker_if() { };

    /// Name of this broker
    /**
     * Shall be a system-wide unique broker name.
     * Broker accessors shall return their underlying broker's name.
     * Accessors can be distinguished using their originator information.
     *
     * @return broker name
     */
    virtual const std::string &name() const = 0;
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////   Access Parameters and Values   /////////////////////// //

    
    /// Set a parameter's init value. 
    /**
     * The init value has priority to the default value being set by the owner!
     * If called for an explicit parameter this is a usual set, including not
     * setting the is_initial_value state to true!
     *
     * @exception        cci::cnf::cci_report::set_param_failed Setting parameter object failed
     * @param parname    Full hierarchical parameter name.
     * @param json_value JSON string representation of the init value the parameter has to be set to.
     */
    virtual void json_deserialize_initial_value(const std::string &parname, const std::string &json_value) = 0;
    
    /// Returns the originator of the latest write access for the given parameter, independently if it is an implicit or explicit parameter, otherwise returns NULL
    /**
     * For explicit parameters it is recommended to use the parameter object's get_latest_write_originator() function.
     * Returns NULL if there is no parameter with the given name
     * or there is no originator.
     * If there is an explicit parameter and its originator is not NULL 
     * this returns the explicit parameter's originator information!
     *
     * @param parname  Name of an implicit or explicit parameter.
     * @return Originator pointer; NULL if there was not yet any write
     */
    virtual const cci_originator* get_latest_write_originator(const std::string &parname) const = 0;

    /// Lock a parameter's init value. 
    /**
     * Lock so that this parameter's init value cannot be overwritten by
     * any subsequent setInitValue call. This allows to emulate a hierarchical
     * precendence since a top-level module can prevent the childs from setting
     * init values by locking the init value before creating the subsystem.
     *
     * Throws (and does not lock) if no initial value is existing
     * that can be locked or if an initial value is already locked or if the
     * parameter is already existing as object (explicit parameter).
     *
     * @exception     cci::cnf::cci_report::set_param_failed Locking parameter object failed
     * @param parname Hierarchical parameter name.
     */
    virtual void lock_initial_value(const std::string &parname) = 0;

    /// Get a parameter's value (JSON string representation). Independent of the implicit or explicit status.
    /**
     * This accesses the parameter's NVP and works
     * for implicit and explicit parameters.
     *
     * See cci_cnf_broker_if::json_serialize_keep_unused to do the same without impacting the used status.
     *
     * @param parname  Full hierarchical name of the parameter whose value should be returned.
     * @return  JSON string of the parameter's value
     */
    virtual const std::string json_serialize(const std::string &parname) = 0;

    /// Get a parameter's value (like cci_cnf_broker_if::json_serialize), but not impacting the used status
    /**
     * This is to be used only by tools, e.g. functional coverage tools.
     *
     * @see json_serialize
     * @param parname  Full hierarchical name of the parameter whose value should be returned.
     * @return  JSON string of the parameter's value
     */
    virtual const std::string json_serialize_keep_unused(const std::string &parname) = 0;

    /// Get a parameter accessor pointer. (TODO: maybe drop this because of Many-to-one Mapping, this returns only one (which one?))
    /**
     * This returns not the owner's parameter object but an accessor.
     *
     * @param   parname   Full hierarchical parameter name.
     * @return  Pointer to the parameter accessor object (NULL if not existing). 
     *          @todo return a vector/iterator over param objects, because there might be more than one
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
    virtual bool param_exists(const std::string &parname) = 0;
    
    /// Returns if the parameter has ever been used.
    /**
     * A parameter has been used if there is/was either a parameter object
     * or handle mapped to the NVP, or the NVP's value has ever been read 
     * (from the broker).
     *
     * @param parname  Full hierarchical parameter name.
     * @return If the parameter is or has been used.
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
    
    
    /// Registers an observer callback function (with the signature of param_callb_func_ptr).
    /**
     * @TODO This should allow patterns as argument parname and return a set of callbacks being registered accordingly.
     *
     * Forwards callbacks to @see cci::cnf::cci_base_param::register_callback
     *
     * \code
     *   // Callback function with default signature.
     *   void config_callback(cci_base_param& changed_param, const callback_type& cb_reason) {
     *     // some action
     *   }
     * \endcode
     *
     * @param type        Type of the callback.
     * @param parname     Parameter name
     * @param observer    Pointer to the observing object (the one being called).
     * @param function    Function pointer to the function being called. Signature param_callb_func_ptr. For broker_callb_func_ptr @see cci_cnf_broker_if::register_str_callback
     * @return            Shared pointer to the callback adapter (e.g. to be used for unregister calls).
     */
    shared_ptr<callb_adapt> register_callback(const callback_type type, const std::string& parname, void* observer, param_callb_func_ptr function) {
      // call the pure virtual function performing the registration
      return register_callback(parname, type, shared_ptr< callb_adapt>(new callb_adapt(observer, function, NULL)));
    }
    
    /// Registers an observer callback function (with the signature of broker_callb_func_ptr).
    /**
     * @TODO This should allow patterns as argument parname and return a set of callbacks being registered accordingly.
     *
     * Similar as @see cci_cnf_broker_if::register_callback but parameter string name in cb signature and additional
     * - create param callbacks allowed
     * - post_write-callbacks for implicit params possible (caused on json and init value access as 
     *   long as they are implicit, when the param becomes explicit the callbacks are called 
     *   (forwarded) from the param object)
     * - (optionally) callbacks for regex names
     * 
     * This will forward callback registration to the explicit parameter object or handle self.
     *
     * Usage example:
     * \code

     class Observer
     {
     public:
       
       Observer(const char* name)
       : mBroker(&cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator("OBSERVER")))
       { 
         // Register callback for new created parameters
         mCallbacks.push_back( mBroker->register_str_callback(cci::cnf::create_param, "*", this, 
                                                              cci::bind(&Observer::config_new_param_callback, this, _1, _2)) );
       }
       
       ~Observer();
       
       /// Callback function with default signature announcing new parameters.
       cci::cnf::callback_return_type config_new_param_callback(const std::string& par_name, const cci::cnf::callback_type& cb_reason) {
         // It is recommended to register with the parameter object directly:
         mCallbacks.push_back(p->register_callback(cci::cnf::pre_write, this, 
         cci::bind(&Observer::config_callback, this, _1, _2)));
         // It is possible (but not recommended) to let the broker forward the callback registration to the parameter
         mCallbacks.push_back( mBroker->register_callback(cci::cnf::post_write, par_name.c_str(), this, 
         cci::bind(&Observer::config_callback, this, _1, _2)));
         // it is still possible to register a string callback function
         mCallbacks.push_back( mBroker->register_str_callback(cci::cnf::post_write, par_name.c_str(), this, 
         cci::bind(&Observer::config_str_callback, this, _1, _2)));
       }
       
       /// Callback function with string signature showing changes for implicit and explicit parameters.
       cci::cnf::callback_return_type config_str_callback(const std::string& par_name, const cci::cnf::callback_type& cb_reason) {
         [...]
       }
       
       [...]
       
     };

     * \endcode
     *
     * @param type        Type of the callback.
     * @param parname     Parameter name or pattern the callback should be applied to.
     * @param observer    Pointer to the observing object (the one being called).
     * @param function    Function pointer to the function being called. Signature broker_callb_func_ptr. For param_callb_func_ptr @see cci_cnf_broker_if::register_callback
     * @return            Shared pointer to the callback adapter (e.g. to be used for unregister calls).
     */
    shared_ptr<callb_adapt> register_str_callback(const callback_type type, const std::string& parname, void* observer, broker_callb_func_ptr function) {
      // call the pure virtual function performing the registration
      return register_callback(parname, type, shared_ptr< callb_adapt>(new callb_adapt(observer, function, NULL)));
    }
    
    /// Function handling the callback (should not be called by user)
    virtual shared_ptr< callb_adapt> register_callback(const std::string& parname, const callback_type type, shared_ptr< callb_adapt> callb) = 0;

    
    /// Unregisters all callbacks (within all existing parameters) for the specified observer object (e.g. sc_module). 
    /**
     * @param observer   Pointer to the observer module who did register
     *                   parameter callbacks. NULL shall be an error.
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
    virtual bool unregister_callback(callb_adapt* callb) = 0;
    
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
     * Note: This function shall never been called for any parameter accessor
     *       objects but only for "real" parameter objects.
     *
     * @exception cci::cnf::cci_report::add_param_failed Adding parameter object failed
     * @param par Parameter (including name and value).
     */
    virtual void add_param(cci_base_param* par) = 0;
    
    /// Removes a parameter from the registry. May only be called by the parameter destructor, must not be called by anone else.
    /**
     * It should be ensured this is not being called from elsewhere than the parameter destructor (e.g. by user).
     *
     * Note: This function shall never been called for any parameter accessor
     *       objects but only for "real" parameter objects.
     *
     * @exception cci::cnf::cci_report::remove_param_failed Remove parameter object failed
     * @param par Parameter pointer.
     */
    virtual void remove_param(cci_base_param* par) = 0;
    

  public:
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////    Optional functions   //////////////////////////// //
    // TODO: Optional Config broker functions to be discussed

    /// Return a pointer list of all (explicit) parameters in the given scope (matching the pattern)
    /**
     * pattern @see get_param_list
     * + (in the case the Many-to-one Mapping should work):
     *   pattern = full_param_name to get all param objects/handles (PH/PO) being mapped to the NVP
     * @todo use iterator instead of vector?
     *
     * This returns not the owner's parameter objects but accessors.
     *
     * @param pattern Specifies the parameters to be returned.
     * @return Vector with parameter base object pointers.
     */
    virtual const std::vector<cci_base_param*> get_params(const std::string& pattern = "") = 0;
    
    
    // Dropped due to conf call discussion in February
    /// Set an alias to a parameter name
    /**
     * TODO: Guideline for other CCI functions, e.g.:
     * - get_param_list will NOT return any alias names
     * - all parameter access functions will take an alias
     *
     * @param orig_parname    Full hierarchical name of the original parameter (or another alias).
     * @param alias_parname   Full hierarchical (full user chosen) alias name.
     */
    //virtual void set_alias(std::string& orig_parname, std::string& alias_parname) = 0;
        
    /// Convenience function to get a typed parameter accessor pointer.
    /**
     * @param   parname   Full hierarchical parameter name.
     * @return  Pointer to the parameter object (NULL if not existing).
     */ 
    template<class T, param_mutable_type TM>
    cci_param_accessor<T, TM>* get_cci_param(const std::string &parname) {
      return dynamic_cast<cci_param_accessor<T, TM>*>(get_param(parname));
    }

    ///If this broker is a private broker (or accessor)
    /**
     * @return If this broker is a private broker
     */
    virtual bool is_private_broker() const = 0;
    
  };

      
__CCI_CLOSE_CONFIG_NAMESPACE__

#endif
