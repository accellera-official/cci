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

#ifndef CCI_CNF_CCI_VALUE_TRAITS_H_INCLUDED_
#define CCI_CNF_CCI_VALUE_TRAITS_H_INCLUDED_

#include "cci_core/systemc.h"
#include "cci_cnf/cci_value.h"

#include <cstring> // std::strncpy

/**
 * @file   cci_value_traits.h
 * @brief  conversions from and to a @ref cci_value
 * @author Philipp A. Hartmann, OFFIS
 */

CCI_OPEN_CONFIG_NAMESPACE_

/**
 * @class cci_value_traits
 * @brief traits class for cci_value conversions
 * @tparam T C++ datatype to convert to/from @ref cci_value
 *
 * @note By default, the primary template is not implemented to
 *       enable instantiations with incomplete types.
 */
template<typename T>
struct cci_value_traits
{
  typedef T type; ///< common type alias
  static bool pack( cci_value::reference dst, type const & src );
  static bool unpack( type & dst, cci_value::const_reference src );
};

// ---------------------------------------------------------------------------
// disabled implementation as a safety guard
template<> struct cci_value_traits<cci_value>;
template<> struct cci_value_traits<cci_value_cref>;
template<> struct cci_value_traits<cci_value_ref>;
template<> struct cci_value_traits<cci_value_list>;
template<> struct cci_value_traits<cci_value_list_cref>;
template<> struct cci_value_traits<cci_value_list_ref>;

// ---------------------------------------------------------------------------
// helper to convert compatible types (implementation artefact)
template< typename T, typename U >
struct cci_value_traits_convert
{
  typedef T type;
  typedef cci_value_traits<U> traits_type;

  static bool pack( cci_value::reference dst, type const & src )
  {
    return traits_type::pack( dst, U(src) );
  }

  static bool unpack( type & dst, cci_value::const_reference src )
  {
      U u_dst;
      bool ret = traits_type::unpack( u_dst, src );
      if( ret )
        dst = u_dst;
      return ret;
  }
};

// --------------------------------------------------------------------------
// C++ builtin types

// default instantiations (in cci_value_traits.cpp)
template struct cci_value_traits<bool>;
template struct cci_value_traits<int>;
template struct cci_value_traits<int64>;
template struct cci_value_traits<unsigned>;
template struct cci_value_traits<uint64>;
template struct cci_value_traits<double>;
template struct cci_value_traits<std::string>;

// related numerical types
// (without range checks for now)

#define CCI_VALUE_TRAITS_DERIVED_( UnderlyingType, SpecializedType ) \
  template<> \
  struct cci_value_traits<SpecializedType> \
    : cci_value_traits_convert<SpecializedType, UnderlyingType > {}

CCI_VALUE_TRAITS_DERIVED_( int, char );
CCI_VALUE_TRAITS_DERIVED_( int, signed char );
CCI_VALUE_TRAITS_DERIVED_( int, short );
CCI_VALUE_TRAITS_DERIVED_( unsigned, unsigned char );
CCI_VALUE_TRAITS_DERIVED_( unsigned, unsigned short );
CCI_VALUE_TRAITS_DERIVED_( int64, long );
CCI_VALUE_TRAITS_DERIVED_( uint64, unsigned long );
CCI_VALUE_TRAITS_DERIVED_( double, float );

// ----------------------------------------------------------------------------
// C++ string literals

template<int N>
struct cci_value_traits<char[N]>
{
  typedef char type[N]; ///< common type alias
  static bool pack( cci_value::reference dst, type const & src )
  {
    dst.set_string( src );
    return true;
  }
  static bool unpack( type & dst, cci_value::const_reference src )
  {
    if( src.is_null() )
    {
      dst[0] = '\0'; // convert "null" to empty string
      return true;
    }
    if( !src.is_string() )
      return false;

    cci_value::const_string_reference str = src.get_string();
    std::strncpy( dst, str, N-1 );
    dst[N-1] = '\0';
    return ( str.size() <= N-1 );
  }
};

// SystemC builtin types

// default instantiations (in cci_value_traits.cpp)
template struct cci_value_traits<sc_core::sc_time>;
template struct cci_value_traits<sc_dt::sc_int_base>;
template struct cci_value_traits<sc_dt::sc_uint_base>;
template struct cci_value_traits<sc_dt::sc_signed>;
template struct cci_value_traits<sc_dt::sc_unsigned>;
template struct cci_value_traits<sc_dt::sc_bv_base>;
template struct cci_value_traits<sc_dt::sc_lv_base>;

template<int N>
struct cci_value_traits< sc_dt::sc_int<N> >
  : cci_value_traits< sc_dt::sc_int_base >
{
  typedef sc_dt::sc_int<N> type;
  /// @todo add bounds checks
};

template<int N>
struct cci_value_traits< sc_dt::sc_uint<N> >
  : cci_value_traits< sc_dt::sc_uint_base >
{
  typedef sc_dt::sc_uint<N> type;
  /// @todo add bounds checks
};

CCI_CLOSE_CONFIG_NAMESPACE_

#undef CCI_VALUE_TRAITS_DERIVED_

#endif // CCI_CNF_CCI_VALUE_TRAITS_H_INCLUDED_
