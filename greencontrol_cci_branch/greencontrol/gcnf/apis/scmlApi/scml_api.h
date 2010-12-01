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

// doxygen comments

#ifndef __SCML_API_H__
#define __SCML_API_H__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <map>

#include "boost/shared_ptr.hpp" 

#include "greencontrol/config.h"
#include "scml_property.h"

#define SCML_API_NAME "Scml_Api"


namespace gs {
namespace cnf {


using boost::shared_ptr;


/// User API adaptor which connects the scml_property objects to the GreenControl framework.
/**
 * This is an API which translates between the CoWare SCML API and the
 * GreenConfig API
 *
 * This API uses the GCnf_Api to communicate with the 
 * GreenConfig Core. 
 */
class Scml_Api
: public sc_object,
  public initialize_if
{

  // Typedef for static map containing pointers to the Scml_Apis
  //        sc_core::sc_module-pointer, Scml_Api-instance-smart-pointer
  typedef std::map<sc_module*, shared_ptr<Scml_Api> >  apiMap;
  
public:

  SC_HAS_PROCESS(Scml_Api);

  /// Get the matching Scml_Api instance.
  /**
   * Method to get the right instance of the Scml_Api.
   * One instance per module is instantiated.
   *
   * @param mod  User module pointer which identifies the needed API instance.
   * @return     Shared pointer to the API instance.
   */ 
  static shared_ptr<Scml_Api> getApiInstance(sc_core::sc_module *mod) 
  {
    /// static map sc_core::sc_module-pointer -> Scml_Api-instance-pointer
    static apiMap* mInstanceMap = NULL;
    if (!mInstanceMap) {
      mInstanceMap = new apiMap();
      GCNF_DUMP_N("static Scml_Api::getApiInstance", "create new apiMap");
    } // TODO: delete this instance!?

    shared_ptr<Scml_Api> scmlapi;
    // If for the overgiven module <mod> an API instance exists, return it
    apiMap::iterator pos;
    pos = mInstanceMap->find(mod);
    if (pos != mInstanceMap->end()) {
      return pos->second;
    }
    // if it does not yet exist, create one
    else {
      shared_ptr<Scml_Api> newscmlapi_sp(new Scml_Api());
      mInstanceMap->insert(std::pair<sc_module*, shared_ptr<Scml_Api> >(mod, newscmlapi_sp));
      //mInstanceMap.insert( make_pair(mod, scmlapi) );      
      return newscmlapi_sp;
    }
  }

  /// Destructor
  ~Scml_Api() {
    GCNF_DUMP_N(name(), "destructor "<<(unsigned int)this);
  }

private:

  /// Constructor
  /**
   * Must not be used directly. Use getApiInstance instead.
   */
  Scml_Api()
    : sc_object(sc_gen_unique_name("__scml_api__"))
      //,start_of_sim(false)
  { 
    GCNF_DUMP_N("Scml_Api", "Scml_Api instance generated.");
    m_gcnf_api = gs::cnf::GCnf_Api::getApiInstance(gs::cnf::get_parent_sc_module(this));
  }
  
public:

  /// Add a parameter without initial value to the ConfigPlugin.
  /**
   * Adds a parameter of any kind to the configuration framework.
   *
   * @param parname  Hierarchical parameter name.
   * @return         Success of the adding.
   */
  bool addParam(const std::string &parname) {
    GCNF_DUMP_N(name(), "addParam "<<parname.c_str());
    return addParam(parname, "");
  }

  /// Add a parameter with initial value to the ConfigPlugin.
  /**
   * Adds a parameter of any kind to the configuration framework.
   *
   * @param parname   Hierarchical parameter name.
   * @param init_val  Initial value the explicit value is set to.
   * @return          Success of the adding.
   */
  bool addParam(const std::string &parname, const std::string &init_val) {
    GCNF_DUMP_N(name(), "addParam("<<parname.c_str()<<", "<<init_val.c_str()<<")");
    return m_gcnf_api->addParam(parname, init_val);
  }

  /// Set a parameter's value. 
  /**
   * Set a string value.
   *
   * @param parname  Hierarchical parameter name.
   * @param value    Value the parameter has to be set to. 
   * @return         Success of the setting.
   */
  bool setParam(const std::string &parname, const std::string &value) {
    GCNF_DUMP_N(name(), "setParam string value:");
    return m_gcnf_api->getPar(parname)->setString(value);
  }
 
  /// Set a parameter's value. 
  /**
   * Set an int value.
   *
   * @param parname  Hierarchical parameter name.
   * @param value    Value the parameter has to be set to. 
   * @return         Success of the setting.
   */
  bool setParam(const std::string &parname, const int value) {
    GCNF_DUMP_N(name(), "setParam int value:");
    std::ostringstream ss;
    ss << value;
    return setParam(parname, ss.str());
  }

  /// Set a parameter's value. 
  /**
   * Set an unsigned int value.
   *
   * @param parname  Hierarchical parameter name.
   * @param value    Value the parameter has to be set to. 
   * @return         Success of the setting.
   */
  bool setParam(const std::string &parname, const unsigned int value) {
    GCNF_DUMP_N(name(), "setParam unsigned int value:");
    std::ostringstream ss;
    ss << value;
    return setParam(parname, ss.str());
  }

  /// Set a parameter's value. 
  /**
   * Set a bool value.
   *
   * @param parname  Hierarchical parameter name.
   * @param value    Value the parameter has to be set to. 
   * @return         Success of the setting.
   */
  bool setParam(const std::string &parname, const bool value) {
    GCNF_DUMP_N(name(), "setParam bool value:");
    std::string s;
    if (value) {
      s = "1";
    } else {
      s = "0";
    }
    return setParam(parname, (std::string) s.c_str());
  }

  /// Set a parameter's value. 
  /**
   * Set a double value.
   *
   * @param parname  Hierarchical parameter name.
   * @param value    Value the parameter has to be set to. 
   * @return         Success of the setting.
   */
  bool setParam(const std::string &parname, const double value) {
    GCNF_DUMP_N(name(), "setParam double value:");
    std::ostringstream ss;
    ss << value;
    return setParam(parname, ss.str());
  }

  /// Get the string value of a string parameter.
  /**
   * @param parname  Hierarchical parameter name.
   * @return         Value of the parameter.
   */
  const std::string getStringParam(const std::string &parname) {
    return m_gcnf_api->getPar(parname)->getString();
  }

  /// Get the int value of a string parameter.
  /**
   * @param parname  Hierarchical parameter name.
   * @return         Value of the parameter.
   */
  int getIntParam(const std::string &parname) {
    GCNF_DUMP_N(name(), "getIntParam "<<parname.c_str()<<":");
    const std::string tmpstring = getStringParam(parname);
    if (tmpstring.length() == 0) return 0;
    if (tmpstring.find("0x", 0) != string::npos || tmpstring.find("0X", 0) != string::npos)
      SC_REPORT_ERROR(name(), "getIntParam: hexadecimal notation is not allowed for int.");
    int ret;
    if(EOF == sscanf(tmpstring.c_str(), "%i", &ret)) {
      GCNF_DUMP_N(name(), "getIntParam: value '"<<tmpstring.c_str()<<"' is no int.");
      SC_REPORT_ERROR(name(), "getIntParam: value is no int.");
    }
    return ret;
  }

  /// Get the unsigned int value of a string parameter.
  /**
   * @param parname  Hierarchical parameter name.
   * @return         Value of the parameter.
   */
  unsigned int getUIntParam(const std::string &parname) {
    GCNF_DUMP_N(name(), "getUIntParam "<<parname.c_str()<<":");
    const std::string tmpstring = getStringParam(parname);
    if (tmpstring.length() == 0) return 0;
    if (tmpstring.find("0x", 0) != string::npos || tmpstring.find("0X", 0) != string::npos)
      SC_REPORT_ERROR(name(), "getUIntParam: hexadecimal notation is not allowed for unsigned int.");
    unsigned int ret;
    if(EOF == sscanf(tmpstring.c_str(), "%u", &ret)) {
      SC_REPORT_ERROR(name(), "getUIntParam: value is no unsigned int.");
    }
    return ret;
  }

  /// Get the bool value of a string parameter.
  /**
   * @param parname  Hierarchical parameter name.
   * @return         Value of the parameter.
   */
  bool getBoolParam(const std::string &parname) {
    GCNF_DUMP_N(name(), "getBoolParam "<<parname.c_str()<<":");
    const std::string tmpstring = getStringParam(parname);
    if (tmpstring.length() == 0) return false;
    int ret_int;
    if(EOF == sscanf(tmpstring.c_str(), "%i", &ret_int)) { // TODO
      SC_REPORT_ERROR(name(), "getBoolParam: value is no bool.");
    }
    bool ret;
    if (ret_int > 0){
      ret = true;
    } else {
      ret = false;
    }
    return ret;
  }

  /// Get the double value of a string parameter.
  /**
   * @param parname  Hierarchical parameter name.
   * @return         Value of the parameter.
   */
  double getDoubleParam(const std::string &parname) {
    GCNF_DUMP_N(name(), "getDoubleParam "<<parname.c_str()<<":");
    const std::string tmpstring = getStringParam(parname);
    if (tmpstring.length() == 0) return 0;
    double ret;
    if(EOF == sscanf(tmpstring.c_str(), "%lf", &ret)) {
      SC_REPORT_ERROR(name(), "getDoubleParam: value is no double.");
    }
    return ret;
  }


  /// If a parameter exists in the ConfigPlugin
  /**
   * @param parname  Hierarchical parameter name.
   * @return         If the parameter exists.
   */
  bool existsParam(const std::string &parname) {
    return m_gcnf_api->existsParam(parname);
  }

  /// Returns a list of all parameters in the ConfigPlugin
  /**
   * @return    Vector of all parameters.
   */
  const std::vector<std::string> getParamList() {
    return m_gcnf_api->getParamList();
  }

  // /////////////////////////   deal the initialize-mode   ////////////////////////////

  /// Implement initialize_if, deal the initialize-mode.
  void start_initialize_mode() {
    GCNF_DUMP_N(name(), "start_initialize_mode called in Scml_Api.");
    //start_of_sim = true;
  }
  
  /// Implement initialize_if, deal the initialize-mode.
  void end_initialize_mode() {
  }

  // ////////////////////////////////////////////////////////////////////////////////

private:

  /// Disabled assignment operator.
  Scml_Api& operator= (const Scml_Api&);

  /// Disabled copy constructor.
  Scml_Api(const Scml_Api&);

  /// Marks whether setting and getting returns not longer false
  // Doesn't care about initialize-mode.
  //bool start_of_sim;         // true from the point of start_of_simulation called on

  /// GCnf_Api which is used by this API.
  cnf_api_if *m_gcnf_api;
  
};


} // end namespace cnf
} // end namespace gs

#endif
