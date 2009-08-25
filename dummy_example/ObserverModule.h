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


#ifndef __OBSERVERMODULE_H__
#define __OBSERVERMODULE_H__

#include <systemc>

#include "ex_globals.h"
#include "cci.h"
#include "../dummy_implementation/dummy_cci.h"


/// Module which registers for parameter changes
class ObserverModule
: public sc_core::sc_module
{
public:

  SC_HAS_PROCESS(ObserverModule);
  ObserverModule(sc_core::sc_module_name name);
  
  ~ObserverModule();
  
  /// Main action to make tests with parameters.
  void main_action();

  /// Callback function with default signature.
  void config_callback(cci::cci_base_param& par, const cci::callback_type& cb_reason);

  /// Callback function with default signature.
  void config_new_param_callback(cci::cci_base_param& par, const cci::callback_type& cb_reason);

  // Optional callback function with default signature.
  //void config_callback_deprecated(const std::string parname, const std::string val);

protected:
  /// Pointer the the module's configuration API
  cci::cci_cnf_api* mApi;
  
  /// Vector of callbacks to ensure they can be unregistered when module is destructed
  std::vector< boost::shared_ptr<cci::callb_adapt_b> > mCallbacks;
  
};


#endif
