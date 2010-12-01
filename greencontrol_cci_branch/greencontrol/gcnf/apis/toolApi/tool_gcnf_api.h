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

#ifndef __TOOL_GCNF_API_H__
#define __TOOL_GCNF_API_H__

#include "greencontrol/config.h"


namespace gs {
namespace cnf {


/// Simple Tool API for initial and runtime configuration
/**
 * This is an simple GreenControl Tool API which allows simple configuration.
 *
 * This API uses the GCnf_Api and may be used during initialize-mode.
 */
class Tool_GCnf_Api
: public sc_object,
  public initialize_if
{
  
public:

  SC_HAS_PROCESS(Tool_GCnf_Api);
	
  /// Constructor
  /**
   * Constructor with default object name "__tool_gcnf_api__"
   */
  Tool_GCnf_Api()
    : sc_object(sc_gen_unique_name("__tool_gcnf_api__"))
  { 
    GCNF_DUMP_N(name(), "Tool_GCnf_Api instance generated.");
    m_gcnf_api = gs::cnf::GCnf_Api::getApiInstance(gs::cnf::get_parent_sc_module(this));
  }
  
  /// Destructor
  ~Tool_GCnf_Api() {
  }
  


  // ////// TOOL-API methods //////////////////////////////////////////////////

  /// Add a parameter with initial value. 
  /**
   * BEWARE: ADD must not be uses for foreign parameters, only for own ones
   *
   * @see gs::cnf::GCnf_Api::addParam()
   */
   bool addParam(const char* hier_param_name, const char* value) {
    GCNF_DUMP_N(name(), "addParam("<<hier_param_name<<", "<<value<<")");
    SC_REPORT_WARNING(name(), "BEWARE: ADD must not be used for foreign parameters, only for own one's");
    return m_gcnf_api->addParam(hier_param_name, value);
  }

  /// Set a parameter's value. 
  /**
   * @see gs::cnf::GCnf_Api::setParam()
   */
   bool setParam(const char* hier_param_name, const char* value) {
    GCNF_DUMP_N(name(), "setParam("<<hier_param_name<<", "<<value<<")");
    return m_gcnf_api->setParam(hier_param_name, value);
  }

  /// Set a parameter's value. 
  /**
   * @see gs::cnf::GCnf_Api::setInitValue()
   */
  bool setInitValue(const char* hier_param_name, const char* value) {
    GCNF_DUMP_N(name(), "setInitValue("<<hier_param_name<<", "<<value<<")");
    return m_gcnf_api->setInitValue(hier_param_name, value);
  }
  
  /// Get a parameter's value.
  /**
   * @see gs::cnf::GCnf_Api::getParam()
   */
  const char* getParam(const char* hier_param_name) {
    GCNF_DUMP_N(name(), "getParam("<<hier_param_name<<")");
    return m_gcnf_api->getParam(hier_param_name).c_str();
  }

  /// Get a parameter.
  /**
   * @see gs::cnf::GCnf_Api::getPar()
   */
  gs::gs_param_base* getPar(const char* hier_param_name) {
    GCNF_DUMP_N(name(), "getPar("<<hier_param_name<<")");
    return m_gcnf_api->getPar(hier_param_name);
  }
  
  /// Return a list of all parameters existing in the ConfigPlugin.
  /**
   * @see gs::cnf::GCnf_Api::getParamList()
   */
  const std::vector<std::string> getParamList() {
    GCNF_DUMP_N(name(), "getParamList()");
    return m_gcnf_api->getParamList();
  }

  /// Return a list of all parameters of the overgiven module.
  /**
   * @see gs::cnf::GCnf_Api::getParamList(const std::string&)
   */
  const std::vector<std::string> getParamList(const std::string module_name) {
    GCNF_DUMP_N(name(), "getParamList(module="<<module_name.c_str()<<")");
    return m_gcnf_api->getParamList(module_name);
  }

  /// Returns an event pointer which is notified when the parameter value changes
  /**
   * @see gs::cnf::GCnf_Api::getUpdateEvent()
   */
  const sc_event& getUpdateEvent(const std::string &parname)  throw (RegisterObserverFailedException) {
    GCNF_DUMP_N(name(), "getUpdateEvent("<<parname.c_str()<<")");      
    return m_gcnf_api->getUpdateEvent(parname);
  }

  /// Registers an observer callback function using a CallbAdapt object.
  /**
   * @see gs::cnf::GCnf_Api::registerCallback()
   */
  bool registerCallback(const std::string &parname, CallbAdapt_b *callback) {
    GCNF_DUMP_N(name(), "registerCallback("<<parname.c_str()<<", callback_obj)");
    return m_gcnf_api->registerCallback(parname, callback);
  }

  // //////////////   deal the initialize-mode   ////////////////////////////////////

  /// implements initialize_if 
  void start_initialize_mode() {
  }

  /// implements initialize_if
  void end_initialize_mode() {
  }

  // ////////////////////////////////////////////////////////////////////////////////

private:

  /// GCnf_Api which is used by this API
  gs::cnf::cnf_api_if* m_gcnf_api;

};


} // end namespace cnf
} // end namespace gs

#endif
