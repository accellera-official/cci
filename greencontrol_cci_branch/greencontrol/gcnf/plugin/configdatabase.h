//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2010 : GreenSocs Ltd
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

#ifndef __CONFIGDATABASE_H__
#define __CONFIGDATABASE_H__

#include <map>
#include <list>
#include <vector>
#include <set>

#include "config_globals.h"
#include "utils.h"
#include "param_db_if.h"


namespace gs {
namespace cnf {


/// Database for configurable parameters, used by the ConfigPlugin.
/**
 * Parameter database which stores parameters for the ConfigPlugin.
 *
 * This implements param_db_if to be used in the ConfigPlugin.
 *
 * Storage is done in a std::map of type string, string.
 * The key is the hierarchical parameter name, the value is its
 * value's string representation.
 *
 * \todo The value has to contain the information of the api type where
 *       it belongs to.
 */
template<typename gs_param_base_T>
class ConfigDatabase_T
: public param_db_if
{
  typedef std::set<cport_address_type> addressSet;

protected:

  /// Struct for parameter information to be saved in map
  struct parameter {

    /// The init value (string representation) of the parameter. Only valid if had_initial_value is true.
    std::string init_value;
    
    /// Lock for the init value (only of interest if param == NULL)
    bool init_value_locked;

    /// If this parameter had an initial value
    bool had_initial_value;
   
    /// Status if this parameter ever has been used (object or read init value)
    bool is_used;

    /// The parameter pointer (NULL if implicit)
    gs_param_base_T* param;

    /// Contructor
    parameter(gs_param_base_T* par)
    : init_value_locked(false)
    , had_initial_value(false)
    , is_used(false) {
      param = par;
      if (param != NULL) is_used = true;
    }

    /// Contructor with init value
    parameter(gs_param_base_T* par, const std::string ivalue)
    : init_value_locked(false)
    , had_initial_value(true)
    , is_used(false) {
      if (par != NULL) SC_REPORT_ERROR("ConfigDtabase::parameter constructor", "When creating implicit parameter with init value you cannot set the parmeter pointer!");
      assert(par == NULL);
      init_value = ivalue;
      param = NULL;
    }
  };

  //               key=parname, value=parameter
  typedef std::map<std::string, parameter> param_map;
  typedef typename param_map::iterator              param_iterator;
  param_map m_parameter_database;

public:
  /// Constructor with name
  /**
   * @param name  Name of this object.
   */
  ConfigDatabase_T(const char* name) 
  : m_name(name)
  {  }
  
  ~ConfigDatabase_T() {
    GCNF_DUMP_N(name(), "DESTRUCTOR ConfigDatabase");
  }
  
private:
  /// Constructor without parameter must not be used!
  ConfigDatabase_T() { assert(false); }
  
public:

  /// @see gs::cnf::param_db_if::addParam
  /* Note: this method (and all related methods) must not call any of the 
   *       pure virtual functions in gs_param_base because this method is 
   *       called by the gs_param_base constructor.*/
  bool addParam(gs_param_base_T* par) {
    assert(par != NULL);
    // if( isHierarchicalParameterName(par->getName()) ) { // now allowed for top-level params
    param_iterator pos;
    pos = m_parameter_database.find(par->getName());
    if (pos != m_parameter_database.end()) {
      // Parameter already exists
      // If parameter is explicit, error:
      if (pos->second.param != NULL) {
        GCNF_DUMP_N(name(), "addParam("<<par->getName().c_str()<<") (type: "<<par->getTypeString()<<"): Error: Parameter is already existing (explicit) (type: "<<(pos->second.param)->getTypeString()<<")!");
        std::string mes("addParam: Parameter '"); mes += par->getName(); mes += "' is already existing (explicit)!";
        SC_REPORT_WARNING(name(), mes.c_str());
        // check type
        if ((pos->second.param)->getType() != par->getType()
            || (pos->second.param)->getTypeString() != par->getTypeString()) {
          GCNF_DUMP_N(name(), "addParam("<<par->getName().c_str()<<"): Error: Existing explicit Parameter is of different type ("<<(pos->second.param)->getTypeString()<<") than the new one ("<<par->getTypeString()<<")!");
          std::string mes("addParam: Existing explicit Parameter '"); mes += par->getName(); mes += "' is of different type!";
          SC_REPORT_ERROR(name(), mes.c_str());
        }
        // set init value anyway which has priority!
        parameter *p = &pos->second;
        if (p->had_initial_value) {
          GCNF_DUMP_N(name(), "addParam("<<par->getName().c_str()<<"): set init value to '"<<p->init_value.c_str()<<"'");
          par->m_setting_initial_value_now = true; // mark the following setString as initial value write
          par->setString(p->init_value);
          par->m_is_initial_value = true; // mark the parameter's value as initial value
        }
        p->is_used = true;
        return false;
      } 
      // If parameter implicit, make it explicit and discard the value
      else {
        GCNF_DUMP_N(name(), "addParam("<<par->getName().c_str()<<"): change parameter to explicit");
        parameter *p = &pos->second;
        p->param = par;
        // set init value which has priority!
        if (p->had_initial_value) {
          GCNF_DUMP_N(name(), "addParam("<<par->getName().c_str()<<") (type: "<<par->getTypeString()<<"): set init value to '"<<p->init_value.c_str()<<"'");
          par->m_setting_initial_value_now = true; // mark the following setString as initial value write
          p->param->setString(p->init_value);
          par->m_is_initial_value = true; // mark the parameter's value as initial value
        }
        p->is_used = true;
      }
    }
    // If parameter doesn't exist, create it as explicit with the initial value
    else {
      GCNF_DUMP_N(name(), "addParam("<<par->getName().c_str()<<") (type: "<<par->getTypeString()<<"): add new explicit parameter");
      param_iterator iter = (m_parameter_database.insert( make_pair(par->getName(), parameter(par)) )).first;
      iter->second.is_used = true;
    }
    // now allowed for top-level params
    //} else {
    //  GCNF_DUMP_N(name(), "addParam("<<par->getName().c_str()<<"): Error: Tried to add a parameter which is not hierarchical given.");
    //  SC_REPORT_ERROR(name(), "addParam: Tried to add a parameter which is not hierarchical given.");
    //  return false;
    //}
    return true;
  }

  /// @see gs::cnf::param_db_if::removeParam
  bool removeParam(gs_param_base_T *par) {
    param_iterator pos;
    pos = m_parameter_database.find(par->getName());
    if (pos != m_parameter_database.end()) {
      // leave the entry in the db and only delete the pointer
      // (the parameter is kept as an implicit one with the init value)
      //   old: delete the whole entry (including the init value): 
      //        m_parameter_database.erase(pos);
      //   new:
      pos->second.param = NULL;
      GCNF_DUMP_N(name(), "removeParam("<<par->getName().c_str()<<") parameter removed successfully.");      
    } else {
      GCNF_DUMP_N(name(), "removeParam("<<par->getName().c_str()<<") error removing parameter");      
      SC_REPORT_WARNING(name(), "removeParam: Tried to remove a not existing parameter!");
      return false;
    }
    return true;
  }

  /// @see gs::cnf::param_db_if::setInitValue
  bool setInitValue(const std::string &hier_parname, const std::string &ivalue) {
    GCNF_DUMP_N(name(), "setInitValue("<<hier_parname.c_str()<<", "<<ivalue.c_str()<<")");      

    param_iterator pos;
    pos = m_parameter_database.find(hier_parname);
    // If parameter not exists
    if (pos == m_parameter_database.end()) {
      GCNF_DUMP_N(name(), "setInitValue: parameter not yet exists: create implicit parameter.");
      m_parameter_database.insert( make_pair(hier_parname, parameter(NULL, ivalue)) ); // had_initial_value gets true here
      return true; // parameter was new added to database
    }
    // If parameter exists
    else {
      // if parameter is implicit: warning
      if (!is_explicit(hier_parname)) {
        // Check for lock
        if (pos->second.init_value_locked) {
          GCNF_DUMP_N(name(), "setInitValue: Parameter init value is locked!");
          SC_REPORT_WARNING(name(), "setInitValue: Parameter init value is locked!");
          return false; // note: this is NOT an error code, it just means that the parameter was not added as new one!
        } 
        GCNF_DUMP_N(name(), "setInitValue: Parameter already exists as implicit parameter. Do not set the initial value twice!");
        SC_REPORT_WARNING(name(), "setInitValue: Parameter already exists as implicit parameter. Do not set the initial value twice!");
        pos->second.init_value = ivalue;
        pos->second.had_initial_value = true;
      }
      else {
        parameter *par = &pos->second;
        par->param->setString(ivalue);
      }
      // had/is_initial_value is NOT been set here true because this is a usual set of an explicit parameter
    }
    return false; // parameter not new
  }

  /// @see gs::cnf::param_db_if::lockInitValue
  bool lockInitValue(const std::string &hier_parname) {
    GCNF_DUMP_N(name(), "lockInitValue("<<hier_parname.c_str()<<")");      
    param_iterator pos;
    pos = m_parameter_database.find(hier_parname);
    // If parameter not exists
    if (pos == m_parameter_database.end()) {
      SC_REPORT_WARNING(name(), "lockInitValue: Parameter no yet exists! Cannot lock a not existing init value!");
      GCNF_DUMP_N(name(), "lockInitValue: Parameter not yet exists! Cannot lock a not existing init value!");
      return false;
    }
    // If parameter exists
    else {
      // if parameter is explicit: warning
      if (is_explicit(hier_parname)) {
        SC_REPORT_WARNING(name(), "lockInitValue: Parameter already exists explicitely! Cannot lock an init value of an explicit parameter!");
        GCNF_DUMP_N(name(), "lockInitValue: Parameter already exists explicitely! Cannot lock an init value of an explicit parameter!");
        return false;
      }
      else {
        if (pos->second.init_value_locked) {
          SC_REPORT_WARNING(name(), "lockInitValue: Parameter init value already locked!");
          GCNF_DUMP_N(name(), "lockInitValue: Parameter init value already locked!");
          return false;
        }
        pos->second.init_value_locked = true;
      }
    }
    return true;
  }
  
  /// @see gs::cnf::param_db_if::initValueLocked
  bool initValueLocked(const std::string &hier_parname) {
    GCNF_DUMP_N(name(), "initValueLocked("<<hier_parname.c_str()<<")");      
    param_iterator pos;
    pos = m_parameter_database.find(hier_parname);
    // If parameter not exists
    if (pos == m_parameter_database.end()) {
      GCNF_DUMP_N(name(), "initValueLocked: Parameter not yet exists!");
      return false;
    }
    return pos->second.init_value_locked;
  }

  /// @see gs::cnf::param_db_if::getValue
  std::string getValue(const std::string &hier_parname, const bool not_impact_is_used_status = false) {
    GCNF_DUMP_N(name(), "getValue("<<hier_parname.c_str()<<")");
    gs_param_base_T* par = getParam(hier_parname);
    // return explicit value
    if (par) {
      return par->getString();
    }
    // return init value
    else {
      param_iterator pos = m_parameter_database.find(hier_parname);
      if (pos != m_parameter_database.end()) {
        // If parameter implicit, return the value
        if (pos->second.param == NULL) {
          parameter *p = &pos->second;
          if (!not_impact_is_used_status) 
            p->is_used = true; // is_used status now true
          return p->init_value;
        }
        // If parameter is explicit, should never happen!
        else assert(false);
      }
      // Parameter does not exist: return no value
      else
        return "";
    }
    return "";
  }

  /// @see gs::cnf::param_db_if::getParam
  gs_param_base_T* getParam(const std::string &hier_parname) {
    GCNF_DUMP_N(name(), "getParam("<<hier_parname.c_str()<<")");      

    param_iterator pos;
    pos = m_parameter_database.find(hier_parname);
    if (pos == m_parameter_database.end()) {
      SC_REPORT_WARNING(name(), "Param does not exist!");
      return NULL;
    }
    return pos->second.param;
  }

  /// @see gs::cnf::param_db_if::existsParam
  bool existsParam(const std::string &hier_parname) {
    param_iterator pos;
    pos = m_parameter_database.find(hier_parname);
    if (pos == m_parameter_database.end()) {
      GCNF_DUMP_N(name(), "existsParam("<<hier_parname.c_str()<<") = false");      
      return false;
    }
    GCNF_DUMP_N(name(), "existsParam("<<hier_parname.c_str()<<") = true");      
    return true;
  }

  /// @see gs::cnf::param_db_if::is_explicit
  bool is_explicit(const std::string &hier_parname) {
    GCNF_DUMP_N(name(), "is_explicit("<<hier_parname.c_str()<<")");
    param_iterator pos;
    pos = m_parameter_database.find(hier_parname);
    if (pos == m_parameter_database.end()) {
      SC_REPORT_ERROR(name(), "is_explicit: Tried to check a not existing parameter.");
      return false;
    }
    return (pos->second.param != NULL);
  }

  /// @see gs::cnf::param_db_if::is_used
  bool is_used(const std::string &hier_parname) {
    GCNF_DUMP_N(name(), "is_used("<<hier_parname.c_str()<<")");
    param_iterator pos;
    pos = m_parameter_database.find(hier_parname);
    if (pos == m_parameter_database.end()) {
      return false;
    }
    return pos->second.is_used;
  }

  /// @see gs::cnf::param_db_if::getParametersVector
  const std::vector<std::string> getParametersVector() {
    GCNF_DUMP_N(name(), "getParameters()");      
    std::vector<std::string> vec;
    for (param_iterator iter = m_parameter_database.begin(); iter!=m_parameter_database.end(); ++iter) {
      vec.push_back(iter->first);
    }
    return vec;
  }

  /// @see gs::cnf::param_db_if::getParameters
  const std::string getParameters() {
    GCNF_DUMP_N(name(), "getParameters()");      

    //vector<string> vec;
    //for (param_iterator iter = m_parameter_database.begin(); iter!=m_parameter_database.end(); ++iter) {
    //  vec.push_back(iter->first);
    //}
    std::stringstream ss_return;
    for (param_iterator iter = m_parameter_database.begin(); iter!=m_parameter_database.end(); ++iter) {
      if (iter !=m_parameter_database.begin()) {
        ss_return << ",";
      }
      ss_return << iter->first;
    }
#ifdef GC_VERBOSE
    std::stringstream ss_all;
    for (param_iterator iter = m_parameter_database.begin(); iter!=m_parameter_database.end(); ++iter) {
      if (iter !=m_parameter_database.begin()) {
        ss_all << ", ";
      }
      if (iter->second.param != NULL) { // is explicit
        ss_all << iter->first << "=" << iter->second.param->getString() << " [explicit]";
      } else {
        ss_all << iter->first << "=" << iter->second.init_value << " [implicit]";
      }
    }
    GCNF_DUMP_N(name(), "parameter list: "<<ss_all.str().c_str());
#endif
    //cout << "ss_return: " << ss_return.str() << endl;
    return ss_return.str();//vec;//ss.str();
  }

  /// Return this name for debug purpose
  const char* name() const { return m_name.c_str(); }

protected: 

  /// Name of this for debug
  std::string m_name;

};

  class gs_param_base;
  /// Typedef for ConfigDatabase_T with the only allowed template specialization
  typedef ConfigDatabase_T<gs_param_base> ConfigDatabase;

} // end namespace cnf
} // end namespace gs

#endif
