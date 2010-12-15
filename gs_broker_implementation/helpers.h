// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//    Mark Burton, mark@greensocs.com
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#ifndef __GS_CCI_HELPERS_H__
#define __GS_CCI_HELPERS_H__


#include <cci.h>
#include "greencontrol/config.h"

namespace cci {
  namespace cnf {

    std::vector<std::string> vector_factory(const char* pub_par ...);

  }
}

#endif
