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

#ifndef __AVscvAnalyserTool_h__
#define __AVscvAnalyserTool_h__

#include "simple_analysis_globals.h"

#include "greencontrol/config.h"
#include "greencontrol/gav/apis/gav_api/GAV_Api.h"


/// Module which uses the scv OutputPlugin
class AVscvAnalyserTool
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(AVscvAnalyserTool);
	
  /// Constructor
  AVscvAnalyserTool(sc_core::sc_module_name name)
    : sc_core::sc_module(name)
  { 
    m_configAPI = gs::cnf::GCnf_Api::getApiInstance(this);
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();

protected:
  /// Configuration API
  gs::cnf::cnf_api_if *m_configAPI;
  
  /// Analysis API
  gs::av::GAV_Api  m_analysisAPI;
 
};


#endif

