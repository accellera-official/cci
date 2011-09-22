//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doxygen comments

#ifndef __CNF_API_IF_H__
#define __CNF_API_IF_H__


namespace gs {
namespace cnf {


// forward declaration 
class gs_param_base;
  
template <class T> class gs_param;

/// GreenControl Config API 'interface'.
/**
 * This interface is implemented by the 
 * GCnf_Api and the GCnf_private_Api.
 *
 * A pointer of this if type is returned by the  static function
 * returning the API instance (see gs::cnf::GCnf_Api) to be used 
 * by each user module  independent from the underlying API type.
 *
 * Some special functions are implemented not virtual.
 * They are template functions (which cannot be virtual) and which
 * are independent of the API type.
 */
class cnf_api_if
{
  
public:
  
  virtual ~cnf_api_if() {}

  // -------------------- deprecated functions ----------------------------
  
  /// Add a parameter without initial value to the ConfigPlugin. Does not work with new design. DEPRECATED!
  /**
   * A gs_param instance will be created at the plugin.
   */
  virtual bool addParam(const std::string &parname) = 0;
  
  /// Add a parameter with initial value to the ConfigPlugin. DEPRECATED!
  /**
   * A gs_param instance will be created at the plugin.
   */
  virtual bool addParam(const std::string &parname, const std::string &default_val, std::string meta_data = "") = 0;
  
  /// Set a parameter's value. Deprecated!
  /**
   * @param parname Hierarchical parameter name.
   * @param value   Value the parameter has to be set to.
   * @return        Success of the setting.
   */
  virtual bool setParam(const std::string &parname, const std::string &value, std::string meta_data = "") = 0;
  
  /// Get a parameter's value. Deprecated!
  /**
   * @param   parname Hierarchical parameter name.
   * @return  Value (string representation) of the parameter <parname>.
   */ 
  virtual const std::string& getParam(const std::string &parname, std::string meta_data = "" ) = 0;
  
  // -------------------- END deprecated functions ----------------------------

  /// Add a parameter to the ConfigPlugin.
  /** 
   * Note: addPar (and all related methods) must not call any of the 
   *       pure virtual functions in gs_param_base because this method is 
   *       called by the gs_param_base constructor.
   *
   * @param par Parameter (including name and value).
   * @return Success of the adding.
   */
  virtual bool addPar(gs_param_base* par, std::string meta_data = "") = 0;
  
  /// Remove a parameter from the ConfigPlugin. Called by the parameter destructor.
  /**
   * Checks if destruction flag is set (so this may only be called by the
   * parameter destructor).
   *
   * @param par Parameter pointer.
   * @return Success of remove.
   */
  virtual bool removePar(gs_param_base* par, std::string meta_data = "") = 0;
  
  /// Set a parameter's init value. 
  /**
   * The init value has priority to the default value set by the owner!
   * If called for an explicit parameter this is a usual set, including not
   * setting the is_initial_value state to true!
   *
   * @param parname Hierarchical parameter name.
   * @param value   Init value the parameter has to be set to.
   * @return        Success of the setting.
   */
  virtual bool setInitValue(const std::string &parname, const std::string &value, std::string meta_data = "") = 0;

  /// Lock a parameter's init value. 
  /**
   * Lock so that this parameter's init value cannot be overwritten by
   * any subsequent setInitValue call. This allows to emulate a hierarchical
   * precendence since a top-level module can prevent the childs from setting
   * init values by locking the init value before creating the subsystem.
   *
   * Returns false (and does not lock) if no initial value is existing
   * that can be locked or if an initial value is already locked or if the
   * parameter is already existing as object (explicit parameter).
   *
   * @param parname Hierarchical parameter name.
   * @return        Success of the lock (false if already locked).
   */
  virtual bool lockInitValue(const std::string &parname, std::string meta_data = "") = 0;
  
  /// Get a parameter's value (string representation). Independent of the implicit or explicit status.
  /**
   * @param parname  Hierarchical name of the parameter whose value should be returned.
   * @param not_impact_is_used_status  If this get call shall impact the is_used status of this potentially implicit parameter. Shall only be set to true when called by some analysis (e.g. functional coverage) tools.
   * @return  Value of the parameter, converted to the user-chosen type
   */
  virtual const std::string getValue(const std::string &parname, std::string meta_data = "", const bool not_impact_is_used_status = false) = 0;

  // TODO
  //template<class T>
  //gs_param<T>* get_gs_param(const std::string &parname);

  /// Get a parameter pointer.
  /**
   * @param   parname   Hierarchical parameter name.
   * @return  Pointer to the parameter object (NULL if not existing).
   */ 
  virtual gs_param_base* getPar(const std::string &parname, std::string meta_data = "") = 0;
  
  /// Checks whether a parameter exists (implicit or explicit).
  /**
   * @param parname  Hierarchical parameter name.
   * @return Whether the parameter <parname> exists in the ConfigPlugin.
   */
  virtual bool existsParam(const std::string &parname, std::string meta_data = "") = 0;

  
  /// Returns if the parameter has ever been used.
  /**
   * A parameter has been used if there is/was either a parameter object
   * mapped to the initial value, or the initial value has ever been read
   * (from the broker/ConfigAPI). 
   * If there is no implicit/explicit parameter with this name this returns false.
   *
   * Note: exists_param, lock_init_value, set_init_value, get_param_list 
   *       shall not impact the is_used status.
   *
   * @param parname  Full hierarchical parameter name.
   * @return If the parameter is or has been used.
   */
  virtual bool is_used(const std::string &parname, std::string meta_data = "") = 0;

  /// If a parameter is explicit.
  /**
   * @param   parname   Hierarchical parameter name.
   * @return  If the parameter is explicit.
   */ 
  //bool is_explicit(const std::string &parname) = 0;
    
  /// Return a list of all parameters existing in the ConfigPlugin.
  /**
   * @return Vector with hierarchical parameter names.
   */
  virtual const std::vector<std::string> getParamList() = 0;
  
  /// Return a list of all parameter names (implicit and explicit) of the specified module (Use '.*' to include the parameters of the childs!).
  /**
   * The list contains all parameters of the module (not including it's childs).
   *
   * If module_name end with '.*' the childs are included!
   *
   * @param module_name Name of the module whose parameter list should be returned.
   * @return Vector with hierarchical parameter names.
   */
  virtual const std::vector<std::string> getParamList(const std::string &module_name, std::string meta_data = "") = 0;
  
  /// Return a list of all parameter names (implicit and explicit) of the specified module (and childs).
  /**
   * If including_childs: All parameters of the module including 
   *                      the parameters of it's child modules.
   *
   * If not including_childs: All direct parameters of the module.
   *
   * @param module_name       Name of the module whose parameter list should be returned.
   * @param including_childs  If the parameters of the child modules should be included.
   * @return Vector with hierarchical parameter names.
   */
  virtual const std::vector<std::string> getParamList(const std::string &module_name, bool including_childs, std::string meta_data = "") = 0;

  /// Return a pointer list of all (explicit) parameters of the specified module. (Use '.*' to include the parameters of the childs!)
  /**
   * The returned list contains all parameters of the module
   * whose name is given, not including it's childs.
   * If module_name end with '.*' the childs are included!
   *
   * @param module_name Name of the module whose parameter list should be returned.
   * @return Vector with hierarchical parameter names.
   */
  virtual const std::vector<gs_param_base*> getParams(const std::string &module_name = "") = 0;
  
  /// Returns an event which is notified when the parameter value changes. Deprecated!
  /**
   * Waiting for the returned event is not allowed during initialize-mode
   * (because of elaboration time). Instead of waiting use the callback
   * functions which work even during elaboration time.
   *
   * Throws RegisterObeserverFailedException if registering observer failed!
   *
   * @param parname   Name of the observed parameter
   * @return          Event which is notified at parameter change.
   */
  virtual sc_event& getUpdateEvent(const std::string &parname)  throw (RegisterObserverFailedException) = 0;
  
  // DEPRECATED Makro for registering callback functions (see method registerCallback). DEPRECATED
#define REGISTER_CALLBACK(class, method, parname)                \
registerCallback(parname, new gs::cnf::CallbAdapt< class >(this, &class::method));
  
  // Makro for registering new parameter callback functions (see method registerNewParamCallback).
#define REGISTER_NEW_PARAM_CALLBACK(class, method)                \
registerNewParamCallback(new gs::cnf::CallbAdapt< class >(this, &class::method));
  
  /// Registers an observer callback function (with the signature of callback_func_ptr). Use the REGISTER_CALLBACK macro!, Deprecated!
  /**
   * This method is deprecated! Use REGISTER_PARAM_CALLBACK(&my_param, class, method)
   * instead!
   *
   * For each parameter several callbacks may be registered.
   *
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
   *     my_GCnf_Api.REGISTER_CALLBACK(MyIP_Class, config_callback, "IP2.stringPar");
   *   }
   *
   *   // Callback function with default signature.
   *   void config_callback(const std::string parname, const std::string value) {
   *     // some action
   *   }
   * };
   * \endcode
   *
   * @param parname    Name of the parameter for which the callback is registered.
   * @param callb      Pointer to the CallbAdapt_b object which contains the object pointer
   *                   and the member function pointer.
   * @return           Success of registering.
   *
   * <em>Macro REGISTER_CALLBACK</em>
   *
   * Creates new CallbAdapt object and registers it at registerCallback.
   *
   * Usage:
   * \code
   * my_Api.REGISTER_CALLBACK(class_name, callback_method_name, parameter_name)
   * \endcode
   * Example:
   * \code
   * my_Api.REGISTER_CALLBACK(MyIP,       config_callback,      "IP2.stringPar")
   * \endcode
   */
  virtual bool registerCallback(const std::string &parname, CallbAdapt_b *callb) = 0;
  
  /// DEPRECATED: Returns an event which is notified when a new parameter is added or set implicitely the first time.
  /**
   * Waiting for the returned event is not allowed during initialize-mode
   * (because of elaboration time). Instead of waiting use the callback
   * functions which work even during elaboration time.
   *
   * Throws RegisterObeserverFailedException if registering observer failed!
   *
   * Attention: Most times this method is of no use because adding of parameters is done during
   *            elaboration and meanwhile no events are possible. Use the registerNewParamCallback
   *            method instead!
   *
   * This method returns an event which is notified each time a parameter is added
   * (without being an implicit parameter before) to the Config Plugin or an
   * (implicit) parameters value is set the first time (without being added before).
   *
   * @return          Event which is notified when new parameter is added.
   */
  virtual sc_event& getNewParamEvent() throw (RegisterObserverFailedException) = 0;
  
  /// Register callback function for notifications of new added (as explicite) or first time implicitely set parameters.
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
   *     // some action, make sure to check if explicit
   *   }
   * };
   * \endcode
   *
   * This method registers a callback which is called each time <br>
   * a) a parameter is added to the Config Plugin as an explicit parameter
   *    (even if it has been implicit before) and
   * b) a parameter's implicit value is set the first time (without being explicit before).
   *
   * @param callb      Pointer to the CallbAdapt_b object which contains the object pointer
   *                   and the member function pointer.
   * @return           Success of registering.
   */
  virtual bool registerNewParamCallback(CallbAdapt_b *callb) = 0;
  
  /// Internal callback function that is called by the parameters. Deprecated!
  /** 
   * Registering is done in the methods getUpdateEvent (deprecated part) and registerCallback (deprecated)
   *
   * Does not perform the callback if the parameter is during destruction.
   * The legacy target module does not await this information and it 
   * cannot be transfered by the string value attribute.
   */
  virtual void legacy_callback_adapter_method(gs_param_base &par) = 0;
  
  /// Unregisters all callbacks (within all existing parameters) for the specified observer module. 
  /**
   * Currently not used.
   *
   * Could be used by a observer's destructor instead of the 
   * macro <code>GC_UNREGISTER_CALLBACKS();</code>.
   *
   * @param observer   Pointer to the observer module who did register parameter callbacks.
   */
  virtual void unregisterAllCallbacks(void* observer, std::string meta_data = "") = 0;
  
  /// Get a parameter's value. Independent of the implicit or explicit status.
  /**
   * This function is placed within this interface because 
   * virtual template functions are not allowed.
   * This function calls the getValue() function of the derived API.
   *
   * The string value is converted to the user-chosen template type
   * by using the gs_param template specialization.
   *
   * This function does only work for types of gs_param<T>, not for 
   * any kind of gs_param_base, e.g. parameter arrays.
   *
   * @param parname  Hierarchical name of the parameter whose value should be returned.
   * @param value    The value of the parameter will be written to this parameter
   * @return         If the convertion was successfull.
   */
  template<class T>
  const bool getValue(const std::string &parname, T &value) {
    return gs_param<T>::static_deserialize(value, getValue(parname));
  }
  
  /// Get a parameter's value. Independent of the implicit or explicit status.
  /**
   * Better use getValue(parname, value) to avoid copying of the value.
   *
   * This function is placed within this interface because 
   * virtual template functions are not allowed.
   * This function calls the getValue() function of the derived API.
   *
   * The string value is converted to the user-chosen template type
   * by using the gs_param template specialization.
   *
   * This function does only work for types of gs_param<T>, not for 
   * any kind of gs_param_base, e.g. parameter arrays.
   *
   * @param parname  Hierarchical name of the parameter whose value should be returned.
   * @return         Value of the parameter, converted to the user-chosen type.
   */
  template<class T>
  const T getValue(const std::string &parname) {
    T val;
    gs_param<T>::static_deserialize(val, getValue(parname));
    return val;
  }

  /// Get a parameter pointer of the gs_param<T> type defined by the caller (does a dynamic_cast).
  /**
   * @param   parname   Hierarchical parameter name.
   * @return  Pointer to the parameter object (NULL if not existing or wrong type).
   */ 
  template<class T>
  gs_param<T>* get_gs_param(const std::string &parname) {
    return dynamic_cast<gs_param<T>*>(getPar(parname));
  }
  
};

/// DEPRECATED type for legacy support
typedef cnf_api_if cnf_api;
  
} // end namespace cnf
} // end namespace gs

#endif
