/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2009-2010 GreenSocs
  All rights reserved.
  Authors:
      Christian Schroeder <schroeder@eis.cs.tu-bs.de>
      Mark Burton <mark@greensocs.com>

  Copyright 2016 Ericsson
  All rights reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 *****************************************************************************/


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

  /// Callback function with default signature announcing new parameters.
  cci::callback_return_type config_new_param_callback(const std::string& par_name, const cci::callback_type& cb_reason);

  /// Callback function with default signature rejecting all changes.
  cci::callback_return_type config_callback_reject_changes(cci::cci_base_param& par, const cci::callback_type& cb_reason);

protected:
  /// Pointer the the module's configuration broker
  cci::cci_broker_if* mBroker;
  
  /// Vector of callbacks to keep them outside the local scope of main_action
  std::vector< cci::shared_ptr<cci::callb_adapt> > mCallbacks;
  
};


#endif
