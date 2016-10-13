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
 *  @file    parameter_owner.h
 *  @brief   This header instantiates various CCI parameters with default values
 *  @author  Girish Verma, CircuitSutra Technologies    <girish@circuitsutra.com>
 *           P V S Phaneendra, CircuitSutra Technologies    <pvs@circuitsutra.com>
 *  @date    31st August, 2011 (Wednesday)
 */

#ifndef   EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_USER_DATATYPE_H_
#define   EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_USER_DATATYPE_H_

#ifndef SC_INCLUDE_DYNAMIC_PROCESSES
  #define SC_INCLUDE_DYNAMIC_PROCESSES
#endif

#include <cci_configuration>
#include <systemc.h>
#include <greencontrol/config.h>
#include <string>
#include <iostream>

/**
 *  @class  route_table_ut
 *  @brief  Route_table_ut structure defines three fields of a typical router
 *          source_addr, dest_addr, and index
 */
struct route_table_ut {
 public:
	 friend bool operator== (const route_table_ut&, const route_table_ut&);

  /**
   *  @fn     route_table_ut()
   *  @brief  The class constructor
   *  @return void
   */
  route_table_ut()
      : s_address(0x0),  // Source Address
        d_address(0x0),  // Destination Address
        index(0x0) {}    // Index

  /**
   *  @fn     route_table_ut(int saddr, int daddr, int idx)
   *  @brief  Overloaded class constructor
   *  @param  saddr The source address
   *  @param  daddr The destination address
   *  @param  idx The index
   *  @return void
   */
  route_table_ut(int saddr, int daddr, int idx)
      : s_address(saddr),  // Source Address
        d_address(daddr),  // Destination Address
        index(idx) {}      // Index

 public:
  int s_address;  ///< Source address field
  int d_address;  ///< Destination address field
  int index;  ///< Index field
};

namespace cci {
// add support for cci_value and JSON (de)serialization
template<>
struct cci_value_traits< route_table_ut >
{
  typedef route_table_ut type;
  static bool pack( cci_value::reference dst, type const & src )
  {
    dst.set_map()
      .push_entry( "s_address", src.s_address )
      .push_entry( "d_address", src.d_address )
      .push_entry( "index"    , src.index );
    return true;
  }
  static bool unpack( type & dst, cci_value::const_reference src )
  {
    if( !src.is_map() )
      return false;

    cci_value::const_map_reference m = src.get_map();
    if( m.has_entry("s_address")
               && m["s_address"].try_get( dst.s_address )
     && m.has_entry("d_address")
               && m["d_address"].try_get( dst.d_address )
     && m.has_entry("index")
               && m["index"].try_get( dst.index ) )
       return true;

    return false;
  }
};
} /* namespace cci */

// Overload stream insertion operator of C++
std::ostream& operator <<(std::ostream& os, const route_table_ut& ud)
{
  cci::cci_value udv(ud);
  return os << udv;
}

// Overload stream extraction operator of C++
/** @todo This operator is currently required by the "GreenControl tool"
 *        implementation.  Its serialisation is not yet based on JSON but
 *        uses plain stream extraction to convert a value from a string.
 */
std::istream& operator >>(std::istream& is, route_table_ut& ud)
{
  cci::cci_value udv;
  is >> udv;
  sc_assert( is && "stream extraction of cci_value failed");
  bool check = udv.try_get(ud);
  sc_assert( check && "stream extraction of route_table_ut failed");
  return is;
}

//!< Provide parameter vendor implementation support for user-defined data type
typedef gs::cnf::gs_param<route_table_ut> user_data_type;


inline bool operator== (const route_table_ut& lhs, const route_table_ut& rhs)
{
	return lhs.d_address == rhs.d_address &&
		lhs.index == lhs.index &&
		lhs.s_address == lhs.s_address;
}

// ----------------------------------------------------------------------------
// copied from gs_cci_symbols.cpp

// TODO: fixme
/*#include "cci_params.h"

namespace cci {
#define CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, MUTABILITY_TYPE) \
  template  cci::cci_param_impl_if*  create_cci_param (cci::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const std::string &nam, const PTYPE& val, const bool is_top_level_name, cci_broker_if* broker_accessor, const std::string& desc); \
  template  cci::cci_param_impl_if*  create_cci_param (cci::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const std::string &nam, const cci_value& val,  const bool is_top_level_name, cci_broker_if* broker_accessor, const std::string& desc);

#define CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(PTYPE) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::mutable_param) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::immutable_param) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::elaboration_time_param) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::other_param)


CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(route_table_ut);

#undef CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS
#undef CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM

}*/ /* namespace cci */

// ----------------------------------------------------------------------------

#endif  // EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_USER_DATATYPE_H_
