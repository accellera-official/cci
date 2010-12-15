// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
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


#ifndef __GS_CCI_BROKER_H__
#define __GS_CCI_BROKER_H__

// NOTE: The broker implementation must not include any parameter vendor dependent headers!

//#define CCI_CNF_IMPL_VERBOSE // to enable debug output for implementation
#ifdef CCI_CNF_IMPL_VERBOSE
# define CCI_CNF_IMPL_DUMP(msg) { std::cout<<msg<<std::endl; } 
#else
# define CCI_CNF_IMPL_DUMP(msg) {  } 
#endif

#include <cci.h>
#include "gs_cci_cnf_broker.h"
#include "gs_cci_cnf_broker_accessor.h"
#include "gs_cci_cnf_private_broker.h"

namespace cci {

  
} // end namespace cci

#endif
