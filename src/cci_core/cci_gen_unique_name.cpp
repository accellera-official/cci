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

#include "cci_core/cci_gen_unique_name.h"
#include <sstream>
#include <cassert>

inline
const char* cci::cci_gen_unique_name(const char* name) {
  
  // static (system-wide) map <name, current counter - 1>
  static std::map<std::string, int> unique_name_map;
  
  std::pair<std::map<std::string,int>::iterator,bool> ret;
  
  // TODO: add error reporting
  assert(name && *name && "cci_gen_unique_name: empty name not allowed!");

  // Try to insert name
  ret = unique_name_map.insert(std::pair<std::string, int>(name, 0));
  if (ret.second==false)
  {
    // insert failed, so name clash
    std::stringstream new_name;
    new_name << name << "_" << ret.first->second; // generate new name
    ret.first->second++; // update counter
    // recursive call, to store the new generated name in the map as well!
    cci_gen_unique_name(new_name.str().c_str());
    // return pointer to stored new name which is persistent
    return unique_name_map.find(new_name.str())->first.c_str();
  }
  // insert successful
  return name;
  
}
