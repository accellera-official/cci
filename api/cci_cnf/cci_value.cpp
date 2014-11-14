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

/**
 * @file   cci_value.cpp
 * @author Philipp A. Hartmann, OFFIS
 */

#include "cci_cnf/cci_value.h"
#include "cci_cnf/cci_report_handler.h"
#include "cci_cnf/rapidjson.h"

#include <algorithm> // std::swap

CCI_OPEN_CONFIG_NAMESPACE_

typedef rapidjson::CrtAllocator allocator_type;
typedef rapidjson::UTF8<>       encoding_type;
typedef rapidjson::GenericValue<encoding_type, allocator_type>    json_value;
typedef rapidjson::GenericDocument<encoding_type, allocator_type> json_document;

static allocator_type json_allocator;

// wrapper implementation is simply a RapidJSON value
typedef json_value impl_type;
// helper to convert to wrapper implementation
static inline impl_type* impl_cast(void* p)
  { return static_cast<impl_type*>(p); }

#define PIMPL( x ) \
  (impl_cast((x).pimpl_))

#define DEREF( x ) \
  (*PIMPL(x))

#define THIS \
  (PIMPL(*this))

#define VALUE_ASSERT( Cond, Msg ) \
  do { if( !(Cond) ) \
      report_error( Msg "\n Condition: " #Cond, __FILE__, __LINE__ ); \
  } while( false )

// ----------------------------------------------------------------------------
// cci_value_cref

void
cci_value_cref::report_error( const char* msg, const char* file, int line ) const
{
    cci_report_handler::cci_value_failure( msg, file, line );
}

bool
operator == ( cci_value_cref const & left, cci_value_cref const & right )
{
  if( PIMPL(left) == PIMPL(right) )
    return true;

  if( !PIMPL(left) || !PIMPL(right) )
    return false;

  return DEREF(left) == DEREF(right);
}

basic_param_type
cci_value_cref::basic_type() const
{
  if( !THIS )
    return param_type_not_available;

  switch(THIS->GetType())
  {
  case rapidjson::kFalseType:
  case rapidjson::kTrueType:
    return param_type_bool;

  case rapidjson::kNumberType:
    return THIS->IsDouble() ? param_type_real : param_type_number;

  case rapidjson::kStringType:
    return param_type_string;

  case rapidjson::kArrayType:
    return param_type_list;

  case rapidjson::kObjectType:
    return param_type_other;

  case rapidjson::kNullType:
  default:
    return param_type_not_available;
  }
}

bool cci_value_cref::is_null() const
  { return !THIS || THIS->IsNull(); }

bool cci_value_cref::is_bool() const
  { return THIS && THIS->IsBool(); }

bool cci_value_cref::is_int() const
  { return THIS && THIS->IsInt(); }

bool cci_value_cref::is_int64() const
  { return THIS && THIS->IsInt64(); }

bool cci_value_cref::is_uint() const
  { return THIS && THIS->IsUint(); }

bool cci_value_cref::is_uint64() const
  { return THIS && THIS->IsUint64(); }

bool cci_value_cref::is_double() const
  { return THIS && THIS->IsDouble(); }

bool cci_value_cref::is_string() const
  { return THIS && THIS->IsString(); }

bool cci_value_cref::is_list() const
  { return THIS && THIS->IsArray(); }

bool cci_value_cref::is_map() const
  { return THIS && THIS->IsObject(); }

bool cci_value_cref::get_bool() const
{
  (is_bool());
  return THIS->GetBool();
}

#define ASSERT_TYPE( Cond ) \
  VALUE_ASSERT( Cond, "invalid type access" )

int cci_value_cref::get_int() const
{
  ASSERT_TYPE(is_int());
  return THIS->GetInt();
}

unsigned cci_value_cref::get_uint() const
{
  ASSERT_TYPE(is_uint());
  return THIS->GetUint();
}

int64 cci_value_cref::get_int64() const
{
  ASSERT_TYPE(is_int64());
  return THIS->GetInt64();
}

uint64 cci_value_cref::get_uint64() const
{
  ASSERT_TYPE(is_uint64());
  return THIS->GetUint64();
}

double cci_value_cref::get_double() const
{
  ASSERT_TYPE(is_double());
  return THIS->GetDouble();
}

cci_value_string_cref cci_value_cref::get_string() const
{
  ASSERT_TYPE(is_string());
  return cci_value_string_cref(pimpl_);
}

cci_value_list_cref cci_value_cref::get_list() const
{
  ASSERT_TYPE(is_list());
  return cci_value_list_cref(pimpl_);
}

cci_value_map_cref cci_value_cref::get_map() const
{
  ASSERT_TYPE(is_map());
  return cci_value_map_cref(pimpl_);
}

// ----------------------------------------------------------------------------
// cci_value_ref

void
cci_value_ref::swap( cci_value_ref& that )
{
  VALUE_ASSERT( pimpl_ && that.pimpl_, "swap with invalid value failed" );
  THIS->Swap( DEREF(that) );
}

cci_value_ref
cci_value_ref::operator=( cci_value_cref const& that )
{
  if( that.is_null() )
    set_null();
  else {
    sc_assert( THIS );
    THIS->CopyFrom( DEREF(that), json_allocator );
  }
  return *this;
}

cci_value_ref
cci_value_ref::set_null()
{
  if( THIS ) THIS->SetNull();
  return this_type( THIS );
}

cci_value_ref
cci_value_ref::set_bool(bool v)
{
  sc_assert( THIS );
  THIS->SetBool(v);
  return this_type( THIS );
}

cci_value_ref
cci_value_ref::set_int(int v)
{
  sc_assert( THIS );
  THIS->SetInt(v);
  return this_type( THIS );
}

cci_value_ref
cci_value_ref::set_uint(unsigned v)
{
  sc_assert( THIS );
  THIS->SetUint(v);
  return this_type( THIS );
}

cci_value_ref
cci_value_ref::set_int64(int64 v)
{
  sc_assert( THIS );
  THIS->SetInt(v);
  return this_type( THIS );
}

cci_value_ref
cci_value_ref::set_uint64(uint64 v)
{
  sc_assert( THIS );
  THIS->SetUint64(v);
  return this_type( THIS );
}

cci_value_ref
cci_value_ref::set_double(double d)
{
  sc_assert( THIS );
  THIS->SetDouble(d);
  return this_type( THIS );
}

cci_value_string_ref
cci_value_ref::set_string( const char* s, size_t len )
{
  sc_assert( THIS );
  THIS->SetString(s, len, json_allocator);
  return cci_value_string_ref(THIS);
}

cci_value_list_ref
cci_value_ref::set_list()
{
  sc_assert( THIS );
  THIS->SetArray();
  return cci_value_list_ref( THIS );
}

cci_value_map_ref
cci_value_ref::set_map()
{
  sc_assert( THIS );
  THIS->SetObject();
  return cci_value_map_ref( THIS );
}

// ----------------------------------------------------------------------------
// cci_value_string_cref

cci_value_string_cref::size_type
cci_value_string_cref::size() const
  { return THIS->GetStringLength(); }

const char*
cci_value_string_cref::c_str() const
  { return THIS->GetString(); }

bool operator==( cci_value_string_cref const & left, const char * right )
  { return !right ? false : DEREF(left) == right; }
bool operator==( cci_value_string_cref const & left, std::string const & right )
  { return DEREF(left) == rapidjson::StringRef( right.c_str(), right.size() ); }
bool operator==( const char * left, cci_value_string_cref const & right )
  { return !left ? false : DEREF(right) == left; }
bool operator==( std::string const & left, cci_value_string_cref const & right )
  { return DEREF(right) == rapidjson::StringRef( left.c_str(), left.size() ); }

// ----------------------------------------------------------------------------
// cci_value_string_ref

void
cci_value_string_ref::swap(this_type & that)
{
  VALUE_ASSERT( pimpl_ && that.pimpl_, "swap with invalid value failed" );
  THIS->Swap( DEREF(that) );
}

// ----------------------------------------------------------------------------
// cci_value_list_cref

cci_value_list_cref::size_type
cci_value_list_cref::size() const
  { return THIS->Size(); }

cci_value_cref
cci_value_list_cref::operator[]( size_type index ) const
  { return cci_value_cref( &(*THIS)[index] ); }

// ----------------------------------------------------------------------------
// cci_value_list_ref

void
cci_value_list_ref::swap(this_type & that)
{
  VALUE_ASSERT( pimpl_ && that.pimpl_, "swap with invalid value failed" );
  THIS->Swap( DEREF(that) );
}

cci_value_list_ref
cci_value_list_ref::clear()
{
  THIS->Clear();
  return *this;
}

cci_value_list_ref
cci_value_list_ref::push_back( const_reference value )
{
  json_value v;
  if( PIMPL(value) )
    v.CopyFrom( DEREF(value), json_allocator );
  THIS->PushBack( v, json_allocator );
  return *this;
}

// ----------------------------------------------------------------------------
// cci_value_map_cref

cci_value_map_cref::size_type
cci_value_map_cref::size() const
  { return THIS->MemberCount(); }

cci_value_cref::impl*
cci_value_map_cref::do_lookup( const char* key, size_type keylen
                             , bool allow_fail /* = false */     ) const
{
  json_value kv( rapidjson::StringRef(key, keylen) );
  json_value::ConstMemberIterator it = THIS->FindMember(kv);
  if( it == THIS->MemberEnd() )
  {
    if( allow_fail ) return NULL;

    std::stringstream ss;
    ss << "cci_value map has no element with key '" << key << "'";
    report_error( ss.str().c_str(), __FILE__, __LINE__ );
    return NULL;
  }
  return const_cast<json_value*>(&it->value);
}

// ----------------------------------------------------------------------------
// cci_value_map_ref

void
cci_value_map_ref::swap(this_type & that)
{
  VALUE_ASSERT( pimpl_ && that.pimpl_, "swap with invalid value failed" );
  THIS->Swap( DEREF(that) );
}

cci_value_map_ref
cci_value_map_ref::clear()
{
  THIS->RemoveAllMembers();
  return *this;
}

cci_value_map_ref
cci_value_map_ref::push_entry( const char * key
                             , cci_value::const_reference const & value )
{
  json_value k( key, json_allocator );
  json_value v;
  if( PIMPL(value) )
    v.CopyFrom( DEREF(value), json_allocator );
  THIS->AddMember( k, v, json_allocator );
  return *this;
}

// ----------------------------------------------------------------------------
// cci_value(, _list, _map ) -- owning wrapper implementations

#define WRAPPER_ASSIGN_PRECOND_(Kind) \
  WRAPPER_ASSIGN_PRECOND_FOR_ ## Kind
#define WRAPPER_ASSIGN_PRECOND_FOR_cci_value \
    if( that.is_null() ) { set_null(); return *this; } \
    init()
#define WRAPPER_ASSIGN_PRECOND_FOR_cci_value_list \
  sc_assert( is_list() && that.is_list() )
#define WRAPPER_ASSIGN_PRECOND_FOR_cci_value_map \
  sc_assert( is_map() && that.is_map() )

#define WRAPPER_DO_INIT_(Kind) \
  WRAPPER_DO_INIT_ ## Kind
#define WRAPPER_DO_INIT_cci_value \
  ((void)0)
#define WRAPPER_DO_INIT_cci_value_list \
  (THIS)->SetArray()
#define WRAPPER_DO_INIT_cci_value_map \
  (THIS)->SetObject()

#define DEFINE_WRAPPER_(Kind)                          \
  Kind::this_type &                                    \
  Kind::operator=( const_reference that )              \
  {                                                    \
    WRAPPER_ASSIGN_PRECOND_( Kind );                   \
    reference::operator=( that );                      \
    return *this;                                      \
  }                                                    \
                                                       \
  void                                                 \
  Kind::swap( this_type & that )                       \
  {                                                    \
    using std::swap;                                   \
    swap( pimpl_    , that.pimpl_ );                   \
    swap( own_pimpl_, that.own_pimpl_ );               \
  }                                                    \
                                                       \
  Kind::impl*                                          \
  Kind::do_init()                                      \
  {                                                    \
    sc_assert( !own_pimpl_ );                          \
    pimpl_ = own_pimpl_ = new impl_type();             \
    WRAPPER_DO_INIT_( Kind );                          \
    return THIS;                                       \
  }                                                    \
                                                       \
  Kind::~Kind()                                        \
  {                                                    \
    delete impl_cast(own_pimpl_);                      \
  }

DEFINE_WRAPPER_(cci_value)
DEFINE_WRAPPER_(cci_value_list)
DEFINE_WRAPPER_(cci_value_map)

// ----------------------------------------------------------------------------
// JSON (de)serialize

bool
cci_value_cref::json_serialize( std::string & dst ) const
{
  rapidjson::StringBuffer buf;
  rapidjson::Writer<rapidjson::StringBuffer> writer( buf );
  if( !THIS ) {
    writer.Null();
  } else {
    THIS->Accept( writer );
  }
  VALUE_ASSERT( writer.IsComplete(), "incomplete JSON sequence" );
  dst.assign( buf.GetString(), buf.GetSize() );
  return true;
}

bool
cci_value_ref::json_deserialize( std::string const & src )
{
  json_document doc;
  doc.Parse( src.c_str() );
  if( doc.GetParseError() ) {
    ///@todo add warning?
    return false;
  }
  THIS->Swap( doc );
  return true;
}

// ----------------------------------------------------------------------------
// Borrowing this .cpp to provide a home for this value (creating cci_param.cpp
// solely for that purpose is overkill. @TODO - review home later in rework
const char* PARAM_ORIGINATOR = "owning_param";

CCI_CLOSE_CONFIG_NAMESPACE_
