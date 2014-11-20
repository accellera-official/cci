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
 * @file   rapidjson.h
 * @brief  internal include file for the RapidJSON library
 * @author Philipp A. Hartmann, OFFIS
 *
 * This file provides a local indirection to include RapidJSON from within
 * the CCI library.  RapidJSON is used for serialization from/to
 * @ref cci_value.
 *
 * @note This file is @b not part of the public API!
 */

#ifndef CCI_CNF_RAPIDJSON_H_INCLUDED_
#define CCI_CNF_RAPIDJSON_H_INCLUDED_

#include <stdexcept>

// --------------------------------------------------------------------------
// configure RapidJSON

#define RAPIDJSON_NAMESPACE \
  cci::cnf::rapidjson
#define RAPIDJSON_NAMESPACE_BEGIN \
  namespace cci { namespace cnf { namespace rapidjson {
#define RAPIDJSON_NAMESPACE_END \
  } } }

// enable support for std::string
#define RAPIDJSON_HAS_STD_STRING 1

RAPIDJSON_NAMESPACE_BEGIN

struct AssertException
  : std::logic_error
{
  AssertException( const char* msg )
    : std::logic_error(msg) {}
};

// throw on failing assertions
#define RAPIDJSON_ASSERT( expr ) \
  ((void)(( expr ) ? 0 : \
    ( throw ::RAPIDJSON_NAMESPACE::AssertException( #expr ), 0 )))

RAPIDJSON_NAMESPACE_END

#include "rapidjson/rapidjson.h"

#ifdef __GNUC__
RAPIDJSON_DIAG_PUSH
RAPIDJSON_DIAG_OFF( pedantic ) // ignore pedantic errors
#endif

// throw exception by default
#define RAPIDJSON_PARSE_ERROR_EARLY_RETURN( what ) \
  ((void)0)
#define RAPIDJSON_PARSE_ERROR_NORETURN(parseErrorCode,offset) \
  throw ::RAPIDJSON_NAMESPACE::ParseException( \
               ::RAPIDJSON_NAMESPACE::parseErrorCode, \
               #parseErrorCode,offset \
  )

#include "rapidjson/error/error.h"
#include "rapidjson/error/en.h"

RAPIDJSON_NAMESPACE_BEGIN

struct ParseException
  : std::runtime_error, ParseResult
{
  ParseException( ParseErrorCode code, const char* msg, size_t offset )
     : std::runtime_error(msg), ParseResult(code,offset) {}
};

RAPIDJSON_NAMESPACE_END

#include "rapidjson/document.h"
#include "rapidjson/writer.h"

#ifdef __GNUC__
RAPIDJSON_DIAG_POP
#endif

#endif // CCI_CNF_RAPIDJSON_H_INCLUDED_
