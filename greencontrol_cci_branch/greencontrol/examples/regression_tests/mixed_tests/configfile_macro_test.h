//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
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

#include <systemc>
#include "greencontrol/config.h"

/// Test module owning some parameters being set with config file macros
SC_MODULE(ConfigFileMacroMod)
{
  gs::gs_param<int> param1;
  gs::gs_param<int> param2;
  gs::gs_param<int> param3;
  gs::gs_param<int> param4;
  
  SC_HAS_PROCESS(ConfigFileMacroMod);
  ConfigFileMacroMod(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  , param1("param1")
  , param2("param2")
  , param3("param3")
  , param4("param4")
  {
    std::cout << std::endl << "ConfigFileMacroMod has been configured with these params:" << std::endl;
    std::cout << "   param1 = " << param1 << std::endl;    
    std::cout << "   param2 = " << param2 << std::endl;    
    std::cout << "   param3 = " << param3 << std::endl;    
    std::cout << "   param4 = " << param4 << std::endl;    
  }
  
};
