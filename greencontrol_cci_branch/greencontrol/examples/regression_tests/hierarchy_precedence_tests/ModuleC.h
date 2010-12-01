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
#include "ModuleD.h"

/// Module setting init values and instantiating submodules
class ModuleC
: public sc_core::sc_module
{
  
  /// GreenConfig Callback enabler
  GC_HAS_CALLBACKS();
  
public:
  
  SC_HAS_PROCESS(ModuleC);
  /// Constructor
  ModuleC(sc_core::sc_module_name name);
  
  /// Destructor
  ~ModuleC();
  
  /// Main SystemC-Thread
  void main_action();
  
protected:
  
  /// Pointer to my config API
  gs::cnf::cnf_api_if *m_cnf_api;
  
  /// Subsystem to be configured
  ModuleD *m_submoduleD1;
  ModuleD *m_submoduleD2;
  
};
