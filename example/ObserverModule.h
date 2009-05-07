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

// doxygen comments

#ifndef __ObserverModule_H__
#define __ObserverModule_H__

#include <systemc>

#include "ex_globals.h"

#include "greencontrol/gcnf/apis/GCnf_Api/GCnf_Api.h"
#include "greencontrol/gcnf/apis/gs_param/gs_param.h"


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

