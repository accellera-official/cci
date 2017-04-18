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
 * \file cci_cmnhdr.h
 * \brief Some platform helpers other implementation-defined parts
 *
 * @author Enrico Galli, Intel
 * @author Philipp A. Hartmann, Intel
 *
 * \note The contents of this file are entirely implementation-defined.
 */
#ifndef CCI_CORE_CCI_CONFIG_H_INCLUDED_
#define CCI_CORE_CCI_CONFIG_H_INCLUDED_

#ifndef CCI_NAMESPACE
/// Namespace for Accellera Configuration, Control & Inspection (CCI) standard
# define CCI_NAMESPACE cci
#endif // CCI_NS_

/// Opening a CCI namespace block (internal)
# define CCI_OPEN_NAMESPACE_  namespace CCI_NAMESPACE {
/// Closing a CCI namespace block (internal)
# define CCI_CLOSE_NAMESPACE_ } /* namespace cci */

/* ------------------------------------------------------------------------ */

// Selected C++ standard baseline, supported values are
//   199711L (C++03, ISO/IEC 14882:1998, 14882:2003)
//   201103L (C++11, ISO/IEC 14882:2011)
//   201402L (C++14, ISO/IEC 14882:2014)
#ifndef CCI_CPLUSPLUS
# ifdef SC_CPLUSPLUS // as defined by SystemC >= 2.3.2
#   define CCI_CPLUSPLUS SC_CPLUSPLUS
# else // copy deduction from SystemC 2.3.2
#   ifdef _MSC_VER // don't rely on __cplusplus for MSVC
#     if _MSC_VER < 1800   // MSVC'2010 and earlier, assume C++03
#       define CCI_CPLUSPLUS 199711L
#     elif _MSC_VER < 1900 // MSVC'2013, assume C++11
#       define CCI_CPLUSPLUS 201103L
#     elif _MSC_VER < 2000 // MSVC'2015/2017, assume C++14
#       define CCI_CPLUSPLUS 201402L
#     else // more recent MSVC versions, assume C++14
#       define CCI_CPLUSPLUS 201402L
#     endif
#   else
#     define CCI_CPLUSPLUS __cplusplus
#   endif
# endif
#endif // CCI_CPLUSPLUS

/* ------------------------------------------------------------------------ */

// Macros to check if certain C++ features are supported
#ifndef __has_feature        // Optional of course.
# define __has_feature(x) 0  // Compatibility with non-clang compilers.
#endif
#ifndef __has_extension
# define __has_extension __has_feature // Compatibility with pre-3.0 compilers.
#endif

/* ------------------------------------------------------------------------ */
// Support for C++ rvalue-references / perfect forwarding

#ifndef CCI_HAS_CXX_RVALUE_REFS
# if _MSC_VER >= 1600 || defined(__GXX_EXPERIMENTAL_CXX0X__) || \
     __has_feature(cxx_rvalue_references)
#	 define CCI_HAS_CXX_RVALUE_REFS
# endif
#endif // detect: CCI_HAS_CXX_RVALUE_REFS

#ifdef CCI_HAS_CXX_RVALUE_REFS
# include <utility>
# define CCI_MOVE_(Obj)::std::move(Obj)
# define CCI_FORWARD_(Type,Obj) ::std::forward<Type>(Obj)
#else
# define CCI_MOVE_(Obj) Obj
# define CCI_FORWARD_(Type,Obj) Obj
#endif // CCI_HAS_CXX_RVALUE_REFS

#endif // CCI_CORE_CCI_CONFIG_H_INCLUDED_
