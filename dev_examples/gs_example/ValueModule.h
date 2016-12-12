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
 * @file   ValueModule.h
 * @author Christian Schroeder, GreenSocs
 * @author Mark Burton, GreenSocs
 */

#ifndef __VALUEMODULE_H__
#define __VALUEMODULE_H__

#include <systemc>
#include "ex_globals.h"
#include "cci_configuration"

/// Module which sets/accesses parameter values using cci_values
class ValueModule
: public sc_core::sc_module
{
public:

  SC_HAS_PROCESS(ValueModule);
  ValueModule(sc_core::sc_module_name name);
  
  /// Main action to make tests with parameters.
  void main_action();

protected:
  /// Pointer the the module's configuration broker
  cci::cci_broker_handle mBroker;

};


#endif
