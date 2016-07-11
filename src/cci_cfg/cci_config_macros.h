/*****************************************************************************
  Copyright 2006-2015 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2009-2011 GreenSocs Ltd
  All rights reserved.

  Copyright 2006-2015 Intel Corporation
  All rights reserved.

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
 * @author Christian Schroeder, GreenSocs Ltd.
 * @author Mark Burton, GreenSocs Ltd.
 */

#ifndef CCI_CCI_CONFIG_MACROS_H_INCLUDED_
#define CCI_CCI_CONFIG_MACROS_H_INCLUDED_

/// Namespace for ASI Configuration, Control & Inspection (CCI) standard, Config part
#define CCI_OPEN_NAMESPACE_  namespace cci {
#define CCI_CLOSE_NAMESPACE_ }

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
#define __CCI_SC_REPORT_MSG_TYPE_PREFIX__ "/ASI/CCI/"
#endif
#ifndef __CCI_CNF_SC_REPORT_MSG_TYPE_PREFIX__
#define __CCI_CNF_SC_REPORT_MSG_TYPE_PREFIX__ __CCI_SC_REPORT_MSG_TYPE_PREFIX__ "CNF"
#endif

#endif
