/*****************************************************************************
  Copyright 2006-2015 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2015 Ericsson
  All rights reserved.

  Copyright 2006-2015 Intel Corporation
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

/**
 * @file   cci_hierarchical_values_set/ParentModule.h
 * @author Lei Liang, Ericsson
 */

#ifndef __PARENTMODULE_H__
#define __PARENTMODULE_H__


#include <systemc>
#include <cci_configuration>
#include "ChildModule.h"

#ifndef SC_INCLUDE_DYNAMIC_PROCESSES
#define SC_INCLUDE_DYNAMIC_PROCESSES
#endif

class ParentModule
: public sc_core::sc_module
{

public:

  SC_HAS_PROCESS(ChildModule);

  ParentModule(sc_core::sc_module_name name):
    log_level("log_level", 0),
    mParam1("mParam1", 0),
    mParam2("mParam2", 0),
    mChild("ChildModule")
  {

  };

  ~ParentModule(){};

  cci::cci_param<int> log_level;
  cci::cci_param<int> mParam1;
  cci::cci_param<int> mParam2;

private:

  ChildModule mChild;
};


#endif
