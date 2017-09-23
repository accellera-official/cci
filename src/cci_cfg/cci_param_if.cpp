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
 * @file   cci_param_if.cpp
 * @author Philipp A Hartmann, Intel
 */

#include "cci_cfg/cci_param_if.h"
#include "cci_cfg/cci_broker_if.h"
#include "cci_cfg/cci_report_handler.h"

CCI_OPEN_NAMESPACE_

void cci_param_if::init( cci_broker_handle broker_handle )
{
  cci_broker_if& broker = broker_handle.ref();
  const std::string& nm = get_name();
  broker.add_param( this );
  if( broker.has_initial_value(nm) ) {
    preset_cci_value( broker.get_initial_cci_value(nm)
                    , broker.get_latest_write_originator(nm) );
  }
}

cci_originator cci_param_if::get_originator() const
{
  return cci_originator(cci_originator::unknown_tag());
}

cci_originator cci_param_if::get_latest_write_originator() const
{
  return cci_originator(cci_originator::unknown_tag());
}

void cci_param_if::destroy( cci_broker_handle broker_handle )
{
  broker_handle.ref().remove_param( this );
  invalidate_all_param_handles();
}

void cci_param_if::preset_cci_value( const cci_value& value
                                   , const cci_originator& originator )
{
  // forwards to a regular value update by default
  set_cci_value(value, originator);
}

void cci_param_if::invalidate_all_param_handles()
{
  CCI_REPORT_FATAL( "DESTROY_PARAM"
                  , "Failed to invalidate parameter handles! "
                    "(pure virtual function called)" );
}

CCI_CLOSE_NAMESPACE_
