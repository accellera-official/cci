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

// GreenConfig
#include "greencontrol/config.h" 

class OwningModule
: public sc_core::sc_module
{
public:
  OwningModule(sc_core::sc_module_name name) 
  : sc_core::sc_module(name) 
  , int_param1("int_param1") 
  , int_param2("int_param2") 
  , int_param3("int_param3") 
  {
  }
  
  gs::gs_param<int> int_param1;
  gs::gs_param<int> int_param2;
  gs::gs_param<int> int_param3;
  
};
