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

#ifndef CCI_CCI_CORE_TYPES_H_INCLUDED_
#define CCI_CCI_CORE_TYPES_H_INCLUDED_

#include "cci_cfg/cci_config_macros.h"

CCI_OPEN_NAMESPACE_

/**
 *  Enumeration for basic data types, whose rough getting and setting is
 *  supported by base class directly
 */
enum cci_data_type {
    /// Default, unknow data types
    CCI_UNAVAILABLE_DATA = 0,
    /// Data types that can handle integer numbers
    CCI_NUMBER_DATA,
    /// Data types that can handle real numbers
    CCI_REAL_DATA,
    /// Data types that can handle bools
    CCI_BOOL_DATA,
    /// Data types that can handle strings
    CCI_STRING_DATA,
    /// Data types that can handle lists
    CCI_LIST_DATA,
    /// Data types that can handle other data types (objects etc.)
    CCI_OTHER_DATA
};

/**
 * Enumeration for convenient cci_param_typed constructor.
 * Choose if a param gets a relative or an absolute name.
 */
enum cci_name_type {
    /// Relative name
    CCI_RELATIVE_NAME,
    /// Absolute name
    CCI_ABSOLUTE_NAME
};

CCI_CLOSE_NAMESPACE_

#endif