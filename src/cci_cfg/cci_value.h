/*****************************************************************************
  Copyright 2006-2015 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2006-2014 OFFIS Institute for Information Technology
  All rights reserved.

  Copyright 2006-2015 Intel Corporation
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

#ifndef CCI_CCI_VALUE_H_INCLUDED_
#define CCI_CCI_VALUE_H_INCLUDED_

#include "cci_cfg/cci_datatypes.h"
#include "cci_core/systemc.h" // sc_dt::(u)int64, potentially strip out

#include <cstring> // std::strlen

/**
 * @file   cci_value.h
 * @author Philipp A. Hartmann, OFFIS/Intel
 */

CCI_OPEN_NAMESPACE_

// define our own typedefs to avoid SystemC dependency?
using sc_dt::int64;
using sc_dt::uint64;

// forward declarations
class cci_value;
class cci_value_cref;
class cci_value_ref;
class cci_value_string_cref;
class cci_value_string_ref;
class cci_value_list;
class cci_value_list_cref;
class cci_value_list_ref;
class cci_value_map;
class cci_value_map_cref;
class cci_value_map_ref;

template<typename T> struct cci_value_traits;

///@cond CCI_HIDDEN_FROM_DOXYGEN
#define CCI_VALUE_TRAITS_(Type) \
  typename cci_value_traits<Type>::type
#define CCI_VALUE_TRAITS_ENABLED_(Type) \
  CCI_VALUE_TRAITS_(Type) *
#define CCI_VALUE_ENABLE_IF_TRAITS_(Type) \
  CCI_VALUE_TRAITS_ENABLED_(Type) = 0
///@endcond CCI_HIDDEN_FROM_DOXYGEN

/// @ref cci_value comparisons
bool operator==( cci_value_cref const &, cci_value_cref const & );

/// @ref cci_value ostream insertion
std::ostream& operator<<( std::ostream&, cci_value_cref const & );

/// @ref cci_value istream extraction
std::istream& operator>>( std::istream&, cci_value_ref );

// --------------------------------------------------------------------------

/// reference to a constant (nested) @ref cci_value
class cci_value_cref
{
  friend class cci_value_ref;
  friend class cci_value_list_cref;
  friend class cci_value_list_ref;
  friend class cci_value_map_cref;
  friend class cci_value_map_ref;
  friend bool operator==( cci_value_cref const &, cci_value_cref const & );
  friend std::ostream& operator<<( std::ostream&, cci_value_cref const & );

protected:
  typedef void impl; // use type-punned pointer for now

  explicit cci_value_cref(impl* i = NULL)
    : pimpl_(i) {}

public:

  /** @name type queries */
  ///@{
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

  /** @name value queries */
  //@{

  /// get boolean value
  bool get_bool()    const;

  /// get integer value
  int      get_int()     const;
  /// get unsigned integer value
  unsigned get_uint()    const;
  /// get 64-bit integer value
  int64    get_int64()   const;
  /// get 64-bit unsigned integer value
  uint64   get_uint64()  const;
  /// get floating point value
  double   get_double()  const;
  /// get the numeric value (alias for get_double())
  double   get_number()  const { return get_double(); }
  //@}

  /**
   * @name complex value queries
   * These functions return (constant) references to the complex value types
   * that can be stored in a cci_value (strings, lists, maps).
   */
  //@{
  cci_value_string_cref get_string() const;
  cci_value_list_cref   get_list() const;
  cci_value_map_cref    get_map()  const;
  //@}

  /** @name arbitrary type value queries */
  //@{
  /// try to get a value of a @ref cci_value_traits enabled type
  template<typename T>
  bool try_get( T& dst, CCI_VALUE_ENABLE_IF_TRAITS_(T) ) const;
  /// get a value of a @ref cci_value_traits enabled type
  template<typename T>
  CCI_VALUE_TRAITS_(T) get() const;
  //@}

  /// convert value to JSON
  bool json_serialize( std::string& ) const;

protected:
  void
  report_error( const char* msg
              , const char* file = NULL, int line = 0 ) const;

protected:
  impl* pimpl_;

private:
  /// constant reference, disabled assignment
  cci_value_cref operator=( cci_value_cref const& ) /* = delete */;
};

template<typename T>
bool
cci_value_cref::try_get( T& dst, CCI_VALUE_TRAITS_ENABLED_(T) ) const
{
  typedef cci_value_traits<T> traits;
  return traits::unpack( dst, *this );
}

template<typename T>
CCI_VALUE_TRAITS_(T) cci_value_cref::get() const
{
  T result;
  if( !try_get(result) ) {
    report_error("conversion from cci_value failed", __FILE__, __LINE__);
  }
  return result;
}

// --------------------------------------------------------------------------

/// reference to a mutable (nested) @ref cci_value
class cci_value_ref
  : public cci_value_cref
{
  friend class cci_value_string_ref;
  friend class cci_value_list_ref;
  friend class cci_value_map_ref;
  friend std::istream& operator>>( std::istream&, cci_value_ref );
  typedef cci_value_cref base_type;
  typedef cci_value_ref  this_type;

protected:
  explicit cci_value_ref(impl* i = NULL)
    : cci_value_cref(i) {}

public:
  /// exchange contents with another cci_value
  void swap( this_type& that );
  this_type operator=( const base_type& );
  this_type operator=( const this_type& );

  /** @name set value functions
   *
   * The various @c set_* functions update the represented value
   * (and its @ref basic_type()) accordingly.
   *
   * These functions provide a "fluent interface" by returning a reference
   * to the surrounding variant object.  This enables chained function calls,
   * which is especially convenient for complex values like lists and maps.
   */
  ///@{

  /// set value to cci_value_traits enabled type
  template<typename T>
  cci_value_ref set( T const & dst, CCI_VALUE_ENABLE_IF_TRAITS_(T) );
  /// try to set value to cci_value_traits enabled type
  template<typename T>
  bool          try_set( T const & dst, CCI_VALUE_ENABLE_IF_TRAITS_(T) );

  /// set value to @c null
  cci_value_ref set_null();
  /// set boolean value (@c true, @c false)
  cci_value_ref set_bool( bool v );
  /// set integer value
  cci_value_ref set_int( int v );
  /// set unsigned integer value
  cci_value_ref set_uint( unsigned v );
  /// set 64-bit integer value
  cci_value_ref set_int64( int64 v );
  /// set unsigned 64-bit integer value
  cci_value_ref set_uint64( uint64 v );
  /// set double value
  cci_value_ref set_double( double v );
  /// set double value (alias for set_double())
  cci_value_ref set_number( double v ) { return set_double(v); }

  /// set string value from null-terminated string
  cci_value_string_ref set_string( const char* s );
  /// set string value from std::string
  cci_value_string_ref set_string( const std::string& s );
  /// set string value from cci_value::const_string_reference
  cci_value_string_ref set_string( cci_value_string_cref s );
  /// set string value from null-terminated string and length
  cci_value_string_ref set_string( const char* s, size_t len );

  /// set value to an (empty) list
  cci_value_list_ref   set_list();
  /// set value to an (empty) map
  cci_value_map_ref    set_map();
  ///@}

  /** @name complex value queries
   *
   * The functions return a reference to a (mutable) @ref cci_value object
   * of the corresponding complex value type (string, list, map).
   */
  ///@{
  using base_type::get_string;
  cci_value_string_ref get_string();

  using base_type::get_list;
  cci_value_list_ref get_list();

  using base_type::get_map;
  cci_value_map_ref  get_map();
  ///@}

  /// try to set the value from a JSON-encoded string
  bool json_deserialize( std::string const& );
};

inline cci_value_ref
cci_value_ref::operator=( this_type const & that )
  { return *this = base_type(that); }

template<typename T>
bool
cci_value_ref::try_set( T const & src, CCI_VALUE_TRAITS_ENABLED_(T) )
{
  typedef cci_value_traits<T> traits;
  return traits::pack( *this, src );
}

template<typename T>
cci_value_ref
cci_value_ref::set( T const& src, CCI_VALUE_TRAITS_ENABLED_(T) )
{
  if( !try_set(src) ) {
    report_error("conversion to cci_value failed", __FILE__, __LINE__);
  }
  return *this;
}

// --------------------------------------------------------------------------

/** @name cci_value string comparisons */
///@{
bool operator==( const char *, cci_value_string_cref const & );
bool operator==( cci_value_string_cref const &, const char * );
bool operator==( std::string const &, cci_value_string_cref const & );
bool operator==( cci_value_string_cref const &, std::string const & );
///@}

/// reference to constant cci_value string value
class cci_value_string_cref
  : public cci_value_cref
{
  friend class cci_value_cref;
  friend bool operator==( const char *, cci_value_string_cref const & );
  friend bool operator==( cci_value_string_cref const &, const char * );
  friend bool operator==( std::string const &, cci_value_string_cref const & );
  friend bool operator==( cci_value_string_cref const &, std::string const & );
  typedef cci_value_cref        base_type;
  typedef cci_value_string_cref this_type;

protected:
  explicit cci_value_string_cref(impl* i = NULL)
    : base_type(i) {}

public:
  typedef size_t size_type;

  /// empty string?
  bool      empty()  const { return size() == 0;  }
  size_type length() const { return size(); }
  size_type size()   const;

  /// implicit conversion to std::string
  operator std::string() const { return std::string( c_str(), length() ); }
  /// obtain underlying null-terminated string
  char const * c_str()   const;

  /// character access by index
  char operator[]( size_type index ) const
    { return c_str()[index]; }

private:
  // exclude non-string value functions
  using base_type::get_bool;
  using base_type::get_int;
  using base_type::get_uint;
  using base_type::get_int64;
  using base_type::get_uint64;
  using base_type::get_double;
  using base_type::get_list;
  using base_type::get_map;

private:
  // constant reference, no assignment
  this_type& operator=( this_type const& ) /* = delete */;
};

/// specialisation for printing a plain string reference (not JSON encoded)
inline std::ostream&
operator<<( std::ostream& os, cci_value_string_cref const& s )
{
  os.write( s.c_str(), static_cast<std::streamsize>(s.length()) );
  return os;
}

// --------------------------------------------------------------------------

/// reference to mutable cci_value string value
class cci_value_string_ref
  : public cci_value_string_cref
{
  friend class cci_value_ref;
  typedef cci_value_string_cref base_type;
  typedef cci_value_string_ref  this_type;

protected:
  explicit cci_value_string_ref(impl* i = NULL)
    : base_type(i) {}

public:
  /// exchange contents with another string value
  void swap( this_type& that );

  /** @name assign string contents */
  //@{
  this_type operator=( this_type const& );
  this_type operator=( cci_value_string_cref s );
  this_type operator=( const char * s );
  this_type operator=( std::string const & s );
  //@}
};

inline cci_value_string_ref
cci_value_string_ref::operator=( this_type const& s )
  { return cci_value_ref(pimpl_).set_string(s); }

inline cci_value_string_ref
cci_value_string_ref::operator=( cci_value_string_cref s )
  { return cci_value_ref(pimpl_).set_string(s); }

inline cci_value_string_ref
cci_value_string_ref::operator=( const char * s )
  { return cci_value_ref(pimpl_).set_string(s); }

inline cci_value_string_ref
cci_value_string_ref::operator=( std::string const & s )
  { return cci_value_ref(pimpl_).set_string(s); }

inline cci_value_string_ref
cci_value_ref::get_string()
  { return cci_value_string_ref( base_type::get_string().pimpl_ ); }

inline cci_value_string_ref
cci_value_ref::set_string(char const * s)
  { return set_string( s ? s : "", s ? std::strlen(s) : 0u ); }

inline cci_value_string_ref
cci_value_ref::set_string(cci_value_string_cref s)
  { return set_string( s.c_str(), s.length() ); }

inline cci_value_string_ref
cci_value_ref::set_string(std::string const & s)
  { return set_string( s.c_str(), s.length() ); }

// --------------------------------------------------------------------------

/// reference to constant cci_value list value
class cci_value_list_cref
  : public cci_value_cref
{
  friend class cci_value_cref;
  typedef cci_value_cref      base_type;
  typedef cci_value_list_cref this_type;

protected:
  explicit cci_value_list_cref(impl* i = NULL)
    : base_type(i) {}

public:
  typedef size_t         size_type;
  typedef cci_value_cref const_reference;
  typedef cci_value_ref  reference;

  /** @name list queries */
  //@{
  bool      empty() const { return size() == 0;  }
  size_type size()  const;
  //@}

  /** @name (constant) element access by index */
  //@{
  const_reference operator[]( size_type index ) const;
  const_reference at( size_type index ) const
    { return (*this)[index]; }
  //@}

private:
  // exclude non-list value functions
  using base_type::get_bool;
  using base_type::get_int;
  using base_type::get_uint;
  using base_type::get_int64;
  using base_type::get_uint64;
  using base_type::get_double;
  using base_type::get_string;
  using base_type::get_map;

private:
  // constant reference, no assignment
  this_type& operator=( this_type const& ) /* = delete */;
};

// --------------------------------------------------------------------------

/// reference to mutable cci_value list value
class cci_value_list_ref
  : public cci_value_list_cref
{
  friend class cci_value_ref;
  typedef cci_value_list_cref base_type;
  typedef cci_value_list_ref  this_type;

protected:
  explicit cci_value_list_ref(impl* i = NULL)
    : base_type(i) {}

public:
  this_type operator=( this_type const& );
  this_type operator=( base_type const& );
  /// exchange contents with another list value
  void swap( this_type& );

  /// clear list elements
  this_type clear();

  /** @name (mutable) element access by index */
  //@{
  using base_type::operator[];
  reference operator[]( size_type index );

  using base_type::at;
  reference at( size_type index )
    { return (*this)[index]; }
  //@}

  /// capacity of underlying storage
  size_type capacity() const;
  /// reserve space in underlying storage
  this_type reserve( size_type );

  /** @name push new elements to the end of the list */
  //@{

  /// append value obtained from a constant cci_value reference
  this_type push_back( const_reference v );
  /// append arbitrary cci_value_traits enabled value
  template<typename T>
  this_type push_back( const T & v, CCI_VALUE_ENABLE_IF_TRAITS_(T) );

  //@}

  // TODO: add iterator interface
};

inline cci_value_list_ref
cci_value_list_ref::operator=( this_type const & that )
  { return *this = base_type(that); }

inline cci_value_list_ref
cci_value_list_ref::operator=( base_type const & that )
  { cci_value_ref v(pimpl_); v = that; return *this; }

inline cci_value_list_ref::reference
cci_value_list_ref::operator[]( size_type index )
  { return reference( base_type::operator[](index).pimpl_ ); }

inline cci_value_list_ref
cci_value_ref::get_list()
  { return cci_value_list_ref( base_type::get_list().pimpl_ ); }

// --------------------------------------------------------------------------

/// reference to constant cci_value map
class cci_value_map_cref
  : public cci_value_cref
{
  friend class cci_value_cref;
  typedef cci_value_cref     base_type;
  typedef cci_value_map_cref this_type;

protected:
  explicit cci_value_map_cref(impl* i = NULL)
    : base_type(i) {}

public:
  typedef size_t         size_type;
  typedef cci_value_cref const_reference;
  typedef cci_value_ref  reference;

  /** @name map queries */
  //@{
  bool      empty()    const { return size() == 0;  }
  size_type size()     const;
  //@}

  /** @name map element queries
   * Check for the existence of an entry with a given key
   */
  //@{
  bool has_entry( const char * key ) const
    { return NULL != do_lookup( key, std::strlen(key)
                              , /* allow_fail = */ true ); }
  bool has_entry( cci_value_string_cref key ) const
    { return NULL != do_lookup( key.c_str(), key.length()
                              , /* allow_fail = */ true ); }
  bool has_entry( std::string const & key ) const
    { return NULL != do_lookup( key.c_str(), key.length()
                              , /* allow_fail = */ true ); }
  //@}

  /** @name map element access
   * Accessing an entry with a given key
   */
  //@{
  const_reference operator[]( const char* key ) const
    { return const_reference( do_lookup( key, std::strlen(key) ) ); }
  const_reference operator[]( cci_value_string_cref key ) const
    { return const_reference( do_lookup( key.c_str(), key.length() ) ); }
  const_reference operator[]( std::string const& key ) const
    { return const_reference( do_lookup( key.c_str(), key.length() ) ); }
  ///@}

  // TODO: add iterator interface

protected:
  impl * do_lookup( const char* key, size_type keylen
                  , bool allow_fail = false           ) const;

private:
  // exclude non-map value functions
  using base_type::get_bool;
  using base_type::get_int;
  using base_type::get_uint;
  using base_type::get_int64;
  using base_type::get_uint64;
  using base_type::get_double;
  using base_type::get_string;
  using base_type::get_list;

private:
  // constant reference, no assignment
  this_type& operator=( this_type const& ) /* = delete */;
};

// --------------------------------------------------------------------------

/// reference to mutable cci_value map
class cci_value_map_ref
  : public cci_value_map_cref
{
  friend class cci_value_ref;
  typedef cci_value_map_cref base_type;
  typedef cci_value_map_ref  this_type;
protected:
  explicit cci_value_map_ref(impl* i = NULL)
    : base_type(i) {}

public:

  this_type operator=( base_type const& );
  this_type operator=( this_type const& );
  void swap( this_type& );

  this_type clear();

  /** @name map element access */
  ///@{
  using base_type::operator[];
  reference operator[]( const char* key )
    { return reference( do_lookup( key, std::strlen(key) ) ); }
  reference operator[]( std::string const& key )
    { return reference( do_lookup( key.c_str(), key.length() ) ); }
  ///@}

  ///@name map element addition
  ///@{
  /// add value obtained from a constant cci_value reference
  this_type push_entry( const char* key, const_reference const & value );
  /// add value obtained from a constant cci_value reference
  this_type push_entry( std::string const& key, const_reference const & value )
    { return push_entry( key.c_str(), value ); }

  /// add an arbitrary cci_value_traits enabled value
  template<typename T>
  this_type push_entry( const char* key, const T & value
                      , CCI_VALUE_ENABLE_IF_TRAITS_(T) );
  /// add an arbitrary cci_value_traits enabled value
  template<typename T>
  this_type push_entry( std::string const & key, const T & value
                      , CCI_VALUE_ENABLE_IF_TRAITS_(T) )
    { return push_entry<T>( key.c_str(), value ); }
  ///@}

  // TODO: add iterator interface
};

inline cci_value_map_ref
cci_value_map_ref::operator=( this_type const & that )
  { return *this = base_type(that); }

inline cci_value_map_ref
cci_value_map_ref::operator=( base_type const & that )
  { cci_value_ref v(pimpl_); v = that; return *this; }

inline cci_value_map_ref
cci_value_ref::get_map()
  { return cci_value_map_ref( base_type::get_map().pimpl_ ); }

// --------------------------------------------------------------------------

/**
 * @brief generic variant type
 *
 * This class provides the first-class type for representing arbitrary values.
 * It can represent its values via a set of primitive types:
 *  * @c null (default)
 *  * boolean values (@c true, @c false)
 *  * numeric values (signed/unsigned integral or floating-point numbers)
 *  * strings
 *  * lists of values
 *  * maps of (key, value) pairs
 *
 * First-class objects of this class have strict value semantics, i.e. each
 * value represents a distinct object.  Due to the hierarchical nature of the
 * data structure, values embedded somewhere in a list or map are referenced
 * by dedicated reference objects (cci_value_cref, cci_value_ref, and their
 * specialized variants for strings, lists and maps), with or without constness.
 *
 * Users can add automatic conversions from/to cci_value objects by providing
 * an implementation (or specialisation) of the cci_value_traits class.
 * Corresponding specializations for the builtin types, the SystemC data types
 * and some freuquently used standard types are provided by default already.
 *
 * \see cci_value_list, cci_value_map, cci_value_traits
 */
class cci_value
  : public cci_value_ref
{
  typedef cci_value this_type;
public:
  /// reference to a constant value
  typedef cci_value_cref        const_reference;
  /// reference to a mutable value
  typedef cci_value_ref         reference;
  /// reference to a constant string value
  typedef cci_value_string_cref const_string_reference;
  /// reference to a mutable string value
  typedef cci_value_string_ref  string_reference;
  /// reference to a constant list value
  typedef cci_value_list_cref   const_list_reference;
  /// reference to a mutable list value
  typedef cci_value_list_ref    list_reference;
  /// reference to a constant map value
  typedef cci_value_map_cref    const_map_reference;
  /// reference to a mutable map value
  typedef cci_value_map_ref     map_reference;

  /// default constructor
  cci_value()
    : cci_value_ref(), own_pimpl_() {}

  /// constructor from basic type
  explicit
  cci_value( basic_param_type ); ///< @todo drop this?

  /// constructor from arbitrary cci_value_traits enabled value
  template<typename T>
  explicit
  cci_value( T const & src, CCI_VALUE_ENABLE_IF_TRAITS_(T) );

  cci_value( this_type const & that );
  cci_value( const_reference that );

  this_type& operator=( this_type const & );
  this_type& operator=( const_reference );

  void swap( cci_value & that );
  void swap( reference that )
    { init(); reference::swap( that ); }

  ~cci_value();

  /** @name set value functions
   * \see cci_value_ref
   */
  //@{
  /// set to arbitrary cci_value_traits enabled value
  template< typename T >
  reference  set( T const & v, CCI_VALUE_ENABLE_IF_TRAITS_(T) )
    { init(); return reference::set(v); }

  /// set boolean value
  reference set_bool( bool v )
    { init(); return reference::set_bool(v); }

  /// set integer value
  reference set_int( int v )
    { init(); return reference::set_int(v); }
  /// set unsigned integer value
  reference set_uint( unsigned v )
    { init(); return reference::set_uint(v); }
  /// set 64-bit integer value
  reference set_int64( int64 v )
    { init(); return reference::set_int64(v); }
  /// set unsigned 64-bit integer value
  reference set_uint64(uint64 v)
    { init(); return reference::set_uint64(v); }

  /// set floating-point value
  reference set_double(double v)
    { init(); return cci_value_ref::set_double(v); }

  string_reference set_string( const char* s )
    { init(); return reference::set_string(s); }
  string_reference set_string( const_string_reference s )
    { init(); return reference::set_string(s); }
  string_reference set_string( const std::string& s )
    { init(); return reference::set_string(s); }

  list_reference set_list()
    { init(); return cci_value_ref::set_list(); }

  map_reference set_map()
    { init(); return cci_value_ref::set_map(); }
  //@}

  /** @name JSON (de)serialization
   */
  using const_reference::json_serialize;
  bool json_deserialize( std::string const & src )
    { init(); return reference::json_deserialize( src ); }

  static cci_value from_json( std::string const & json );
  static std::string to_json( const_reference v );

  friend std::istream& operator>>( std::istream& is, this_type & v )
    { v.init(); return is >> reference(v); }
  //@}

private:
  impl* init();
  impl* do_init();

  impl* own_pimpl_;
};

template<typename T>
cci_value::cci_value( T const & v, CCI_VALUE_TRAITS_ENABLED_(T) )
  : cci_value_ref(), own_pimpl_()
{
  do_init();
  set(v);
}

inline
cci_value::cci_value( this_type const & that )
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

inline cci_value &
cci_value::operator=( this_type const & that )
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
// The following two functions depend on the completeness of the cci_value
// class, enforced by some compilers (e.g. Clang).

template<typename T>
cci_value_list_ref::this_type
cci_value_list_ref::push_back( const T& value, CCI_VALUE_TRAITS_ENABLED_(T) )
{
  cci_value v(value);
  return push_back( const_reference(v) );
}

template<typename T>
cci_value_map_ref
cci_value_map_ref::push_entry( const char* key, const T& value
                             , CCI_VALUE_TRAITS_ENABLED_(T) )
{
  cci_value v(value);
  return push_entry( key, const_reference(v) );
}

// --------------------------------------------------------------------------

/**
 * @brief list of cci_value values
 *
 * This class is equivalent to a cci_value after calling @c set_list().
 * @see cci_value, cci_value_list_cref, cci_value_list_ref
 */
class cci_value_list
  : public cci_value_list_ref
{
  typedef cci_value_list this_type;
public:
  typedef cci_value_list_cref const_reference;
  typedef cci_value_list_ref  reference;

  cci_value_list();

  cci_value_list( this_type const & );
  cci_value_list( const_reference );

  this_type& operator=( this_type const & );
  this_type& operator=( const_reference );
  void swap( this_type & );
  void swap( reference that )
    { reference::swap( that ); }

  ~cci_value_list();

private:
  impl* do_init();
  impl* own_pimpl_;
};

inline
cci_value_list::cci_value_list()
  : reference(), own_pimpl_()
{
  do_init();
}

inline
cci_value_list::cci_value_list( this_type const & that )
  : reference(), own_pimpl_()
{
  do_init();
  *this = that;
}

inline
cci_value_list::cci_value_list( const_reference that )
  : reference(), own_pimpl_()
{
  do_init();
  *this = that;
}

inline cci_value_list &
cci_value_list::operator=( this_type const & that )
{
  return operator=( const_reference(that) );
}

// --------------------------------------------------------------------------

/**
 * @brief map of (key, cci_value) pairs
 *
 * This class is equivalent to a cci_value after calling @c set_map().
 * @see cci_value, cci_value_map_cref, cci_value_map_ref
 */
class cci_value_map
  : public cci_value_map_ref
{
  typedef cci_value_map this_type;
public:
  typedef cci_value_map_cref const_reference;
  typedef cci_value_map_ref  reference;

  cci_value_map();

  cci_value_map( this_type const & );
  cci_value_map( const_reference );

  this_type& operator=( this_type const& );
  this_type& operator=( const_reference );
  void swap( this_type & );
  void swap( reference that )
    { reference::swap( that ); }

  ~cci_value_map();

private:
  impl* do_init();
  impl* own_pimpl_;
};

inline
cci_value_map::cci_value_map()
  : reference(), own_pimpl_()
{
  do_init();
}

inline
cci_value_map::cci_value_map( this_type const & that )
  : reference(), own_pimpl_()
{
  do_init();
  *this = that;
}

inline
cci_value_map::cci_value_map( const_reference that )
  : reference(), own_pimpl_()
{
  do_init();
  *this = that;
}

inline cci_value_map &
cci_value_map::operator=( this_type const & that )
{
  return operator=( const_reference(that) );
}

// --------------------------------------------------------------------------

CCI_CLOSE_NAMESPACE_

#undef CCI_VALUE_TRAITS_
#undef CCI_VALUE_TRAITS_ENABLED_
#undef CCI_VALUE_ENABLE_IF_TRAITS_

#endif // CCI_CCI_VALUE_H_INCLUDED_
