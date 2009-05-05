//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
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

// doxygen comments

#ifndef __CCI_CNF_API_H__
#define __CCI_CNF_API_H__


namespace cci {


// forward declaration 
class cci_param_base;
  
template <class T> class cci_param;

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
class cci_cnf_api
{
  
public:
  
  virtual ~cci_cnf_api() {}

  // -------------------- deprecated functions ----------------------------
  
  /// Add a parameter without initial value to the ConfigPlugin. Does not work with new design. DEPRECATED!
  /**
   * A cci_param instance will be created at the plugin.
   */
  virtual bool addParam(const std::string &parname) = 0;
  
  /// Add a parameter with initial value to the ConfigPlugin. DEPRECATED!
  /**
   * A cci_param instance will be created at the plugin.
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
   *       pure virtual functions in cci_param_base because this method is 
   *       called by the cci_param_base constructor.
   *
   * @param par Parameter (including name and value).
   * @return Success of the adding.
   */
  virtual bool addPar(cci_param_base* par, std::string meta_data = "") = 0;
  
  /// Remove a parameter from the ConfigPlugin. Called by the parameter destructor.
  /**
   * Checks if destruction flag is set (so this may only be called by the
   * parameter destructor).
   *
   * @param par Parameter pointer.
   * @return Success of remove.
   */
  virtual bool removePar(cci_param_base* par, std::string meta_data = "") = 0;
  
  /// Set a parameter's init value. 
  /**
   * The init value has priority to the initial value set by the owner!
   *
   * @param parname Hierarchical parameter name.
   * @param value   Init value the parameter has to be set to.
   * @return        Success of the setting.
   */
  virtual bool setInitValue(const std::string &parname, const std::string &value, std::string meta_data = "") = 0;
  
  /// Get a parameter's value (string representation). Independent of the implicit or explicit status.
  /**
   * @param parname  Hierarchical name of the parameter whose value should be returned.
   * @return  Value of the parameter, converted to the user-chosen type
   */
  virtual const std::string getValue(const std::string &parname, std::string meta_data = "") = 0;

  // TODO
  //template<class T>
  //cci_param<T>* get_cci_param(const std::string &parname);

  /// Get a parameter pointer.
  /**
   * @param   parname   Hierarchical parameter name.
   * @return  Pointer to the parameter object (NULL if not existing).
   */ 
  virtual cci_param_base* getPar(const std::string &parname, std::string meta_data = "") = 0;
  
  /// Checks whether a parameter exists (implicit or explicit).
  /**
   * @param parname  Hierarchical parameter name.
   * @return Whether the parameter <parname> exists in the ConfigPlugin.
   */
  virtual bool existsParam(const std::string &parname, std::string meta_data = "") = 0;

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
  virtual const std::vector<cci_param_base*> getParams(const std::string &module_name = "") = 0;
  
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
  virtual void unregisterAllCallbacks(void* observer, std::string meta_data = "") = 0;
  
  /// Get a parameter's value. Independent of the implicit or explicit status.
  /**
   * This function is placed within this interface because 
   * virtual template functions are not allowed.
   * This function calls the getValue() function of the derived API.
   *
   * The string value is converted to the user-chosen template type
   * by using the cci_param template specialization.
   *
   * This function does only work for types of cci_param<T>, not for 
   * any kind of cci_param_base, e.g. parameter arrays.
   *
   * @param parname  Hierarchical name of the parameter whose value should be returned.
   * @param value    The value of the parameter will be written to this parameter
   * @return         If the convertion was successfull.
   */
  template<class T>
  const bool getValue(const std::string &parname, T &value) {
    return cci_param<T>::static_deserialize(value, getValue(parname));
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
   * by using the cci_param template specialization.
   *
   * This function does only work for types of cci_param<T>, not for 
   * any kind of cci_param_base, e.g. parameter arrays.
   *
   * @param parname  Hierarchical name of the parameter whose value should be returned.
   * @return         Value of the parameter, converted to the user-chosen type.
   */
  template<class T>
  const T getValue(const std::string &parname) {
    T val;
    cci_param<T>::static_deserialize(val, getValue(parname));
    return val;
  }

  /// Get a parameter pointer of the cci_param<T> type defined by the caller (does a dynamic_cast).
  /**
   * @param   parname   Hierarchical parameter name.
   * @return  Pointer to the parameter object (NULL if not existing or wrong type).
   */ 
  template<class T>
  cci_param<T>* get_cci_param(const std::string &parname) {
    return dynamic_cast<cci_param<T>*>(getPar(parname));
  }
  
};

    
} // end namespace cci

#endif
