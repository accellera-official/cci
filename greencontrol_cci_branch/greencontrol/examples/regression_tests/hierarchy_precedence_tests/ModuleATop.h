//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#include "greencontrol/config.h"  // GreenConfig
#include "ModuleBSubsystem.h"

/// Top-Level-Module setting init values and instantiating the subsystem B
class ModuleATop
: public sc_core::sc_module
{
  
  /// GreenConfig Callback enabler
  GC_HAS_CALLBACKS();
  
public:
  
  SC_HAS_PROCESS(ModuleATop);
  /// Constructor
  ModuleATop(sc_core::sc_module_name name);
  
  /// Destructor
  ~ModuleATop();
  
  /// Main SystemC-Thread
  void main_action();
  
protected:
  
  /// Pointer to my config API
  gs::cnf::cnf_api_if *m_cnf_api;
  
  /// Subsystem to be configured
  ModuleBSubsystem *m_subsystem;
  
};
