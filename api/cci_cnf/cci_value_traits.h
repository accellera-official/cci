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

template<typename T> struct cci_value_traits<T*>        { /* disabled */ };

template<> struct cci_value_traits<cci_value>           { /* disabled */ };
template<> struct cci_value_traits<cci_value_cref>      { /* disabled */ };
template<> struct cci_value_traits<cci_value_ref>       { /* disabled */ };
template<> struct cci_value_traits<cci_value_list>      { /* disabled */ };
template<> struct cci_value_traits<cci_value_list_cref> { /* disabled */ };
template<> struct cci_value_traits<cci_value_list_ref>  { /* disabled */ };

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

// ----------------------------------------------------------------------------
// C++ arrays

template<typename T, int N>
struct cci_value_traits<T[N]>
{
  typedef T type[N]; ///< common type alias
  static bool pack( cci_value::reference dst, type const & src )
  {
    cci_value_list ret;
    // ret.reserve(N);

    for( size_t i = 0; i < N; ++i )
      ret.push_back( src[i] );
    ret.swap( dst.set_list() );
    return true;
  }
  static bool unpack( type & dst, cci_value::const_reference src )
  {
    if( !src.is_list() )
      return false;

    cci_value::const_list_reference lst = src.get_list();
    size_t i = 0;
    for( ; i < N && i < lst.size() && lst[i].try_get<T>( dst[i] ); ++i ) {}

    return ( i == lst.size() );
  }
};

// ----------------------------------------------------------------------------
// std::vector<T, Alloc>

template< typename T, typename Alloc >
struct cci_value_traits< std::vector<T,Alloc> >
{
  typedef std::vector<T,Alloc> type; ///< common type alias
  static bool pack( cci_value::reference dst, type const & src )
  {
    cci_value_list ret;
    // ret.reserve( src.size() );

    for( size_t i = 0; i < src.size(); ++i )
      ret.push_back( src[i] );
    ret.swap( dst.set_list() );
    return true;
  }
  static bool unpack( type & dst, cci_value::const_reference src )
  {
    if( !src.is_list() )
      return false;

    cci_value::const_list_reference lst = src.get_list();
    type ret;
    T    cur;
    size_t i = 0;
    ret.reserve( lst.size() );
    for( ; i < lst.size() && lst[i].try_get(cur); ++i )
      ret.push_back( cur );

    return ( i == lst.size() ) ? ( dst.swap(ret), true) : false;
  }
};

// ----------------------------------------------------------------------------
// SystemC builtin types

// default instantiations (in cci_value_traits.cpp)
template struct cci_value_traits<sc_core::sc_time>;
template struct cci_value_traits<sc_dt::sc_logic>;
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

template<int N>
struct cci_value_traits< sc_dt::sc_bigint<N> >
  : cci_value_traits< sc_dt::sc_signed >
{
  typedef sc_dt::sc_bigint<N> type;
  /// @todo add bounds checks
};

template<int N>
struct cci_value_traits< sc_dt::sc_biguint<N> >
  : cci_value_traits< sc_dt::sc_unsigned >
{
  typedef sc_dt::sc_biguint<N> type;
  /// @todo add bounds checks
};

template<int N>
struct cci_value_traits< sc_dt::sc_bv<N> >
  : cci_value_traits< sc_dt::sc_bv_base >
{
  typedef sc_dt::sc_bv<N> type;
  /// @todo add bounds checks
};

template<int N>
struct cci_value_traits< sc_dt::sc_lv<N> >
  : cci_value_traits< sc_dt::sc_lv_base >
{
  typedef sc_dt::sc_lv<N> type;
  /// @todo add bounds checks
};

CCI_CLOSE_CONFIG_NAMESPACE_

#endif // CCI_CNF_CCI_VALUE_TRAITS_H_INCLUDED_

///@todo add support for SystemC fixpoint types
#if defined(SC_INCLUDE_FX) && !defined(CCI_CNF_CCI_VALUE_TRAITS_H_INCLUDED_FX_)
#define CCI_CNF_CCI_VALUE_TRAITS_H_INCLUDED_FX_
CCI_OPEN_CONFIG_NAMESPACE_

template struct cci_value_traits<sc_dt::sc_fxval>;
template struct cci_value_traits<sc_dt::sc_fxval_fast>;
template struct cci_value_traits<sc_dt::sc_fix>;
template struct cci_value_traits<sc_dt::sc_ufix>;
template struct cci_value_traits<sc_dt::sc_fixed_fast>;

template<int W, int I, sc_dt::sc_q_mode, sc_dt::sc_o_mode O, int N >
struct cci_value_traits< sc_dt::sc_fixed<W,I,Q,O,N> >
  : cci_value_traits< sc_fix >
{
  typedef sc_dt::sc_fixed<W,I,Q,O,N> type;
  /// @todo add bounds checks
};

template<int W, int I, sc_dt::sc_q_mode, sc_dt::sc_o_mode O, int N >
struct cci_value_traits< sc_dt::sc_ufixed<W,I,Q,O,N> >
  : cci_value_traits< sc_ufix >
{
  typedef sc_dt::sc_ufixed<W,I,Q,O,N> type;
  /// @todo add bounds checks
};

CCI_CLOSE_CONFIG_NAMESPACE_
#endif // SC_INCLUDE_FX && ! CCI_CNF_CCI_VALUE_TRAITS_H_INCLUDED_FX_

#ifdef CCI_VALUE_TRAITS_DERIVED_
#undef CCI_VALUE_TRAITS_DERIVED_
#endif