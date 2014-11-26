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

#include "cci_cnf/cci_value_traits.h"

/**
 * @file   cci_value_traits.cpp
 * @brief  conversions from and to a @ref cci_value (common types)
 * @author Philipp A. Hartmann, OFFIS
 */

CCI_OPEN_CONFIG_NAMESPACE_

#define DEFINE_PACK_( Type )                                                  \
  template<> bool                                                             \
  cci_value_traits<Type>::pack( cci_value::reference dst, type const & src )

#define DEFINE_UNPACK_(Type)                                                  \
  template<> bool                                                             \
  cci_value_traits<Type>::unpack( type & dst, cci_value::const_reference src )

// ----------------------------------------------------------------------------
// C++ builtin types

#define DEFINE_BUILTIN_( Type, Alias )                                        \
  DEFINE_PACK_(Type)                                                          \
  {                                                                           \
    dst.set_ ## Alias( src );                                                 \
    return true;                                                              \
  }                                                                           \
  DEFINE_UNPACK_(Type)                                                        \
  {                                                                           \
    if( !src.is_ ## Alias() ) return false;                                   \
    dst = src.get_ ## Alias();                                                \
    return true;                                                              \
  }

DEFINE_BUILTIN_(bool,     bool)
DEFINE_BUILTIN_(int,      int)
DEFINE_BUILTIN_(int64,    int64)
DEFINE_BUILTIN_(unsigned, uint)
DEFINE_BUILTIN_(uint64,   uint64)
DEFINE_BUILTIN_(double,   double)

DEFINE_BUILTIN_(std::string, string)

// ----------------------------------------------------------------------------
// SystemC builtin types

DEFINE_BUILTIN_( sc_dt::sc_uint_base, uint64 )
DEFINE_BUILTIN_( sc_dt::sc_int_base,  int64 )

// sc_signed
// sc_unsigned
// sc_bv_base
// sc_lv_base
// ...

CCI_CLOSE_CONFIG_NAMESPACE_
