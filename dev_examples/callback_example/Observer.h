/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 ****************************************************************************/

/**
 * @file   Observer.h
 * @author Christian Schroeder, GreenSocs
 * @author Mark Burton, GreenSocs
 */

#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include <systemc>

#include "ex_globals.h"
#include "cci_configuration"

/// Module which registers for parameter changes
class Observer
{
public:

  Observer(const char* name);
  
  ~Observer();
  
  /// Callback function with string signature showing changes for implicit and explicit parameters.
  cci::callback_return_type config_str_callback(const std::string& par_name, const cci::callback_type& cb_reason);

  /// Callback function with default signature announcing new parameters.
  cci::callback_return_type config_new_param_callback(const std::string& par_name, const cci::callback_type& cb_reason);

  /// Callback function with default signature showing changes.
  // TODO: fixme
  //cci::callback_return_type config_callback(cci::cci_base_param& par, const cci::callback_type& cb_reason);

  /// Debug output
  const char* name() const;
  
protected:
  
  /// Pointer the the module's configuration broker
  cci::cci_broker_if* mBroker;
  
  /// Vector of callbacks to keep them outside the local scope of main_action
  std::vector< cci::shared_ptr<cci::callb_adapt> > mCallbacks;

  /// Name for debug info
  std::string mName;
  
};


#endif
