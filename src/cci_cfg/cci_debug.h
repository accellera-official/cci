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

/**
 * @file
 * @author Christian Schroeder, GreenSocs Ltd.
 * @author Mark Burton, GreenSocs Ltd.
 * @author Thomas Goodfellow, OFFIS
 * @brief Debugging support for the SystemC CCI library
 *
 */


#ifndef CCI_CCI_DEBUG_H_INCLUDED_
#define CCI_CCI_DEBUG_H_INCLUDED_


#include <cassert>

#if defined(CCI_CNF_VERBOSE) || defined(CCI_PARAM_VERBOSE)
#include <string>
#include <iostream>
#endif

#ifdef CCI_CNF_VERBOSE
# define CCI_CNF_DUMP(msg) { std::cout<<msg<<std::endl; } 
#else
# define CCI_CNF_DUMP(msg) {  } 
#endif

// Debug output of the parameter objects
#ifdef CCI_PARAM_VERBOSE
# define CCI_PARAM_DUMP(msg) { std::cout<<this->name()<<": "<<msg<<std::endl; } 
# define CCI_PARAM_DUMP_WITHNAME(name, msg) { std::cout<<name<<": "<<msg<<std::endl; } 
#else
# define CCI_PARAM_DUMP(msg)
# define CCI_PARAM_DUMP_WITHNAME(name, msg)
#endif


#endif
