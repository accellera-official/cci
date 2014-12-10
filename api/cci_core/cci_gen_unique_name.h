// LICENSETEXT
//
//   Copyright (C) 2011 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#ifndef CCI_CORE_CCI_GEN_UNIQUE_NAME_H_INCLUDED_
#define CCI_CORE_CCI_GEN_UNIQUE_NAME_H_INCLUDED_

#include <iostream>
#include <map>

namespace cci {
  
  /// Generate a cci-wide unique name. 
  /** 
   * Keeps the first name as it is, later adds _number, beginning with _0
   * @TODO more elegant
   */
  const char* cci_gen_unique_name(const char* name);

} // end namespace cci

// TODO: add to proper core library
#include "cci_gen_unique_name.cpp"

#endif
