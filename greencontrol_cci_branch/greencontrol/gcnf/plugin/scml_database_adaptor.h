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

#ifndef __SCML_DATABASE_APAPTOR_H__
#define __SCML_DATABASE_APAPTOR_H__

#include <string>
#include <iostream>
#include <exception>

#include "greencontrol/gcnf/plugin/config_globals.h"
#include "greencontrol/gcnf/plugin/utils.h"
#include "param_db_if.h"

#include <scml.h>


namespace gs {
namespace cnf {


/// Database channel which connects to CoWare scml library, used by the ConfigPlugin.
/**
 * Parameter database which reads scml parameters and provides them to the 
 * GreenConfig framework. writing parameters is forbidden.
 *
 * This is a SystemC channel which implements param_db_if to be bound
 * to a port sc_port<param_db_if> in the ConfigPlugin.
 */
class Scml_database_adaptor
: public sc_core::sc_module,
  public param_db_if
{

public:

  class DatabaseException: public std::exception {
    virtual const char* what() const throw()
    {
      return "Param does not exist!";
    }
  };

  /// Constructor with name
  /**
   * @param name  Name of this object.
   */
  Scml_database_adaptor(sc_core::sc_module_name name) 
  : sc_core::sc_module(name)
  { 
    // Get the (only) registry instance
    scml_reg = &scml_property_registry::inst();
  }
  
private:
  /// Constructor without parameter must not be used!
  Scml_database_adaptor() { sc_assert(false); }
  
public:

  /// @see gs::cnf::param_db_if::addParam
  bool addParam(const std::string &hier_parname, const std::string &value) {
    GCNF_DUMP_N(name(), "addParam("<<hier_parname.c_str()<<")");
    //SC_REPORT_WARNING(name(), "addParam: Scml database adaptor does not provide writing to parameters.");
    return true;
  }

  /// @see gs::cnf::param_db_if::setParam
  bool setParam(const std::string &hier_parname, const std::string &value) {
    GCNF_DUMP_N(name(), "setParam("<<hier_parname.c_str()<<", "<<value.c_str()<<")");      
    //SC_REPORT_WARNING(name(), "setParam: Scml database adaptor does not provide writing to parameters.");
    return true;
  }

  /// @see gs::cnf::param_db_if::getParam
  const std::string getParam(const std::string &hier_parname) {
    GCNF_DUMP_N(name(), "getParam("<<hier_parname.c_str()<<")");
    try {
      return getValue(hier_parname);
    }
    catch (DatabaseException &e) {
      GCNF_DUMP_N(name(), "Parameter not existing.");
      return string("");
    }
  }
  
  /// @see gs::cnf::param_db_if::existsParam
  bool existsParam(const std::string &hier_parname) {
    GCNF_DUMP_N(name(), "existsParam("<<hier_parname.c_str()<<")");
    try {
      getValue(hier_parname);
    } 
    catch (DatabaseException &e) {
      GCNF_DUMP_N(name(), "Parameter not existing.");
      return false;
    }
    return true;
  }

  /// @see gs::cnf::param_db_if::is_explicit
  /**
   * All scml parameters are explicit.
   */
  bool is_explicit(const std::string &hier_parname) {
    GCNF_DUMP_N(name(), "is_explicit("<<hier_parname.c_str()<<")");
    return true;
  }

  /// @see gs::cnf::param_db_if::getParametersVector
  const std::vector<std::string> getParametersVector() {
    GCNF_DUMP_N(name(), "getParameters()");      
    SC_REPORT_ERROR(name(), "getParameters: Scml database adaptor does not provide parameter listing.");
    return std::vector<std::string>();
  }

  /// @see gs::cnf::param_db_if::getParameters
  const std::string getParameters() {
    GCNF_DUMP_N(name(), "getParameters()");      
    SC_REPORT_ERROR(name(), "getParameters: Scml database adaptor does not provide parameter listing.");
    return string("");
  }

protected:

  const std::string getValue(const std::string &nam) throw(DatabaseException) {
    GCNF_DUMP_N(name(), "getValue("<<nam.c_str()<<")");
    #ifdef DEBUG
      if( !isHierarchicalParameterName(nam) ) {
        SC_REPORT_ERROR(name(), "getValue: Tried to get a parameter which is not hierarchical given.");
      }
    #endif
    
    bool existing = false;
    std::string value;

    const std::string moduleName = nam.substr(0, nam.find_last_of(SC_NAME_DELIMITER));
    const std::string name = nam.substr(nam.find_last_of(SC_NAME_DELIMITER)+1, nam.length()-nam.find_last_of(SC_NAME_DELIMITER)-1);

    // get parameter out of registry (Property arbitrary)
    // Try all types
    // Try int
    if (scml_reg->hasProperty(scml_property_registry::MODULE, scml_property_registry::INTEGER, moduleName, name)) {
      int tmp = scml_reg->getIntProperty(scml_property_registry::MODULE, moduleName, name);
      existing = true;
      std::stringstream s;
      s << tmp;
      value = s.str();
      GCNF_DUMP_N("Scml_database_adaptor", "getValue Int: '"<<value.c_str()<<"'");
    }
    // Try bool
    else if (scml_reg->hasProperty(scml_property_registry::MODULE, scml_property_registry::BOOL, moduleName, name)) {
      bool tmp = scml_reg->getBoolProperty(scml_property_registry::MODULE, moduleName, name);
      existing = true;
      std::stringstream s;
      s << tmp;
      value = s.str();
      GCNF_DUMP_N("Scml_database_adaptor", "getValue Int: '"<<value.c_str()<<"'");
    }
    // Try double
    else if (scml_reg->hasProperty(scml_property_registry::MODULE, scml_property_registry::DOUBLE, moduleName, name)) {
      double tmp = scml_reg->getDoubleProperty(scml_property_registry::MODULE, moduleName, name);
      existing = true;
      std::stringstream s;
      s << tmp;
      value = s.str();
      GCNF_DUMP_N("Scml_database_adaptor", "getValue Int: '"<<value.c_str()<<"'");
    }
    // Try string
    else if (scml_reg->hasProperty(scml_property_registry::MODULE, scml_property_registry::STRING, moduleName, name)) {
      value = scml_reg->getStringProperty(scml_property_registry::MODULE, moduleName, name);
      existing = true;
      GCNF_DUMP_N("Scml_database_adaptor", "getValue Int: '"<<value.c_str()<<"'");
    }
    
    if (!existing) {
      SC_REPORT_WARNING(this->name(), "Param does not exist!");
      throw DatabaseException();
    }

    return value;
  }
  
  /// scml registry
  scml_property_registry *scml_reg;

};


} // end namespace cnf
} // end namespace gs

#endif
