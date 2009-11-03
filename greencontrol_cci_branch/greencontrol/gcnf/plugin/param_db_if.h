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

// doygen comments

#ifndef __PARAM_DB_IF_H__
#define __PARAM_DB_IF_H__

#include "greencontrol/core/gc_globals.h"


namespace gs {
namespace cnf {

  
// forward declaration
class gs_param_base;

/// SystemC interface to allow the ConfigPlugin to get access to a parameter database.
/**
 * Each database that should be used by the ConfigPlugin has implement this interface
 * to be used as channel in the port in the ConfigPlugin.
 */
struct param_db_if
: virtual public sc_interface
{
public:

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
   * Removes the parameter independent from implicit/explicit.
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
   * @param hier_parname Hierarchical parameter name (Delimiter-separated).
   * @param value        init value which has to be set. The default value of an implicit parameter has priority to the initial value of a new added parameter.
   * @return             true: parameter was not yet existing in database and was added
   *                     false: parameter exists and was updated
   */
  virtual bool setInitValue(const std::string &hier_parname, const std::string &value) = 0;
  
  /// Get a parameter's value (independent of the implicit/explicit state of the parameter).
  /**
   * If the parameter does not exist, return empty string.
   * If the parameter is implicit, return the value that has been set 
   * using the setInitValue call.
   * If the parameter is explicit, return the current value.
   *
   * @param hier_parname Hierarchical parameter name (Delimiter-separated).
   * @return             Value of the param (init value if implicit).
   */
  virtual std::string getValue(const std::string &hier_parname) = 0;
  
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
