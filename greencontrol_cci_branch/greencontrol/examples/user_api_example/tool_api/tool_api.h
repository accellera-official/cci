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

#ifndef __TOOL_API_H__
#define __TOOL_API_H__

#include "greencontrol/config.h"

#define TOOL_API_NAME "Tool_Api"

namespace my_tool_api {
  
using namespace gs::ctr;
using namespace gs::cnf;

/// Simple Tool API for configuration during runtime.
/**
 * This is an simple GreenControl Tool API which allows simple configuration.
 * The API should support configuration during elaboration and runtime. 
 * TODO: TEST elaboration configuration!
 *
 * This API uses directly the gc_port and is a simple example how wo use it:
 * This API has a gc_port whose two control ports (initiator and target) have
 * to be used directly by calling notify or getting notifies through calls 
 * of masterAccess and slaveAccess methods. The later methods are called 
 * through the gc_port_if.
 */
class Tool_Api
: public sc_object,
  public gs::cnf::initialize_if,
  public gs::ctr::gc_port_if
{
  
public:

  SC_HAS_PROCESS(Tool_Api);
	
  /// Constructor
  /**
   * Constructor with default object name "__tool_api__".
   * The gc_port is instantiated with the CONFIG_SERVICE and the type name of
   * this API (and not as plugin).
   */
  Tool_Api()
    : sc_object(sc_gen_unique_name("__tool_api__")),
      m_gc_port(CONFIG_SERVICE, TOOL_API_NAME, false)
  { 
    m_gc_port.api_port(*this);  // bind sc_port of m_gc_port
  }

  // //////////////////// GC_PORT_IF ///////////////////////////////////
  /**
   * Implements gc_port_if.
   * This method starts whenever a master triggers a payload-event.
   */
  void masterAccess(ControlTransactionContainer &t_p)
  {
    ControlTransactionHandle tr = t_p.first;
    ControlPhase ph = t_p.second;

    GC_DUMP_N(name(), "got "<<ph.toString().c_str()<<" atom from master");      
  }
  
  /**
   * Implements gc_port_if.
   * This method starts whenever a slave triggers a payload-event.
   */
  void slaveAccess(ControlTransactionContainer &t_p)
  {  
    ControlTransactionHandle tr = t_p.first;
    ControlPhase ph = t_p.second;

    GC_DUMP_N(name(), "got "<<ph.toString().c_str()<<" atom from slave");      

    switch (ph.state) {
    case ControlPhase::CONTROL_RESPONSE:
      {
        // processed directly in the method which initiated the transaction
        break;
      }
    case ControlPhase::CONTROL_ERROR:
      {
        SC_REPORT_WARNING(name(), "slaveAccess got phase ControlError!");
        break;
      }
    default:
      {
        SC_REPORT_WARNING(name(), "slaveAccess got not processed phase!");
      }
    }
  }

  // ////// TOOL-API methods //////////////////////////////////////////////////

  /**
   * Used to set the parameter <hier_param_name> with the <value>.
   */
  bool setParam(const char* hier_param_name, const char* value) {
    GC_DUMP_N(name(), "setParam("<<hier_param_name<<", "<<value<<")");
    return getPar(hier_param_name)->setString(value);
  }

  /**
   * Used to set the init value of the parameter <hier_param_name> with the <value>.
   */
  bool setInitValue(const char* hier_param_name, const char* value) {
    GC_DUMP_N(name(), "setInitValue("<<hier_param_name<<", "<<value<<")");
    bool success = false;
    
    // create Transaction and send it to config plugin
    ControlTransactionHandle th = m_gc_port.init_port.create_transaction();
    th->set_mService(CONFIG_SERVICE);
    th->set_mCmd(CMD_SET_INIT_VAL);
    // Target will be set in the core (with service information)
    th->set_mSpecifier(hier_param_name); // e.g. "TestIP1.Param1"
    th->set_mValue(value);  // e.g. "TestValue1"
    
    ControlPhase p(ControlPhase::CONTROL_REQUEST);
    ControlTransactionContainer ctc = ControlTransactionContainer(th,p);
    m_gc_port.init_port.out->notify(ctc, PEQ_IMMEDIATE);
    
    if (th->get_mError() == 0) {
      GC_DUMP_N(name(), "setInitValue: ... setting successfull");
      success = true;
    } else {
      GC_DUMP_N(name(), "setInitValue: ... setting failed (error "<<th->get_mError()<<")!");
      success = false;
    }
    
    return success;
  }
  
  /**
   * Used to get the value of the parameter <hier_param_name>.
   */
  const char* getParam(const char* hier_param_name) {
    GC_DUMP_N(name(), "getParam("<<hier_param_name<<")");
    return getPar(hier_param_name)->getString().c_str();
  }

  /**
   * Used to get the parameter <hier_param_name>.
   */
  gs::gs_param_base* getPar(const char* hier_param_name) {
    GC_DUMP_N(name(), "getPar("<<hier_param_name<<")");
    
    // create Transaction an send it to config plugin
    ControlTransactionHandle th = m_gc_port.init_port.create_transaction();
    th->set_mService(CONFIG_SERVICE);
    th->set_mCmd(CMD_GET_PARAM);
    th->set_mSpecifier(hier_param_name); // e.g. "TestIP1.TestParam1"
    
    ControlPhase p(ControlPhase::CONTROL_REQUEST);
    ControlTransactionContainer ctc = ControlTransactionContainer(th,p);
    m_gc_port.init_port.out->notify(ctc, PEQ_IMMEDIATE);
    
    gs::gs_param_base *par = static_cast<gs::gs_param_base*>(th->get_mAnyPointer());
    if (th->get_mError() == 0 && par != NULL) {
    } else {
      GC_DUMP_N(name(), "getParam: ... getting failed (error "<<th->get_mError()<<")!");
    }
    
    return par;
  }
  
  /**
   * Used to get a list of all parameters in the config plugin.
   */
  const std::vector<std::string> getParamList() {
    GC_DUMP_N(name(), "getParamList()");

    // create Transaction an send it to config plugin
    ControlTransactionHandle th = m_gc_port.init_port.create_transaction();
    th->set_mService(CONFIG_SERVICE);
    th->set_mCmd(CMD_GET_PARAM_LIST);
    // Target will be set in the core (with service information)
    th->set_mValue(""); // module
    
    ControlPhase p(ControlPhase::CONTROL_REQUEST);
    GC_DUMP_N(name(), "getParamList: notify init_port");
    ControlTransactionContainer ctc = ControlTransactionContainer(th,p);
    m_gc_port.init_port.out->notify(ctc, PEQ_IMMEDIATE);

    std::string token = th->get_mValue();
    std::vector<std::string> vec;
    if (th->get_mError() != 0) {
      GC_DUMP_N(name(), "getParamList: ... getting list failed (error "<<th->get_mError()<<")!");
      return vec;
    }

    // Convert val to vector
    std::string::size_type idx;
    // test for commas
    while((idx = token.find_first_of(",")) != std::string::npos) {
      if (idx > 0) { // is a word
        vec.push_back(token.substr(0,idx));
      }    
      token = token.substr(idx+1);
    }
    vec.push_back(token.substr(0,idx));

#ifdef GC_VERBOSE
    // Show vector
    vector<string>::iterator iter;
    std::stringstream ss_show;
    iter = vec.begin();
    while( iter != vec.end() ) {
      if (iter != vec.begin()) ss_show << ", ";
      ss_show << *iter;
      iter++;
    }
    GC_DUMP_N(name(), "getParamList: ... got list: '"<<ss_show.str().c_str()<<"'");
#endif

    return vec;
  }

  // ////////////////////////////////////////////////////////////////////////////////
  // / deal the initialize-mode

  /// implements initialize_if 
  void start_initial_configuration() {
  }

  /// implements initialize_if 
  void end_initialize_mode() {
  }
  // ////////////////////////////////////////////////////////////////////////////////

private:
  /// gc_port to send transaction containers
  gc_port m_gc_port;

#ifdef GC_VERBOSE
  /**
   * Helpfunction for GC_DUMP_Ns
   */
  const char * name() {
    std::stringstream ss;
    ss << this->get_parent()->name() << SC_NAME_DELIMITER << "__Tool_Api__";
    return ss.str().c_str();// "TODO Tool_Api";//(dynamic_cast<sc_core::sc_object*>(this))->name();
  }
#endif

};

  
} // and namespace my_tool_api

#endif
