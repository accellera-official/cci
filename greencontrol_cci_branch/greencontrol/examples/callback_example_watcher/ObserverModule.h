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

#ifndef __ObserverModule_H__
#define __ObserverModule_H__

#include <systemc>

#include "callb_globals.h"

#include "greencontrol/config.h"


/// Module which registers for parameter changes
class ObserverModule
: public sc_core::sc_module
{
  
private:
  sc_core::sc_event *m_new_param_event;
  sc_core::sc_event *m_param_change_event;
  
public:

  GC_HAS_CALLBACKS();
  
  SC_HAS_PROCESS(ObserverModule);
	
  /// Constructor
  ObserverModule(sc_core::sc_module_name name)
    : sc_core::sc_module(name)
  { 
    m_Api = gs::cnf::GCnf_Api::getApiInstance(this);
    SC_THREAD(main_action);
    
    m_new_param_event = &m_Api->getNewParamEvent();
    SC_METHOD(event_reactor_new_param);
    sensitive << *m_new_param_event;
    dont_initialize();

    m_param_change_event = &m_Api->getPar("Owner.int_param")->getUpdateEvent();
    SC_METHOD(event_reactor_param_change);
    sensitive << *m_param_change_event;
    dont_initialize();
  }
  
  ~ObserverModule() {
    DEMO_TRACE(name(), "~~~ Destructor ObserverModule ~~~");
    GC_UNREGISTER_CALLBACKS();
  }
  
  /// Main action to make tests with parameters.
  void main_action();

  /// SC_METHOD which reacts on parameter events
  void event_reactor_new_param();
  /// SC_METHOD for parameter update event tests
  void event_reactor_param_change();
  
  // Callback function with default signature.
  void config_new_param_callback(const std::string parname, const std::string val);
  
  // Callback function with default signature.
  void config_callback(gs::gs_param_base& par);

  // Deprecated callback function with default signature.
  void config_callback_deprecated(const std::string parname, const std::string val);


protected:
  gs::cnf::cnf_api *m_Api;
  
};


#endif

