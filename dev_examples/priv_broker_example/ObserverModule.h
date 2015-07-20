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
#include "cci_configuration"


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
  cci::callback_return_type config_callback(cci::cci_base_param& par, const cci::callback_type& cb_reason);

  /// Callback function with default signature rejecting all changes.
  cci::callback_return_type config_callback_reject_changes(cci::cci_base_param& par, const cci::callback_type& cb_reason);

protected:
  /// Pointer the the module's configuration broker
  cci::cci_broker_if* mBroker;
  
  /// Vector of callbacks to keep them outside the local scope of main_action
  std::vector< cci::shared_ptr<cci::callb_adapt> > mCallbacks;
  
};


#endif
