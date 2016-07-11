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


#include "OwnerModuleA.h"

void OwnerModuleA::main_action() {

  std::cout << "----------------------------" << std::endl;

  // get the broker which is responsible for this module
  cci::cci_broker_if* mBroker = &cci::cci_broker_manager::get_current_broker(cci::cci_originator(*this));
  assert(mBroker != NULL && "get_cnf_broker_instance returned is NULL");

  
  std::cout << "----------------------------" << std::endl;

}
