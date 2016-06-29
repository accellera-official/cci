/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2011 GreenSocs
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


#ifndef __MODULEC_H__
#define __MODULEC_H__

#include <systemc>
#include "ex_globals.h"
#include "cci_configuration"

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "gs_cci_cnf_private_broker_accessor.h"


/// Module which owns some very private cci parameters.
class ModuleC
: public sc_core::sc_module
{
protected:
  /// for secure access by parameters
  cci::cci_broker_if* m_broker;
  
public:
  
  SC_HAS_PROCESS(ModuleC);
	
  /// Constructor
  ModuleC(sc_core::sc_module_name name, cci::cci_broker_manager priv_broker);
  
  /// Destructor
  ~ModuleC();
  
  /// Main action to make tests with parameters.
  void main_action();
  
  /// Example parameter.
  cci::cci_param<std::string>    priv_param;
  
};


#endif
