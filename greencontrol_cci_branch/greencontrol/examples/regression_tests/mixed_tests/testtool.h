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

#ifndef __TESTTOOL_H__
#define __TESTTOOL_H__

#include <systemc>
#include <sstream>

#define GC_VERBOSE

#include "greencontrol/gcnf/apis/toolApi/tool_gcnf_api.h"


/// Test tool
/**
 * Test tool which uses the Tool_GCnf_Api
 */
class TestTool
: public sc_core::sc_module
{
  
public:

  GC_HAS_CALLBACKS();
  
  SC_HAS_PROCESS(TestTool);
	
  /// Constructor
  TestTool(sc_core::sc_module_name name)
    : sc_core::sc_module(name)
  { 
    m_observer_event = new sc_core::sc_event();

    SC_THREAD(main_action);
    SC_THREAD(event_observer);
    
    std::string val = toolApi.getPar("IP1.scmlIntParam")->getString();
    std::cout << "IP1.scmlIntParam=" << val << std::endl;

    test_action_callback();
  }
  
  ~TestTool() {
    GC_UNREGISTER_CALLBACKS();
  }
  
  /// SC_THREAD for main action.
  void main_action();

  /// SC_THREAD which is called when the observer-event is notified.
  /**
   * If the event pointer m_observer_event is changed to a new event
   * the changing method has to notify the event m_observer_update_event.
   *
   * Attention: The wait in this thread is invokes at two different event
   *            notifies were only one of them indicates a changed parameter.
   *            This is for test!
   */
  void event_observer();

  /// Tests callbacks.
  void test_action_callback();

  /// Callback function implements gc_callback_if.
  /**
   */
  void config_callback(gs::gs_param_base& par);

  /// Callback function 2 implements gc_callback_if.
  /**
   */
  void config_callback2(gs::gs_param_base& par);

  void boost_config_callback(const std::string parname, const std::string value);

private:

  /// Tool API which is used by this test tool
  gs::cnf::Tool_GCnf_Api toolApi;

  /// Pointer to an observer event to test.
  const sc_core::sc_event *m_observer_event;

  // Event which announces an updated m_observer_event
  sc_core::sc_event m_observer_update_event;
};


#endif
