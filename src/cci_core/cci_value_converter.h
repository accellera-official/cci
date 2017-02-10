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

#ifndef CCI_EXTERN_
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable: 4231) // don't warn about extern templates
# define CCI_EXTERN_ extern
#else
# define CCI_EXTERN_
#endif
#endif // CCI_EXTERN_

#ifndef CCI_cci_value_converter_H_INCLUDED_
#define CCI_cci_value_converter_H_INCLUDED_

#include "cci_core/systemc.h"
#include "cci_core/cci_value.h"

#include <cstring> // std::strncpy

/**
 * @file   cci_value_converter.h
 * @brief  conversions from and to a @ref cci_value
 * @author Philipp A. Hartmann, OFFIS/Intel
 */

CCI_OPEN_NAMESPACE_

/**
 * @class cci_value_converter
 * @brief traits class for cci_value conversions
 * @tparam T C++ datatype to convert to/from @ref cci_value
 *
 * Whenever a C++ type @c T is used in conjunction with a cci_value,
 * the requireed value conversion is performed by this traits class,
 * providing the two conversion functions @ref pack and @ref unpack.
 * Both functions return @c true upon success and @c false otherwise.
 * In case of a failing conversion, it is recommended to leave the given
 * destination object @c dst untouched.
 *
 * @note By default, the primary template is not implemented to
 *       enable instantiations with incomplete types.
 *
 * You only need to implement the two functions @ref pack / @ref unpack
 * to enable conversion support for your custom datatype:
 * @code
 * struct my_int { int value; };
 *
 * template<> bool
 * cci_value_converter<my_int>::pack( cci_value::reference dst, type const & src )
 * {
 *    dst.set_int( src.value );
 *    return true;
 * }
 * template<> bool
 * cci_value_converter<my_int>::unpack( type & dst, cci_value::const_reference src )
 * {
 *    if( ! src.is_int() ) return false;
 *    dst.value  = src.get_int();
 *    return true;
 * }
 * @endcode
 *
 * To @em disable conversion support for a given type, you can refer
 * to the helper template @ref cci_value_converter_disabled.
 */
template<typename T>
struct cci_value_converter
{
  typedef T type; ///< common type alias
  /// convert from \ref type value to a \ref cci_value
  static bool pack( cci_value::reference dst, type const & src );
  /// convert from \ref cci_value to a \ref type value
  static bool unpack( type & dst, cci_value::const_reference src );
};

// ---------------------------------------------------------------------------
/**
 * @brief helper to disable cci_value conversion for a given type
 * @tparam T type without cci_value conversions
 *
 * In order to disable the conversion from/to a cci_value for a given type
 * @c T during @em run-time, you can simply inherit from this helper in
 * the specialization of cci_value_converter:
 * @code
 * struct my_type { ... };
 * template<>
 * struct cci_value_converter<my_type>
 *   : cci_value_converter_disabled<my_type> {};
 * @endcode
 *
 * \note In order to disable support for a given type at @em compile-time,
 *       the specialization of cci_value_converter can be left empty.
 */
template< typename T >
struct cci_value_converter_disabled
{
  typedef T type;
  static bool pack( cci_value::reference, T const & ) { return false; }
  static bool unpack( type &, cci_value::const_reference ) { return false; }
};

///@cond CCI_HIDDEN_FROM_DOXYGEN

// ---------------------------------------------------------------------------
// disallowed implementation as a safety guard

template<typename T> struct cci_value_converter<T*>        { /* disallowed */ };

template<> struct cci_value_converter<cci_value>           { /* disallowed */ };
template<> struct cci_value_converter<cci_value_cref>      { /* disallowed */ };
template<> struct cci_value_converter<cci_value_ref>       { /* disallowed */ };
template<> struct cci_value_converter<cci_value_list>      { /* disallowed */ };
template<> struct cci_value_converter<cci_value_list_cref> { /* disallowed */ };
template<> struct cci_value_converter<cci_value_list_ref>  { /* disallowed */ };

// ---------------------------------------------------------------------------
/// helper to convert compatible types (implementation artefact)
template< typename T, typename U >
struct cci_value_delegate_converter
{
  typedef T type;
  typedef cci_value_converter<U> traits_type;

  static bool pack( cci_value::reference dst, type const & src )
  {
    return traits_type::pack( dst, static_cast<U>(src) );
  }

  static bool unpack( type & dst, cci_value::const_reference src )
  {
      U u_dst;
      bool ret = traits_type::unpack( u_dst, src );
      if( ret )
        dst = static_cast<T>(u_dst);
      return ret;
  }
};

// --------------------------------------------------------------------------
// C++ builtin types

// default instantiations (in cci_value_converter.cpp)
CCI_EXTERN_ template struct cci_value_converter<bool>;
CCI_EXTERN_ template struct cci_value_converter<int>;
CCI_EXTERN_ template struct cci_value_converter<int64>;
CCI_EXTERN_ template struct cci_value_converter<unsigned>;
CCI_EXTERN_ template struct cci_value_converter<uint64>;
CCI_EXTERN_ template struct cci_value_converter<double>;
CCI_EXTERN_ template struct cci_value_converter<std::string>;

// related numerical types
// (without range checks for now)

#define cci_value_converter_DERIVED_( UnderlyingType, SpecializedType ) \
  template<> \
  struct cci_value_converter<SpecializedType> \
    : cci_value_delegate_converter<SpecializedType, UnderlyingType > {}

cci_value_converter_DERIVED_( int, char );
cci_value_converter_DERIVED_( int, signed char );
cci_value_converter_DERIVED_( int, short );
cci_value_converter_DERIVED_( unsigned, unsigned char );
cci_value_converter_DERIVED_( unsigned, unsigned short );
cci_value_converter_DERIVED_( int64, long );
cci_value_converter_DERIVED_( uint64, unsigned long );
cci_value_converter_DERIVED_( double, float );

///@endcond CCI_HIDDEN_FROM_DOXYGEN

// ----------------------------------------------------------------------------
// C++ string literals

template<int N>
struct cci_value_converter<char[N]>
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
struct cci_value_converter<T[N]>
{
  typedef T type[N]; ///< common type alias
  static bool pack( cci_value::reference dst, type const & src )
  {
    cci_value_list ret;
    ret.reserve( N );

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

template<typename T, int N>
struct cci_value_converter<const T[N]> : cci_value_converter<T[N]>
{
  typedef const T type[N]; ///< common type alias
  // deliberately not implemented
  static bool unpack( type & dst, cci_value::const_reference src );
};

// ----------------------------------------------------------------------------
// std::vector<T, Alloc>

template< typename T, typename Alloc >
struct cci_value_converter< std::vector<T,Alloc> >
{
  typedef std::vector<T,Alloc> type; ///< common type alias
  static bool pack( cci_value::reference dst, type const & src )
  {
    cci_value_list ret;
    ret.reserve( src.size() );

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

// default instantiations (in cci_value_converter.cpp)
CCI_EXTERN_ template struct cci_value_converter<sc_core::sc_time>;
CCI_EXTERN_ template struct cci_value_converter<sc_dt::sc_logic>;
CCI_EXTERN_ template struct cci_value_converter<sc_dt::sc_int_base>;
CCI_EXTERN_ template struct cci_value_converter<sc_dt::sc_uint_base>;
CCI_EXTERN_ template struct cci_value_converter<sc_dt::sc_signed>;
CCI_EXTERN_ template struct cci_value_converter<sc_dt::sc_unsigned>;
CCI_EXTERN_ template struct cci_value_converter<sc_dt::sc_bv_base>;
CCI_EXTERN_ template struct cci_value_converter<sc_dt::sc_lv_base>;

template<int N>
struct cci_value_converter< sc_dt::sc_int<N> >
  : cci_value_converter< sc_dt::sc_int_base >
{
  typedef sc_dt::sc_int<N> type;
};

template<int N>
struct cci_value_converter< sc_dt::sc_uint<N> >
  : cci_value_converter< sc_dt::sc_uint_base >
{
  typedef sc_dt::sc_uint<N> type;
};

template<int N>
struct cci_value_converter< sc_dt::sc_bigint<N> >
  : cci_value_converter< sc_dt::sc_signed >
{
  typedef sc_dt::sc_bigint<N> type;
};

template<int N>
struct cci_value_converter< sc_dt::sc_biguint<N> >
  : cci_value_converter< sc_dt::sc_unsigned >
{
  typedef sc_dt::sc_biguint<N> type;
};

template<int N>
struct cci_value_converter< sc_dt::sc_bv<N> >
  : cci_value_converter< sc_dt::sc_bv_base >
{
  typedef sc_dt::sc_bv<N> type;
};

template<int N>
struct cci_value_converter< sc_dt::sc_lv<N> >
  : cci_value_converter< sc_dt::sc_lv_base >
{
  typedef sc_dt::sc_lv<N> type;
};

CCI_CLOSE_NAMESPACE_

#endif // CCI_cci_value_converter_H_INCLUDED_

///@todo add support for SystemC fixpoint types
#if defined(SC_INCLUDE_FX) && !defined(CCI_CNF_cci_value_converter_H_INCLUDED_FX_)
#define CCI_CNF_cci_value_converter_H_INCLUDED_FX_
CCI_OPEN_NAMESPACE_

CCI_EXTERN_ template struct cci_value_converter<sc_dt::sc_fxval>;
CCI_EXTERN_ template struct cci_value_converter<sc_dt::sc_fxval_fast>;
CCI_EXTERN_ template struct cci_value_converter<sc_dt::sc_fix>;
CCI_EXTERN_ template struct cci_value_converter<sc_dt::sc_fix_fast>;
CCI_EXTERN_ template struct cci_value_converter<sc_dt::sc_ufix>;
CCI_EXTERN_ template struct cci_value_converter<sc_dt::sc_ufix_fast>;

template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N >
struct cci_value_converter< sc_dt::sc_fixed<W,I,Q,O,N> >
  : cci_value_converter< sc_dt::sc_fix >
{
  typedef sc_dt::sc_fixed<W,I,Q,O,N> type;
};

template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N >
struct cci_value_converter< sc_dt::sc_fixed_fast<W,I,Q,O,N> >
  : cci_value_converter< sc_dt::sc_fix_fast >
{
  typedef sc_dt::sc_fixed_fast<W,I,Q,O,N> type;
};

template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N >
struct cci_value_converter< sc_dt::sc_ufixed<W,I,Q,O,N> >
  : cci_value_converter< sc_dt::sc_ufix >
{
  typedef sc_dt::sc_ufixed<W,I,Q,O,N> type;
};

template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N >
struct cci_value_converter< sc_dt::sc_ufixed_fast<W,I,Q,O,N> >
  : cci_value_converter< sc_dt::sc_ufix_fast >
{
  typedef sc_dt::sc_ufixed_fast<W,I,Q,O,N> type;
};

CCI_CLOSE_NAMESPACE_
#endif // SC_INCLUDE_FX && ! CCI_CNF_cci_value_converter_H_INCLUDED_FX_

#ifdef cci_value_converter_DERIVED_
#undef cci_value_converter_DERIVED_
#endif

#ifdef CCI_EXTERN_
# undef CCI_EXTERN_
# ifdef _MSC_VER
#  pragma warning(pop)
# endif
#endif // CCI_EXTERN_
