/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2009-2011 GreenSocs
  All rights reserved.
  Authors:
      Christian Schroeder <schroeder@eis.cs.tu-bs.de>
      Mark Burton <mark@greensocs.com>

  Copyright 2016 Ericsson
  All rights reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 *****************************************************************************/



#ifndef CCI_CCI_BROKER_IF_H_INCLUDED_
#define CCI_CCI_BROKER_IF_H_INCLUDED_


#include "cci_cfg/cci_shared_ptr.h"
#include "cci_cfg/cci_callbacks.h"


CCI_OPEN_NAMESPACE_


  // forward declaration
  class cci_param_untyped_handle;
  class cci_param_if;
    
  // forward declaration 
  template <class T>
  class cci_param_typed_handle;

  // forward declaration for friend class
  class cci_broker_manager;
  
  /// CCI configuration broker interface.
  /**
   * This can be used by a tool to access the database or parameter objects, set initial values etc.
   * or can be used by the model itself to get access to configuration objects etc.
   *
   * This always returns not the owner's parameter objects but parameter handle wrappers.
   */
  class cci_broker_if
  {
  protected:
    friend class cci_broker_manager;
   
    /// Creates or returns existing responsible handle for this broker for the given originator
    /**
     * This is called by the broker manager when the static search functions shall return a broker handle.
     * It is possible (but "senseless") to call this as a user. Returned instances shall be deleted together 
     * with the original broker.
     *
     * It is ok to nest calls with the same originator.
     *
     * @param originator   Originator for which the broker handle shall be returned
     * @return Broker handle
     */
    virtual cci_broker_if& create_broker_handle(const cci_originator& originator) = 0;

    /// If this is a handle, returns the originator this broker handle is responsible for, otherwise returns NULL
    /**
     * @return Originator pointer in the case this is a handle; NULL if this is a raw broker
     */
    virtual const cci_originator* get_originator() const = 0;

  public:

    /// Destructor
    virtual ~cci_broker_if() { };

    /// Name of this broker
    /**
     * Shall be a system-wide unique broker name.
     * Broker handles shall return their underlying broker's name.
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
     * @exception        cci::cci_report::set_param_failed Setting parameter object failed
     * @param parname    Full hierarchical parameter name.
     * @param json_value JSON string representation of the init value the parameter has to be set to.
     */
    virtual void json_deserialize_initial_value(const std::string &parname, const std::string &json_value) = 0;

    /// Get a parameter's init value.
    /**
     *
     * @param parname    Full hierarchical parameter name.
     * @return           CCI value of the parameter's initial value. Empty value is returned when parameter is not existing or its initial value is not existing
     */
    virtual const cci::cci_value get_initial_cci_value(const std::string &parname) = 0;
    
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
     * @exception     cci::cci_report::set_param_failed Locking parameter object failed
     * @param parname Hierarchical parameter name.
     */
    virtual void lock_initial_value(const std::string &parname) = 0;

    /// Get a parameter's value (JSON string representation). Independent of the implicit or explicit status.
    /**
     * This accesses the parameter's NVP and works
     * for implicit and explicit parameters.
     *
     * See cci_broker_if::json_serialize_keep_unused to do the same without impacting the used status.
     *
     * @param parname  Full hierarchical name of the parameter whose value should be returned.
     * @return  JSON string of the parameter's value
     */
    virtual const std::string json_serialize(const std::string &parname) = 0;

    /// Get a parameter's value (like cci_broker_if::json_serialize), but not impacting the used status
    /**
     * This is to be used only by tools, e.g. functional coverage tools.
     *
     * @see json_serialize
     * @param parname  Full hierarchical name of the parameter whose value should be returned.
     * @return  JSON string of the parameter's value
     */
    virtual const std::string json_serialize_keep_unused(const std::string &parname) = 0;

    /// Get a parameter handle pointer. (TODO: maybe drop this because of Many-to-one Mapping, this returns only one (which one?))
    /**
     * This returns not the owner's parameter object but a handle.
     *
     * @param   parname   Full hierarchical parameter name.
     * @return  Pointer to the parameter handle object (NULL if not existing).
     *          @todo return a vector/iterator over param objects, because there might be more than one
     */ 
    virtual cci_param_untyped_handle* get_param_handle(const std::string &parname) = 0;
    
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
     * @todo This should allow patterns as argument parname and return a set of callbacks being registered accordingly.
     *
     * Forwards callbacks to @see cci::cci_param_untyped_handle::register_callback
     *
     * \code
     *   // Callback function with default signature.
     *   void config_callback(cci_param_untyped_handle& changed_param, const callback_type& cb_reason) {
     *     // some action
     *   }
     * \endcode
     *
     * @param type        Type of the callback.
     * @param parname     Parameter name
     * @param observer    Pointer to the observing object (the one being called).
     * @param function    Function pointer to the function being called. Signature param_callb_func_ptr. For broker_callb_func_ptr @see cci_broker_if::register_str_callback
     * @return            Shared pointer to the callback adapter (e.g. to be used for unregister calls).
     */
    shared_ptr<callb_adapt> register_callback(const callback_type type, const std::string& parname, void* observer, param_callb_func_ptr function) {
      // call the pure virtual function performing the registration
      return register_callback(parname, type, shared_ptr< callb_adapt>(new callb_adapt(observer, function, NULL)));
    }
    
    /// Registers an observer callback function (with the signature of broker_callb_func_ptr).
    /**
     * @todo This should allow patterns as argument parname and return a set of callbacks being registered accordingly.
     *
     * Similar as @see cci_broker_if::register_callback but parameter string name in cb signature and additional
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
       : mBroker(&cci::cci_broker_manager::get_current_broker(cci::cci_originator("OBSERVER")))
       { 
         // Register callback for new created parameters
         mCallbacks.push_back( mBroker->register_str_callback(cci::create_param, "*", this, 
                                                              cci::bind(&Observer::config_new_param_callback, this, _1, _2)) );
       }
       
       ~Observer();
       
       /// Callback function with default signature announcing new parameters.
       cci::callback_return_type config_new_param_callback(const std::string& par_name, const cci::callback_type& cb_reason) {
         // It is recommended to register with the parameter object directly:
         mCallbacks.push_back(p->register_callback(cci::pre_write, this, 
         cci::bind(&Observer::config_callback, this, _1, _2)));
         // It is possible (but not recommended) to let the broker forward the callback registration to the parameter
         mCallbacks.push_back( mBroker->register_callback(cci::post_write, par_name.c_str(), this, 
         cci::bind(&Observer::config_callback, this, _1, _2)));
         // it is still possible to register a string callback function
         mCallbacks.push_back( mBroker->register_str_callback(cci::post_write, par_name.c_str(), this, 
         cci::bind(&Observer::config_str_callback, this, _1, _2)));
       }
       
       /// Callback function with string signature showing changes for implicit and explicit parameters.
       cci::callback_return_type config_str_callback(const std::string& par_name, const cci::callback_type& cb_reason) {
         [...]
       }
       
       [...]
       
     };

     * \endcode
     *
     * @param type        Type of the callback.
     * @param parname     Parameter name or pattern the callback should be applied to.
     * @param observer    Pointer to the observing object (the one being called).
     * @param function    Function pointer to the function being called. Signature broker_callb_func_ptr. For param_callb_func_ptr @see cci_broker_if::register_callback
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
    //       instead of cci_param_untyped_handle.
    
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
    
    /// Adds a parameter to the registry.
    /** 
     * Note: addPar (and all related methods) must not call any of the 
     *       pure virtual functions in cci_param_untyped_handle because this method
     *       may be called by the cci_param_untyped_handle constructor.
     *
     * Note: This function shall never been called for any parameter handle
     *       objects but only for "real" parameter objects.
     *
     * @exception cci::cci_report::add_param_failed Adding parameter object failed
     * @param par Parameter (including name and value).
     */
    virtual void add_param(cci_param_if* par) = 0;
    
    /// Removes a parameter from the registry. May only be called by the parameter destructor, must not be called by anone else.
    /**
     * It should be ensured this is not being called from elsewhere than the parameter destructor (e.g. by user).
     *
     * Note: This function shall never been called for any parameter handle
     *       objects but only for "real" parameter objects.
     *
     * @exception cci::cci_report::remove_param_failed Remove parameter object failed
     * @param par Parameter pointer.
     */
    virtual void remove_param(cci_param_if* par) = 0;
    

  public:
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////    Optional functions   //////////////////////////// //
    // TODO: Optional Config broker functions to be discussed

    /// Return a pointer list of all (explicit) parameter handles in the given scope (matching the pattern)
    /**
     * pattern @see get_param_list
     * + (in the case the Many-to-one Mapping should work):
     *   pattern = full_param_name to get all param objects/handles (PH/PO) being mapped to the NVP
     * @todo use iterator instead of vector?
     *
     * This returns not the owner's parameter objects but handles.
     *
     * @param pattern Specifies the parameters to be returned.
     * @return Vector with parameter base object pointers.
     */
    virtual const std::vector<cci_param_untyped_handle*> get_param_handles(const std::string& pattern = "") = 0;
    
    
    // Dropped due to conf call discussion in February
    //! @cond Doxygen_Suppress
    /// Set an alias to a parameter name
    /**
     * TODO: Guideline for other CCI functions, e.g.:
     * - get_param_list will NOT return any alias names
     * - all parameter access functions will take an alias
     *
     * @param orig_parname    Full hierarchical name of the original parameter (or another alias).
     * @param alias_parname   Full hierarchical (full user chosen) alias name.
     */
    //! @endcond
    //virtual void set_alias(std::string& orig_parname, std::string& alias_parname) = 0;
        
    /// Convenience function to get a typed parameter handle pointer.
    /**
     * @param   parname   Full hierarchical parameter name.
     * @return  Pointer to the parameter object (NULL if not existing).
     */ 
    template<class T>
    cci_param_typed_handle<T>* get_cci_param_handle(const std::string &parname) {
      return dynamic_cast<cci_param_typed_handle<T>*>(get_param_handle(parname));
    }

    ///If this broker is a private broker (or handle)
    /**
     * @return If this broker is a private broker
     */
    virtual bool is_private_broker() const = 0;
    
  };

 /// Creates or returns the one non-private global config broker provided by the broker implementation
 /// Called by the header function get_current_broker, NEVER call this as a user!
 /**
  * This returns the raw broker, not a handle, thus this
  * shall not be returned directy to the user!
  *
  * The implemementation shall register the global broker with the broker registry
  * cci_broker_registry::registry() !
  *
  * @return The one non-private global config broker (not wrapped with a handle)
  */
 cci_broker_if& create_global_cnf_broker();

CCI_CLOSE_NAMESPACE_

#endif
