/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2009-2010 GreenSocs
  All rights reserved.
  Authors:
      Christian Schroeder <schroeder@eis.cs.tu-bs.de>

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


#ifndef __OWNERMODULEB_H__
#define __OWNERMODULEB_H__


#include <systemc>
#include "ex_globals.h"
#include "cci_configuration"
#include "param_impl/test_datatype.h"

/// Module which owns some std cci parameters (provided by lib A) and one additional one of lib implementation B.
class OwnerModuleB
: public sc_core::sc_module
{
  
public:
  
  SC_HAS_PROCESS(OwnerModuleB);
	
  /// Constructor
  OwnerModuleB(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  , int_param ("int_param", 150 )
  , bool_param("bool_param", false)
  , test_datatype_param("test_datatype_param", test_datatype())
  { 
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();
  
  /// Example parameter.
  cci::cci_param<int>             int_param;
  /// Example parameter.
  cci::cci_param<bool>            bool_param;
  /// Example parameter. which exists in the parameter implementation B
  cci::cci_param<test_datatype> test_datatype_param;
  
};


#endif
