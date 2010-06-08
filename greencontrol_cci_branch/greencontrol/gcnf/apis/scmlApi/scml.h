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



// This is the GreenSocs file which is included _instead_ of the CoWare scml.h file 
// in order to provide the scml_property class.


#ifndef SCML_H
#define SCML_H

#include "greencontrol/gcnf/apis/scmlApi/scml_property.h"
#ifdef ENABLE_SCML_NAMESPACE
using gs::cnf::scml_property;
#endif

#endif
