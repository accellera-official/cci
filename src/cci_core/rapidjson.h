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
 * @file   rapidjson.h
 * @brief  internal include file for the RapidJSON library
 * @author Philipp A. Hartmann, OFFIS/Intel
 *
 * This file provides a local indirection to include RapidJSON from within
 * the CCI library.  RapidJSON is used for serialization from/to
 * @ref cci_value.
 *
 * @note This file is @b not part of the public API!
 */

#ifndef CCI_RAPIDJSON_H_INCLUDED_
#define CCI_RAPIDJSON_H_INCLUDED_

///@cond CCI_HIDDEN_FROM_DOXYGEN

#include <stdexcept>
#include <iosfwd>

// --------------------------------------------------------------------------
// configure RapidJSON

#define RAPIDJSON_NAMESPACE \
  cci::rapidjson
#define RAPIDJSON_NAMESPACE_BEGIN \
  namespace cci { namespace rapidjson {
#define RAPIDJSON_NAMESPACE_END \
  } }

// enable support for std::string
#define RAPIDJSON_HAS_STD_STRING 1

// parse floating point numbers with full precision
#define RAPIDJSON_PARSE_DEFAULT_FLAGS \
  ::RAPIDJSON_NAMESPACE::kParseFullPrecisionFlag

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
#if CCI_CPLUSPLUS >= 201103L
RAPIDJSON_DIAG_OFF( terminate ) // ignore throwing assertions
#endif
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

// -----------------------------------------------------------------------
// std::basic_(i/o)stream wrapper

RAPIDJSON_NAMESPACE_BEGIN

template<typename Encoding>
class StdBasicInputStream {
public:
  typedef typename Encoding::Ch  Ch;  //!< character type
  typedef std::basic_istream<Ch> Str; //!< C++ stream type

  /*!
    \param is C++ input stream to wrap
  */
  StdBasicInputStream(Str& is) : str_(&is) {
    RAPIDJSON_ASSERT(str_->good());
  }

  Ch Peek() const {
    typename Str::int_type c = str_->peek();
    return (c == Str::traits_type::eof()) ? Ch() : static_cast<Ch>(c);
  }

  Ch Take() {
    typename Str::int_type c = str_->get();
    return (c == Str::traits_type::eof()) ? Ch() : static_cast<Ch>(c);
  }

  size_t Tell() const {
    return static_cast<size_t>(str_->tellg());
  }

  // Not yet implemented (needed for encoding detection)
  const Ch* Peek4() const { RAPIDJSON_ASSERT(false); return 0; }

  // Not implemented
  void   Put(Ch c)   { RAPIDJSON_ASSERT(false); }
  void   Flush()     { RAPIDJSON_ASSERT(false); }
  Ch*    PutBegin()  { RAPIDJSON_ASSERT(false); return 0; }
  size_t PutEnd(Ch*) { RAPIDJSON_ASSERT(false); return 0; }

private:
  Str* str_; //!< wrapped C++ character stream
};

typedef StdBasicInputStream<UTF8<> >  StdInputStream;

template<typename Encoding>
class StdBasicOutputStream {
public:
  typedef typename Encoding::Ch  Ch;  //!< character type
  typedef std::basic_ostream<Ch> Str; //!< C++ stream type

  /*!
    \param os C++ output stream to wrap
  */
  StdBasicOutputStream(Str& os) : str_(&os) {
    RAPIDJSON_ASSERT(str_->good());
  }

  void Put(Ch c) {
    str_->put(c);
  }

  void Flush() {
    str_->flush();
  }

  size_t Tell() const {
    return static_cast<size_t>(str_->tellp());
  }

  // Not implemented
  Ch     Peek() const { RAPIDJSON_ASSERT(false); return Ch(); }
  Ch     Take()       { RAPIDJSON_ASSERT(false); return Ch(); }
  Ch*    PutBegin()   { RAPIDJSON_ASSERT(false); return 0; }
  size_t PutEnd(Ch*)  { RAPIDJSON_ASSERT(false); return 0; }

private:
  Str* str_; //!< wrapped C++ character stream
};

typedef StdBasicOutputStream<UTF8<> > StdOutputStream;

RAPIDJSON_NAMESPACE_END

#ifdef __GNUC__
RAPIDJSON_DIAG_POP
#endif

///@endcond CCI_HIDDEN_FROM_DOXYGEN

#endif // CCI_RAPIDJSON_H_INCLUDED_
