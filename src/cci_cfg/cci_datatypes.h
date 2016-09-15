/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010 GreenSocs
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

#ifndef CCI_CCI_DATATYPES_H_INCLUDED_
#define CCI_CCI_DATATYPES_H_INCLUDED_

#include "cci_cfg/cci_config_macros.h"

CCI_OPEN_NAMESPACE_

  /// Enumeration for basic data types, whose rough getting and setting is supported by base class directly
  enum basic_param_type {
    /// default, type unknown
    param_type_not_available = 0, 
    /// used for all param data types that can handle integer numbers
    param_type_number,   
    /// used for all param data types that can handle real numbers
    param_type_real,     
    /// used for all param data types that can handle bool
    param_type_bool,     
    /// used for all param data types that can handle strings
    param_type_string,   
    /// used for all param lists 
    param_type_list,     
    /// used for all param other data types (objects etc.)
    param_type_other     
  };
  
  
  /// Enumeration for cci_param_typed template specifying the parameter type according the lock behavior
  enum param_mutable_type {
    /// Mutable Parameter
    mutable_param = 0,
    /// Immutable Parameter
    immutable_param,
    /// Elaboration Time Parameter (Mutable up to end_of_elaboration)
    elaboration_time_param,
    /// Vendor specific/other Parameter type
    other_param
  };

  /**
   * Enumeration for convenient cci_param_typed constructor.
   * Choose if a param gets a relative or an absolute name.
   */
  enum cci_name_type {
    CCI_RELATIVE_NAME,
    CCI_ABSOLUTE_NAME
  };
  
  
CCI_CLOSE_NAMESPACE_

#endif
