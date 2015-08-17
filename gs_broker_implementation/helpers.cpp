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

#include "helpers.h"

/*std::vector<std::string> cci::vector_factory(const char* pub_par ...) {
  std::vector<std::string> pub_param_lst;
  va_list list;
  va_start(list, pub_par);
  const char* p = pub_par;
  for(;;) { 
    if(std::string(p) == std::string(END_OF_PUBLIC_PARAM_LIST))
      break;
    pub_param_lst.push_back(p);
    p = va_arg(list, char*);
  }
  va_end(list);
  
  return pub_param_lst;
}*/
