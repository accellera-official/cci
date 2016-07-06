/*****************************************************************************
  Copyright 2006-2015 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2009-2011 GreenSocs Ltd
  All rights reserved.

  Copyright 2016 Ericsson AB.
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
 *  @author   Christian Schroeder <schroeder@eis.cs.tu-bs.de>
 *            Mark Burton <mark@greensocs.com>
 */

#ifndef CCI_CCI_FACTORY_H_INCLUDED_
#define CCI_CCI_FACTORY_H_INCLUDED_

#include <string>
#include "cci_datatypes.h"

/**
 * @file
 * @author Christian Schroeder, GreenSocs Ltd.
 * @author Mark Burton, GreenSocs Ltd.
 * @author Thomas Goodfellow, OFFIS
 * @brief Declarations of parameter factory functions
 */

CCI_OPEN_NAMESPACE_

template<class T, param_mutable_type TM>
class cci_param;

class cci_broker_if;

/// Parameter factory function being called from the cci param constructor to construct the underlying parameter
/**
 * Forward declaration, to be implemented by the parameter vendor(s) for all supported data types.
 *
 * The implementation shall return a new parameter object.
 *
 * @param owner_par CCI Parameter which will store the returned param
 * @param name      The (local or top-level) name the parameter shall get
 * @param value     Default value for this parameter
 * @param is_top_level_name If the given name shall be a top-level name
 * @param broker_accessor The broker accessor responsible for this parameter
 */
template<typename T, param_mutable_type TM>
cci_param_impl_if* create_cci_param(cci_param<T, TM>* owner_par, const std::string& name, const T& value, const bool is_top_level_name, cci_broker_if* broker_accessor, const std::string& desc = "");

/// Parameter factory function being called from the cci param constructor to construct the underlying parameter
/**
 * Forward declaration, to be implemented by the parameter vendor(s) for all supported data types.
 *
 * The implementation shall return a new parameter object.
 *
 * @param owner_par CCI Parameter which will store the returned param
 * @param name      The (local or top-level) name the parameter shall get
 * @param value     Default value (as JSON string) for this parameter
 * @param is_top_level_name If the given name shall be a top-level name
 * @param broker_accessor The broker accessor responsible for this parameter
 */
template<typename T, param_mutable_type TM>
cci_param_impl_if* create_cci_param(cci_param<T, TM>* owner_par, const std::string& name, const cci_value& value, const bool is_top_level_name, cci_broker_if* broker_accessor, const std::string& desc = "");

CCI_CLOSE_NAMESPACE_

#endif
