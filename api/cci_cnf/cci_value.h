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

#ifndef CCI_CNF_CCI_VALUE_H_INCLUDED_
#define CCI_CNF_CCI_VALUE_H_INCLUDED_

#include "cci_datatypes.h"
#include "cci_core/systemc.h" // sc_dt::(u)int64, potentially strip out

/**
 * @file   cci_value.h
 * @author Philipp A. Hartmann, OFFIS
 */

CCI_OPEN_CONFIG_NAMESPACE_

// define our own typedefs to avoid SystemC dependency?
using sc_dt::int64;
using sc_dt::uint64;

// forward declarations
class cci_value;
class cci_value_cref;
class cci_value_ref;

template<typename T> struct cci_value_traits;

// --------------------------------------------------------------------------

/// constant reference to a (nested) @ref cci_value
class cci_value_cref
{
  friend class cci_value_ref;

protected:
  typedef void impl; // use type-punned pointer for now

  explicit cci_value_cref(impl* i = NULL)
    : pimpl_(i) {}

public:

  //!@name type queries
  basic_param_type  basic_type() const;
  bool is_null()    const;

  bool is_bool()    const;
  bool is_false()   const
    { return is_bool() && !get_bool(); }
  bool is_true()    const
    { return is_bool() &&  get_bool(); }

  bool is_number()  const
    { return is_int64() || is_uint64() || is_double(); }

  bool is_int()     const;
  bool is_uint()    const;
  bool is_int64()   const;
  bool is_uint64()  const;
  bool is_double()  const;

  bool is_string()  const;

  bool is_map()     const;

  bool is_list()    const;
  //@}

  //!@name value queries

  bool get_bool()    const;

  int      get_int()     const;
  unsigned get_uint()    const;
  int64    get_int64()   const;
  uint64   get_uint64()  const;
  double   get_double()  const;

  template<typename T>  bool try_get( T& dst ) const;
  template<typename T>  T    get() const;

  //@}

  bool json_serialize( std::string& ) const;

protected:
  void
  report_error( const char* msg
              , const char* file = NULL, int line = 0 ) const;

protected:
  impl* pimpl_;

private:
  // constant reference, no assignment
  cci_value_cref operator=( cci_value_cref const& ) /* = delete */;
};

template<typename T>
bool cci_value_cref::try_get( T& dst ) const
{
  typedef cci_value_traits<T> traits;
  return traits::unpack( dst, *this );
}

template<typename T>
T cci_value_cref::get() const
{
  T result;
  if( !try_get(result) ) {
    report_error("conversion from cci_value failed", __FILE__, __LINE__);
  }
  return result;
}

// --------------------------------------------------------------------------

/// mutable reference to a (nested) @ref cci_value
class cci_value_ref
  : public cci_value_cref
{
  typedef cci_value_cref base_type;
  typedef cci_value_ref  this_type;

protected:
  explicit cci_value_ref(impl* i = NULL)
    : cci_value_cref(i) {}

public:
  void swap( this_type& that );
  this_type operator=( const base_type& );
  this_type operator=( const this_type& );

  template<typename T> cci_value_ref set( T const & dst );
  template<typename T> bool          try_set( T const & dst );

  cci_value_ref set_null();
  cci_value_ref set_bool( bool v );
  cci_value_ref set_int( int v );
  cci_value_ref set_uint( unsigned v );
  cci_value_ref set_int64( int64 v );
  cci_value_ref set_uint64( uint64 v );
  cci_value_ref set_double( double v );


  bool json_deserialize( std::string const& );
};

inline cci_value_ref
cci_value_ref::operator=( this_type const & that )
  { return *this = base_type(that); }

template<typename T>
bool cci_value_ref::try_set( T const & src )
{
  typedef cci_value_traits<T> traits;
  return traits::pack( *this, src );
}

template<typename T>
cci_value_ref cci_value_ref::set( T const& src )
{
  if( !try_set(src) ) {
    report_error("conversion to cci_value failed", __FILE__, __LINE__);
  }
  return *this;
}

// --------------------------------------------------------------------------

class cci_value
  : public cci_value_ref
{
public:
  typedef cci_value_cref        const_reference;
  typedef cci_value_ref         reference;

  cci_value()
    : cci_value_ref(), own_pimpl_() {}

  explicit
  cci_value( basic_param_type ); ///< @todo drop this?

  template<typename T>
  explicit
  cci_value( T const & src );

  cci_value( const cci_value& that );
  cci_value( const_reference that );

  reference operator=( cci_value const& );
  reference operator=( const_reference );

  void swap( cci_value & that );
  void swap( reference that )
    { init(); reference::swap( that ); }

  ~cci_value();

  template< typename T >
  reference  set( T const & v )
    { init(); return reference::set(v); }

  reference set_bool( bool v )
    { init(); return reference::set_bool(v); }

  reference set_int( int v )
    { init(); return reference::set_int(v); }
  reference set_uint( unsigned v )
    { init(); return reference::set_uint(v); }
  reference set_int64( int64 v )
    { init(); return reference::set_int64(v); }
  reference set_uint64(uint64 v)
    { init(); return reference::set_uint64(v); }

  reference set_double(double v)
    { init(); return cci_value_ref::set_double(v); }

  using const_reference::json_serialize;
  bool json_deserialize( std::string const & src )
    { init(); return reference::json_deserialize( src ); }

  static cci_value from_json( std::string const & json );
  static std::string to_json( const_reference v );

private:
  impl* init();
  impl* do_init();

  impl* own_pimpl_;
};

template<typename T>
cci_value::cci_value(T const & v)
  : cci_value_ref(), own_pimpl_()
{
  do_init();
  set(v);
}

inline
cci_value::cci_value( const cci_value & that )
  : reference(), own_pimpl_()
{
  *this = that;
}

inline
cci_value::cci_value( const_reference that )
  : reference(), own_pimpl_()
{
  *this = that;
}

inline cci_value::reference
cci_value::operator=( cci_value const & that )
{
  return operator=( const_reference(that) );
}

inline cci_value::impl*
cci_value::init()
{
  if( !pimpl_ )
    pimpl_ = do_init();
  return pimpl_;
}

inline cci_value
cci_value::from_json( std::string const & json )
{
  cci_value v;
  bool ok = v.json_deserialize( json );
  sc_assert( ok );
  return v;
}

inline std::string
cci_value::to_json( const_reference v )
{
  std::string json;
  bool ok = v.json_serialize( json );
  sc_assert( ok );
  return json;
}

// --------------------------------------------------------------------------

CCI_CLOSE_CONFIG_NAMESPACE_

#endif // CCI_CNF_CCI_VALUE_H_INCLUDED_
