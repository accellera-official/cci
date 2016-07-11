/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2009-2011 GreenSocs
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

#ifndef __ex_globals_h__
#define __ex_globals_h__

// Some debug output for GrenConfig wrapped to CCI
//#define GS_PARAM_CALLBACK_VERBOSE

#define DEMO_VERBOSE

#ifdef DEMO_VERBOSE
# define DEMO_DUMP(name, msg)     { printf("@%s /%d (%s): ", sc_core::sc_time_stamp().to_string().c_str(), (unsigned)sc_core::sc_delta_count(), name); std::cout << msg; printf("\n"); } 
# define DEMO_DUMP_RAW(name, msg) { printf("(%s):", name); std::cout << msg; printf("\n"); } 
#else
# define DEMO_DUMP(name, msg)
# define DEMO_DUMP_RAW(name, msg)
#endif

#endif
