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

#ifndef CCI_CCI_SHARED_PTR_H_INCLUDED_
#define CCI_CCI_SHARED_PTR_H_INCLUDED_

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace cci {
  
  using boost::shared_ptr;

  using boost::make_shared;
  
} // end namespace cci

#endif
