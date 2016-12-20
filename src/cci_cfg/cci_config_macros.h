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
 * @author Christian Schroeder, GreenSocs Ltd.
 * @author Mark Burton, GreenSocs Ltd.
 */

#ifndef CCI_CCI_CONFIG_MACROS_H_INCLUDED_
#define CCI_CCI_CONFIG_MACROS_H_INCLUDED_

#include "cci_core/cci_cmnhdr.h"

// Some default strings - user should define custom ones before including CCI
// (and before building parameter or broker code since they are used beyond
// the client API library)
#ifndef  __CCI_OWNER_ORIGINATOR_STRING__
#define __CCI_OWNER_ORIGINATOR_STRING__   "OWNER_ORIGINATOR"
#endif
#ifndef __CCI_DEFAULT_BROKER_STRING__
#define __CCI_DEFAULT_BROKER_STRING__     "DEFAULT_BROKER"
#endif
#ifndef __CCI_UNKNOWN_ORIGINATOR_STRING__
#define __CCI_UNKNOWN_ORIGINATOR_STRING__ "UNKNOWN_ORIGINATOR"
#endif
#ifndef __CCI_SC_REPORT_MSG_TYPE_PREFIX__
#define __CCI_SC_REPORT_MSG_TYPE_PREFIX__ "/Accellera/CCI/"
#endif
#ifndef __CCI_CNF_SC_REPORT_MSG_TYPE_PREFIX__
#define __CCI_CNF_SC_REPORT_MSG_TYPE_PREFIX__ __CCI_SC_REPORT_MSG_TYPE_PREFIX__ "CNF/"
#endif

#endif //CCI_CCI_CONFIG_MACROS_H_INCLUDED_
