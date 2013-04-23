// LICENSETEXT
//
//   Copyright (C) 2011 : GreenSocs Ltd
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


#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include <systemc>

#include "ex_globals.h"
#include "cci"


/// Module which registers for parameter changes
class Observer
{
public:

  Observer(const char* name);
  
  ~Observer();
  
  /// Callback function with string signature showing changes for implicit and explicit parameters.
  cci::cnf::callback_return_type config_str_callback(const std::string& par_name, const cci::cnf::callback_type& cb_reason);

  /// Callback function with default signature announcing new parameters.
  cci::cnf::callback_return_type config_new_param_callback(const std::string& par_name, const cci::cnf::callback_type& cb_reason);

  /// Callback function with default signature showing changes.
  cci::cnf::callback_return_type config_callback(cci::cnf::cci_base_param& par, const cci::cnf::callback_type& cb_reason);

  /// Debug output
  const char* name() const;
  
protected:
  
  /// Pointer the the module's configuration broker
  cci::cnf::cci_cnf_broker_if* mBroker;
  
  /// Vector of callbacks to keep them outside the local scope of main_action
  std::vector< cci::shared_ptr<cci::cnf::callb_adapt> > mCallbacks;

  /// Name for debug info
  std::string mName;
  
};


#endif
