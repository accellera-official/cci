/*****************************************************************************
  Copyright 2006-2014 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2006-2014 OFFIS Institute for Information Technology
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
 * @file   cci_value.cpp
 * @author Philipp A. Hartmann, OFFIS
 */

#include "cci_cnf/cci_value.h"
#include "cci_cnf/cci_report_handler.h"


CCI_OPEN_CONFIG_NAMESPACE_


// Borrowing this .cpp to provide a home for this value (creating cci_param.cpp
// solely for that purpose is overkill. @TODO - review home later in rework
const char* PARAM_ORIGINATOR = "owning_param";

CCI_CLOSE_CONFIG_NAMESPACE_
