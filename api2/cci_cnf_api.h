//   OSCI CCI WG
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// 
// ENDLICENSETEXT


#ifndef __CCI_CNF_API_H__
#define __CCI_CNF_API_H__


namespace cci {


  // forward declaration 
  class cci_param_base;
    
  template <class T> 
  class cci_param;

  
  /// CCI configuration API interface.
  class cci_cnf_api
  {
    
  public:
    
    // Destructor
    virtual ~cci_cnf_api() { }

    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////   Access Parameters and Values   /////////////////////// //

    
    /// Set a parameter's init value. 
    /**
     * The init value has priority to the initial value set by the owner!
     *
     * @param parname Full hierarchical parameter name.
     * @param value   String representation of the init value the parameter has to be set to.
     * @return        Success of the setting.
     */
    virtual bool set_init_value(const std::string &parname, const std::string &value) = 0;
    
    /// Get a parameter's value (string representation). Independent of the implicit or explicit status.
    /**
     * @param parname  Full hierarchical name of the parameter whose value should be returned.
     * @return  Value of the parameter, converted to the user-chosen type
     */
    virtual const std::string get_string(const std::string &parname) = 0;

    /// Get a parameter pointer.
    /**
     * @param   parname   Full hierarchical parameter name.
     * @return  Pointer to the parameter object (NULL if not existing).
     */ 
    virtual cci_param_base* get_param(const std::string &parname) = 0;
    
    /// Checks whether a parameter exists (implicit or explicit).
    /**
     * @param parname  Full hierarchical parameter name.
     * @return Whether the parameter <parname> exists in the ConfigPlugin.
     */
    virtual bool exists_param(const std::string &parname) = 0;

    
    // //////////////////////////////////////////////////////////////////// //
    // ////////////////   Get Parameter List   //////////////////////////// //

    
    /// Return a list of all parameters existing (TODO implicit and explicit?) in the registry.
    /**
     * @return Vector with full hierarchical parameter names.
     */
    virtual const std::vector<std::string> get_param_list() = 0;

    
    // //////////////////////////////////////////////////////////////////// //
    // /////////////////   Callback Handling   //////////////////////////// //

    
    // Makro for registering new parameter callback functions (see method registerCallback).
  #define REGISTER_NEW_PARAM_CALLBACK(class, method)                \
  registerNewParamCallback(new gs::cnf::CallbAdapt< class >(this, &class::method));
    

    /// Register callback function for notifications of new added (or first time implicitely set) parameters.
    /**
     * The callback works even during initialize-mode (elaboration time).
     *
     * Inside the callback functions no waits are allowed!
     *
     * The user may register any methods as callback functions which have
     * the following signature:
     * \code
     * void method_name(const std::string parname, const std::string value);
     * \endcode
     *
     *
     * Usage example:
     * \code
     * class MyIP_Class
     * : public sc_core::sc_module
     * {
     * public:
     *   // some code [...]
     *   
     *   GC_nfApi my_GCnf_Api;
     *
     *   // Example code to register callback function
     *   void main_action() {
     *     // some code, parameters etc...
     *     my_GCnf_Api.REGISTER_NEW_PARAM_CALLBACK(MyIP_Class, config_callback);
     *   }
     *
     *   // Callback function with default signature.
     *   void config_callback(const std::string parname, const std::string value) {
     *     // some action
     *   }
     * };
     * \endcode
     *
     * This method registers a callback which is done each time a parameter is added
     * (without being an implicit parameter before) to the Config Plugin or an 
     * (implicit) parameters value is set the first time (without being added before).
     *
     * @param callb      Pointer to the CallbAdapt_b object which contains the object pointer
     *                   and the member function pointer.
     * @return           Success of registering.
     */
   // TODO virtual bool registerNewParamCallback(CallbAdapt_b *callb) = 0;
    
    /// Internal callback function that is called by the parameters. Deprecated!
    /** 
     * Registering is done in the methods getUpdateEvent (deprecated part) and registerCallback (deprecated)
     *
     * Does not perform the callback if the parameter is during destruction.
     * The legacy target module does not await this information and it 
     * cannot be transfered by the string value attribute.
     */
    virtual void legacy_callback_adapter_method(cci_param_base &par) = 0;
    
    /// Unregisters all callbacks (within all existing parameters) for the specified observer module. 
    /**
     * Currently not used.
     *
     * Could be used by a observer's destructor instead of the 
     * macro <code>GC_UNREGISTER_CALLBACKS();</code>.
     *
     * @param observer   Pointer to the observer module who did register parameter callbacks.
     */
    virtual void unregisterAllCallbacks(void* observer) = 0;
    
    
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
    virtual bool add_param(cci_param_base* par) = 0;
    
    /// Remove a parameter from the registry. Called by the parameter destructor.
    /**
     * Checks if destruction flag is set (so this may only be called by the
     * parameter destructor).
     *
     * @param par Parameter pointer.
     * @return Success of remove.
     */
    virtual bool removePar(cci_param_base* par) = 0;
    

  public:
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Optional functions   ///////////////////////////// //

    
    /// Set an alias to a parameter name
    /**
     * TODO: Guideline for other functions, e.g.:
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
    virtual template<class T>
    cci_param<T>* get_cci_param(const std::string &parname) = 0;
    
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
