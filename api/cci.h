/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2009 by all Contributors.
  All Rights reserved.

  Developed by GreenSocs : http://www.greensocs.com/
   Christian Schroeder, schroeder@eis.cs.tu-bs.de
   Mark Burton, mark@greensocs.com

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/


#ifndef __CCI_H__
#define __CCI_H__


#define OSCI_CCI_SC_REPORT_IDENT "/OSCI/CCI"

#include <systemc>

namespace cci {

  
  // forward declaration
  class cci_cnf_api;
  
  // forward declaration, to be implemented by tool
  cci_cnf_api* get_cnf_api_instance(sc_core::sc_module* = NULL);
  
  
} // end namespace cci


#include "cci_datatypes.h"
#include "cci_callbacks.h"
#include "cci_base_param.h"
#include "cci_param.h"
#include "cci_cnf_api.h"


#endif
