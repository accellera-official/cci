// LICENSETEXT
//
//   Copyright (C) 2009-2010 : GreenSocs Ltd
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


#ifndef __OBSERVERMODULE_H__
#define __OBSERVERMODULE_H__

#include <systemc>

#include "ex_globals.h"
#include "cci.h"


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

  /// Callback function with default signature showing changes.
  cci::cnf::callback_return_type config_callback(cci::cnf::cci_base_param& par, const cci::cnf::callback_type& cb_reason);

  /// Callback function with default signature announcing new parameters.
  cci::cnf::callback_return_type config_new_param_callback(cci::cnf::cci_base_param& par, const cci::cnf::callback_type& cb_reason);

protected:
  /// Pointer the the module's configuration API
  cci::cnf::cci_cnf_broker* mApi;
  
  /// Vector of callbacks to keep them outside the local scope of main_action
  std::vector< cci::shared_ptr<cci::cnf::callb_adapt_b> > mCallbacks;
  
};


#endif
