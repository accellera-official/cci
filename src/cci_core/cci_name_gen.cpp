/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2011 GreenSocs
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

#include <sstream>
#include <cassert>
#include <cstring>

#include "cci_core/cci_name_gen.h"
#include "cci_cfg/cci_report_handler.h"

CCI_OPEN_NAMESPACE_

const char* cci_gen_unique_name(const char *name)
{
    if(!name || !std::strlen(name)) {
        CCI_REPORT_ERROR("cci_name_gen/gen_unique_name",
                         "empty name is not allowed");
    }
    if (!sc_core::sc_register_hierarchical_name(name)) {
        const char* new_name = sc_core::sc_gen_unique_name(name);
        sc_core::sc_register_hierarchical_name(new_name);
        std::stringstream msg;
        msg << name << " is already used in the SystemC hierarchy, using "
            << new_name << " instead";
        CCI_REPORT_WARNING("cci_name_gen/gen_unique_name", msg.str().c_str());
        return sc_core::sc_get_hierarchical_name(new_name);
    }
    return sc_core::sc_get_hierarchical_name(name);
}

const char* cci_get_name(const char *name)
{
    return sc_core::sc_get_hierarchical_name(name);
}

CCI_CLOSE_NAMESPACE_