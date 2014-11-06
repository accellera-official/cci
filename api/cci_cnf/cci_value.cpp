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

#define DEREF( x ) \
  (*impl_cast((x).pimpl_))

#define THIS \
  (impl_cast(this->pimpl_))

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



// ----------------------------------------------------------------------------
// cci_value

cci_value::reference
cci_value::operator=( const_reference that )
{
  if( that.is_null() )
    return set_null();
  init();
  return reference::operator=( that );
}

void
cci_value::swap( cci_value & that )
{
  using std::swap;
  swap( pimpl_    , that.pimpl_ );
  swap( own_pimpl_, that.own_pimpl_ );
}

cci_value::impl*
cci_value::do_init()
{
  sc_assert( !own_pimpl_ );
  return pimpl_ = own_pimpl_ = new impl_type();
}

cci_value::~cci_value()
{
  delete impl_cast(own_pimpl_);
}

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
