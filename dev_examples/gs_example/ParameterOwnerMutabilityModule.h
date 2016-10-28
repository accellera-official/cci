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
 * @file   ParameterOwnerMutabilityModule.h
 * @author Christian Schroeder, GreenSocs
 */

#ifndef __PARAMETEROWNERMUTABILITYMODULE_H__
#define __PARAMETEROWNERMUTABILITYMODULE_H__

#include <systemc>
#include "ex_globals.h"
#include "cci_configuration"

/// Module which owns some cci parameters of different mutybility.
class ParameterOwnerMutabilityModule
: public sc_core::sc_module
{
  
public:
  
  SC_HAS_PROCESS(ParameterOwnerMutabilityModule);
	
  /// Constructor
  ParameterOwnerMutabilityModule(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  , mutable_int_param   ("mutable_int_param", 50 )
  , immutable_int_param ("immutable_int_param", 51 )
  , elab_time_int_param ("elab_time_int_param", 52 )
  { 
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();
  
  /// Example mutable parameter.
  cci::cci_param<int, cci::CCI_MUTABLE_PARAM>    mutable_int_param;
  /// Example immutable parameter.
  cci::cci_param<int, cci::CCI_IMMUTABLE_PARAM>  immutable_int_param;
  /// Example elaboration_time_parameter parameter.
  cci::cci_param<int, cci::CCI_ELABORATION_TIME_PARAM>  elab_time_int_param;
  
};


#endif
