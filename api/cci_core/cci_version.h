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
 * @file   cci_version.h
 * @brief  basic version information for CCI
 * @author Philipp A. Hartmann, OFFIS
 *
 */
#ifndef CCI_CORE_CCI_VERSION_H_INCLUDED_
#define CCI_CORE_CCI_VERSION_H_INCLUDED_

#define CCI_SHORT_RELEASE_DATE 20150631

#define CCI_VERSION_ORIGINATOR "Accellera"
#define CCI_VERSION_MAJOR      0
#define CCI_VERSION_MINOR      9
#define CCI_VERSION_PATCH      0
#define CCI_IS_PRERELEASE      1

// token stringification

#define CCI_STRINGIFY_HELPER_( Arg ) \
  CCI_STRINGIFY_HELPER_DEFERRED_( Arg )
#define CCI_STRINGIFY_HELPER_DEFERRED_( Arg ) \
  CCI_STRINGIFY_HELPER_MORE_DEFERRED_( Arg )
#define CCI_STRINGIFY_HELPER_MORE_DEFERRED_( Arg ) \
  #Arg

#define CCI_VERSION_RELEASE_DATE \
  CCI_STRINGIFY_HELPER_( CCI_SHORT_RELEASE_DATE )

#if ( CCI_IS_PRERELEASE == 1 )
#  define CCI_VERSION_PRERELEASE "pub_rev"
#  define CCI_VERSION \
    CCI_STRINGIFY_HELPER_( CCI_VERSION_MAJOR.CCI_VERSION_MINOR.CCI_VERSION_PATCH ) \
    "_" CCI_VERSION_PRERELEASE "_" CCI_VERSION_RELEASE_DATE \
    "-" CCI_VERSION_ORIGINATOR
#else
#  define CCI_VERSION_PRERELEASE "" // nothing
#  define CCI_VERSION \
    CCI_STRINGIFY_HELPER_( CCI_VERSION_MAJOR.CCI_VERSION_MINOR.CCI_VERSION_PATCH ) \
    "-" CCI_VERSION_ORIGINATOR
#endif

#endif // CCI_CORE_CCI_VERSION_H_INCLUDED_