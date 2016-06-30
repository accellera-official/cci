/*****************************************************************************
  Copyright 2006-2015 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2009-2011 GreenSocs Ltd
  All rights reserved.

  Copyright 2006-2014 OFFIS Institute for Information Technology
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
 * @author Thomas Goodfellow, OFFIS
 * @brief Undef CCI internal macros (housekeeping)
 */

// Since this is solely for removing symbols it can be included mulitple times
// safely (which may even be useful, when hunting a symbol's origin)

#undef CCI_NOT_SUPPORTED_WRN
#undef CCI_REPORT_INFO
#undef CCI_REPORT_WARNING
#undef CCI_REPORT_ERROR
#undef CCI_REPORT_FATAL

/** Define this before including root "cci" to make use of naming and debugging
  * macros; nobody beyond API implementors should *need* this
  */
#ifndef CCI_CNF_SHARE_LIBRARY_MACROS

#undef __CCI_OWNER_ORIGINATOR_STRING__
#undef __CCI_DEFAULT_BROKER_STRING__
#undef __CCI_UNKNOWN_ORIGINATOR_STRING__
#undef __CCI_SC_REPORT_MSG_TYPE_PREFIX__
#undef __CCI_CNF_SC_REPORT_MSG_TYPE_PREFIX__

// May need these debugging macros split out to a separate undef header, so
// that the client can remove all macros except the debugging ones
#undef CCI_CNF_DUMP
#undef CCI_PARAM_DUMP
#undef CCI_SHOW_BROKER_STACK

#endif
