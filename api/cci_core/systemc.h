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
 * @file   systemc.h
 * @brief  include SystemC from within the CCI library
 * @author Thomas Goodfellow, OFFIS
 * @author Philipp A. Hartmann, OFFIS
 *
 * This file provides a local indirection to include SystemC from within
 * the CCI library.  Prefer this file over direct inclusion of the SystemC
 * header inside the CCI implementation(s).
 *
 * Some features not yet present in some (supported) versions of SystemC
 * may require local workarounds, e.g. @ref sc_core::sc_get_current_object()
 * (added in SystemC 2.3.1).
 *
 */
#ifndef CCI_CORE_SYSTEMC_H_INCLUDED_
#define CCI_CORE_SYSTEMC_H_INCLUDED_

#include <systemc>

#endif // CCI_CORE_SYSTEMC_H_INCLUDED_
