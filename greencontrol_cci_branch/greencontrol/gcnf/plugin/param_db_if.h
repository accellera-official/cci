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
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doygen comments

#ifndef __PARAM_DB_IF_H__
#define __PARAM_DB_IF_H__

#include "greencontrol/core/gc_globals.h"


namespace gs {
namespace cnf {

  
// forward declaration
class gs_param_base;

/// Interface to allow the ConfigPlugin to get access to a parameter database.
/**
 * Each database that should be used by the ConfigPlugin has implement this interface
 * to be used as channel in the port in the ConfigPlugin.
 */
struct param_db_if
{
public:

  virtual ~param_db_if() { }
  
  /// Add an explicit parameter to the database (or change implicit->explicit).
  /** 
   * If the parameter does not yet exist as implicit parameter, create an new parameter.
   * If the parameter already exists as implicit parameter, add the pointer (then the
   * parameter is explicit) and set the init value.
   *
   * The parameter is added as explicit because this call may only be done by
   * the module which owns the parameter itself.
   *
   * If the parameter already exists as implicit parameter, change it to an explicit
   * parameter but do not overwrite the value with the initial value overgiven here.
   *
   * This impacts the is_used status!
   *
   * If the parameter already exists as explicit parameter, return an error (false).
   *
   * Note: this method (and all related methods) must not call any of the 
   *       pure virtual functions in gs_param_base because this method is 
   *       called by the gs_param_base constructor.
   *
   * @param par Pointer to the parameter object. Parameter name has to be hierarchical
   *                     (Delimiter-separated).
   * @return             Success. Is false if the parameter already exists as explicit.
   */
  virtual bool addParam(gs_param_base *par) = 0;

  /// Removes the parameter from the database (because it will be deleted)
  /**
   * Removes the parameter object independent from implicit/explicit
   * while keeping the initial value (and keeping the is_used status).
   *
   * @param par  Parameter pointer to the parameter that should be removed.
   * @return     If the removement was successful.
   */
  virtual bool removeParam(gs_param_base *par) = 0;

  /// Set a parameter's init value. If parameter already exists: set value; if not: create implicit parameter
  /**
   * If the parameter does not yet exist, create an implicit parameter.
   * The init value of an implicit parameter has priority to the 
   * initial value of a new added parameter. This database has to take
   * care of that!
   *
   * If the parameter already exists, update (overwrite) the value.
   *
   * If the init value is locked, do nothing.
   *
   * @param hier_parname Hierarchical parameter name (Delimiter-separated).
   * @param value        init value which has to be set. The default value of an implicit parameter has priority to the initial value of a new added parameter.
   * @return             true: parameter was not yet existing in database and was added
   *                     false: parameter exists and was updated
   */
  virtual bool setInitValue(const std::string &hier_parname, const std::string &value) = 0;

  /// Lock a parameter's init value. 
  /**
   * Lock so that this parameter's init value cannot be overwritten by
   * any subsequent setInitValue call. This allows to emulate a hierarchical
   * precendence since a top-level module can prevent the childs from setting
   * init values by locking the init value before creating the subsystem.
   *
   * Returns false (and does not lock) if parameter is already existing 
   * explicitely.
   * Returns false (and does not lock) if no initial value is existing
   * that can be locked.
   *
   * @param parname Hierarchical parameter name.
   * @return        Success of the lock (false if already locked or already explicitely existing).
   */
  virtual bool lockInitValue(const std::string &parname) = 0;
  
  /// If a parameter's init value is locked. 
  /**
   * Returns true when the implicit parameter's init value is locked,
   * returns also true if the init value of an explicit parameter is 
   * marked as locked.
   * Returns false if it is not locked or if this parameter does not 
   * yet exist implicitely at all.
   *
   * @param parname Hierarchical parameter name.
   * @return        If the init value is locked (true also if explicite already)
   */
  virtual bool initValueLocked(const std::string &parname) = 0;

  /// Get a parameter's value (independent of the implicit/explicit state of the parameter).
  /**
   * If the parameter does not exist, return empty string.
   * If the parameter is implicit, return the value that has been set 
   * using the setInitValue call.
   * If the parameter is explicit, return the current value.
   *
   * This impacts the is_used status!
   *
   * @param hier_parname Hierarchical parameter name (Delimiter-separated).
   * @param not_impact_is_used_status  If this get call shall impact the is_used status of this potentially implicit parameter. Shall only be set to true when called by some analysis (e.g. functional coverage) tools.
   * @return             Value of the param (init value if implicit).
   */
  virtual std::string getValue(const std::string &hier_parname, const bool not_impact_is_used_status = false) = 0;
  
  /// Get a parameter pointer.
  /**
   * Is never called with a not existing parameter.
   *
   * @param hier_parname Hierarchical parameter name (Selimiter-separated).
   * @return             Pointer to the parameter.
   */ 
  virtual gs_param_base* getParam(const std::string &hier_parname) = 0;

  /// Checks if a parameter exists (implicit or explicit).
  /**
   * Should be implemented fast; is called before getParam.
   *
   * @param hier_parname  Hierarchical parameter name whose existence should be checked.
   * @return           If the parameter exists.
   */
  virtual bool existsParam(const std::string &hier_parname) = 0;

  /// Checks if the parameter is explicit or not
  /**
   * May only be called if the parameter exists.
   *
   * @param hier_parname  Hierarchical parameter name.
   * @return              If the parameter is explicit.
   */
  virtual bool is_explicit(const std::string &hier_parname) = 0;

  /// Returns if the parameter has ever been used.
  /**
   * A parameter has been used if there is/was either a parameter object
   * mapped to the initial value, or the initial value has ever been read.
   * If there is no implicit/explicit parameter with this name this returns false.
   *
   * Note: exists_param, lock_init_value, set_init_value, get_param_list 
   *       shall not impact the is_used status.
   *
   * @param hier_parname  Hierarchical parameter name.
   * @return If the parameter is or has been used.
   */
  virtual bool is_used(const std::string &hier_parname) = 0;
  
  /// Returns a vector of strings of all parameters existing in the database.
  /**
   * @return All parameters (implicit and explicit) in the ConfigPlugin.
   */
  virtual const std::vector<std::string> getParametersVector() = 0;

  /// Returns string of all parameters existing in the database.
  /**
   * The parameters are listed comma separated.
   *
   * @return All parameters in the ConfigPlugin.
   */
  virtual const std::string getParameters() = 0;

};


} // end namespace cnf
} // end namespace gs

#endif
