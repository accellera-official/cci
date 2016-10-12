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
 * @file   ex_globals.h
 * @author Christian Schroeder, GreenSocs
 * @author Mark Burton, GreenSocs
 */


#ifndef __callb_globals_h__
#define __callb_globals_h__

// Some debug output for GrenConfig wrapped to CCI
//#define GS_PARAM_CALLBACK_VERBOSE

#define DEMO_VERBOSE

#ifdef DEMO_VERBOSE
# define DEMO_DUMP(name, msg) { printf("@%s /%d (%s): ", sc_core::sc_time_stamp().to_string().c_str(), (unsigned)sc_core::sc_delta_count(), name); std::cout << msg; printf("\n"); } 
#else
# define DEMO_DUMP(name, msg)
#endif

#endif
