###############################################################################
#
# Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
# more contributor license agreements.  See the NOTICE file distributed
# with this work for additional information regarding copyright ownership.
# Accellera licenses this file to you under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with the
# License.  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied.  See the License for the specific language governing
# permissions and limitations under the License.
#
###############################################################################

###############################################################################
#
# CMakeLists.txt --
# Top-level CMake script to configure the CCI sources and to generate
# native Makefiles and project workspaces for your compiler environment.
#
# Original Author: Torsten Maehne, Université Pierre et Marie Curie, Paris,
#                  2013-06-11
# Adapted for CCI by Mark Burton, GreenSocs, 2021
#
###############################################################################

###############################################################################
#
# MODIFICATION LOG - modifiers, enter your name, affiliation, date and
# changes you are making here.
#
#     Name, Affiliation, Date: Guillaume Delbergue, GreenSocs, 08 Mar, 2016
# Description of Modification: Extract SystemC/TLM version and date
#
###############################################################################

###############################################################################
#
# This CMake build script <http://www.cmake.org/> was developed taking the
# original autotools-based build system of the Accellera SystemC distribution
# as a reference. However, it is less restrictive regarding the supported
# OS/processor/compiler combinations by detecting the actual features provided
# by the target (Unix or Windows) platform. On Unix (including OS X),
# shared libraries can be built. If the QuickThreads library provides support
# for the target processor, it will be automatically used. Otherwise, we rely
# on Pthreads on Unix and Fiber on Windows. By default, the SystemC library
# installation will follow the GNU standard installation layout so that also
# other SystemC libraries (SystemC, SCV, TLM, AMS extensions) can be installed
# into the same directory hierarchy (Unix: /opt/systemc/;
# Windows: $ENV{ProgramFiles}/SystemC/). The target platform's conventions are
# respected meaning usually include/ for the headers and lib/, lib64/, or
# lib/<multiarch-tuple>/ for the libraries. The lib-${SystemC_TARGET_ARCH})/
# convention is not used by default, as ${SystemC_TARGET_ARCH} does not
# reliably encode the OS/processor/compiler tuple.
#
# The CMake build scripts are compatible with CMake >=2.8.5 and have been tested
# on the following OS/processor/compiler platforms:
#
# - Linux (GCC, Clang): i386, x86_64
#
# - Linux (GCC): aarch64
#
# - Mac OS X >= 10.5 (GCC, Clang): i386, x86_64, ppc
#
# - Windows (MingW32 GCC + optionally MSYS): x86
#
# Currently untested, but considered by the build script (based on the
# autotools scripts) are:
#
# - *BSD (GCC, Clang): i386, x86_64
#
# - Linux (GCC, Clang): sparc, powerpc
#
# - Linux (Clang): aarch64
#
# - Mac OS X <= 10.5 (GCC): ppc, ppc64
#
# - Windows (Cygwin GCC, MingW32 GCC, Visual C++): x86, AMD64
#
# - Solaris (GCC and native compiler): sparc
#
# - HP-UX (GCC and native compiler): hppa
#
# The build process can be configured through the configuration variables
# offered to the user in the CMake console and GUI (ccmake and cmake-gui,
# respectively). Each variable is annotated with a brief documentation string.
# The most important variables are:
#
# BUILD_SHARED_LIBS             Build shared instead of static libraries
#                               (default: ON if not targetting Windows).
#
# BUILD_SOURCE_DOCUMENTATION    Build source code documentation using Doxygen
#                               (default: OFF).
#
# CMAKE_BUILD_TYPE              Specifies the build type on single-configuration
#                               generators. (default: Release).
#
# CMAKE_INSTALL_PREFIX          Root directory of the SystemC libraries
#                               installation (defaults to $ENV{SYSTEMC_HOME}
#                               if set to an absolute path and otherwise to either
#                               /opt/systemc/ (Unix-like platforms including
#                               CYGWIN), $ENV{ProgramFiles}/SystemC/ (on Windows
#                               systems), or ${CMAKE_INSTALL_PREFIX}/systemc.
#
# CMAKE_OSX_ARCHITECTURES       Architectures for cross-compilation on Mac OS X
#                               (default: empty, i.e., only for the system
#                               processor).
#
# CMAKE_VERBOSE_MAKEFILE        Generate a verbose Makefile (default: OFF).
#
#
# SystemC_TARGET_ARCH           Target architecture according to the
#                               Accellera SystemC conventions set either from
#                               $ENV{SYSTEMC_TARGET_ARCH}, $ENV{SYSTEMC_ARCH},
#                               or detected by CMake.
#
# INSTALL_TO_LIB_BUILD_TYPE_DIR Install the libraries to a
#                               lib-${CMAKE_BUILD_TYPE}/ to enable parallel
#                               installation of the different build variants.
#                               (default: OFF)
#
# INSTALL_TO_LIB_TARGET_ARCH_DIR  Install the libraries to lib-<target-arch>
#                               to facilitate linking applications, which
#                               build systems assume to find SystemC in
#                               lib-<target-arch>. (default: OFF)
#
# INSTALL_LIB_TARGET_ARCH_SYMLINK  On Unix, install a symlink lib-<target-arch>
#                               to lib-${CMAKE_BUILD_TYPE} facilitating the
#                               linking of user code, which build system assumes
#                               to find the SystemC libraries under
#                               lib-<target-arch>. (default: OFF)
#
# Other configuration variables will be proposed by CMake depending on the OS
# and detected processor, compiler, and libraries. For more information, please
# refer to the CMake documentation or <http://www.cmake.org/>.
#
###############################################################################
#
# TODO:
#
# - Enable the compilation of SystemC as a DLL on Windows! To make it work, the
#   SystemC library has to be at least split into libsystemc-core.dll and a
#   static libsystemc.lib.
#
# - Check whether preprocessor definitions WIN32, _CONSOLE, _LIB, _DEBUG, NDEBUG,
#   and NOGDI are necessary on WIN32 platforms (MSVC and MinGW).
#
# - Turn on more compiler warnings by default once SystemC is supposed to
#   compile without any warnings on recent compiler versions. Currently, the
#   compiler warning flags are set to reduce the number of generated compiler
#   warnings (especially with recent Clang versions, e.g., Apple Clang 6.0) to
#   an acceptable amount. With the chosen flags, the SystemC library compiles
#   without warnings, but some examples still produce warnings.
#
# - Test on as many different hardware / OS platforms as possible.
#
# - Configure and install systemc.pc and tlm.pc for pkg-config
#
###############################################################################


cmake_minimum_required (VERSION 3.5)
cmake_policy(SET CMP0001 NEW)

project (SystemCCCI CXX C)

set(SystemCCCI_VERSION_FILE "${PROJECT_SOURCE_DIR}/src/cci/core/cci_version.h")
if(EXISTS ${SystemCCCI_VERSION_FILE})
    file(READ ${SystemCCCI_VERSION_FILE} SYSTEMCCCI_VERSION_FILE_CONTENT)
    string(REGEX MATCH "CCI_VERSION_MAJOR[ \t]+([0-9]+)" SystemCCCI_VERSION_MAJOR ${SYSTEMCCCI_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCCCI_VERSION_MAJOR ${SystemCCCI_VERSION_MAJOR})
    string(REGEX MATCH "CCI_VERSION_MINOR[ \t]+([0-9]+)" SystemCCCI_VERSION_MINOR ${SYSTEMCCCI_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCCCI_VERSION_MINOR ${SystemCCCI_VERSION_MINOR})
    string(REGEX MATCH "CCI_VERSION_PATCH[ \t]+([0-9]+)" SystemCCCI_VERSION_PATCH ${SYSTEMCCCI_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCCCI_VERSION_PATCH ${SystemCCCI_VERSION_PATCH})
    string(REGEX MATCH "CCI_IS_PRERELEASE[ \t]+([0-9]+)" SystemCCCI_VERSION_IS_PRERELEASE ${SYSTEMCCCI_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCCCI_VERSION_IS_PRERELEASE ${SystemCCCI_VERSION_IS_PRERELEASE})
    string(REGEX MATCH "CCI_VERSION_PRERELEASE[ \t]+\"([^\".]+)\"" SystemCCCI_VERSION_PRERELEASE ${SYSTEMCCCI_VERSION_FILE_CONTENT})
    string(REGEX REPLACE "(.*)\"(.*)\"" "\\2" SystemCCCI_VERSION_PRERELEASE ${SystemCCCI_VERSION_PRERELEASE})
    string(REGEX MATCH "CCI_SHORT_RELEASE_DATE[ \t]+([0-9]+)" SystemCCCI_VERSION_RELEASE_DATE ${SYSTEMCCCI_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCCCI_VERSION_RELEASE_DATE ${SystemCCCI_VERSION_RELEASE_DATE})
else(EXISTS ${SystemCCCI_VERSION_FILE})
    message (FATAL_ERROR "Unable to read CCI version file")
endif(EXISTS ${SystemCCCI_VERSION_FILE})

set (SystemCCCI_VERSION "${SystemCCCI_VERSION_MAJOR}.${SystemCCCI_VERSION_MINOR}.${SystemCCCI_VERSION_PATCH}")
if (SystemCCCI_VERSION_IS_PRERELEASE)
  set (SystemCCCI_VERSION "${SystemCCCI_VERSION}_${SystemCCCI_VERSION_PRERELEASE}_${SystemCCCI_VERSION_RELEASE_DATE}")
endif (SystemCCCI_VERSION_IS_PRERELEASE)
set (SystemCCCI_SOVERSION "${SystemCCCI_VERSION_MAJOR}.${SystemCCCI_VERSION_MINOR}")



###############################################################################
# Build options
###############################################################################

if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Release CACHE STRING
       "Choose the type of build, options are: empty, Debug, Release, RelWithDebInfo, MinSizeRel."
       FORCE)
endif (NOT CMAKE_BUILD_TYPE)

if (NOT (WIN32 OR CYGWIN))
  option (BUILD_SHARED_LIBS "Build shared libraries." ON)
else (NOT (WIN32 OR CYGWIN))
  option (BUILD_SHARED_LIBS "Build shared libraries." OFF)
endif (NOT (WIN32 OR CYGWIN))
if (BUILD_SHARED_LIBS AND (WIN32 OR CYGWIN))
  message (WARNING "The compilation of SystemC as a DLL on Windows is currently not supported!")
  set (BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries." FORCE)
endif (BUILD_SHARED_LIBS AND (WIN32 OR CYGWIN))

option (BUILD_SOURCE_DOCUMENTATION "Build source documentation with Doxygen." OFF)
option (SYSTEMCCCI_BUILD_TESTS "Build tests" ON)

###############################################################################
# Set SystemC_TARGET_ARCH depending on OS and processor type
###############################################################################

message (STATUS "Checking OS and processor type.")

if (APPLE)
  list (LENGTH CMAKE_OSX_ARCHITECTURES N_OSX_ARCHITECTURES)
endif (APPLE)

if (NOT APPLE OR (APPLE AND (N_OSX_ARCHITECTURES LESS 2)))
  if (APPLE)
    if (N_OSX_ARCHITECTURES EQUAL 1)
      message (STATUS "The target architecture will be passed explicitly to the Apple compiler.")
      set (CMAKE_REQUIRED_FLAGS "-arch ${CMAKE_OSX_ARCHITECTURES}")
    endif (N_OSX_ARCHITECTURES EQUAL 1)
    unset (HAS__i386_DEFINED CACHE)
    unset (HAS__x86_64__DEFINED CACHE)
    unset (HAS__ppc__DEFINED CACHE)
  endif (APPLE)
  include (CheckSymbolExists)
  check_symbol_exists (__LP64__ "" HAS__LP64__DEFINED)
  check_symbol_exists (__sparc "" HAS__sparc_DEFINED)
  check_symbol_exists (__sparc__ "" HAS__sparc__DEFINED)
  check_symbol_exists (__hppa "" HAS__hppa_DEFINED)
  check_symbol_exists (__x86_64__ "" HAS__x86_64__DEFINED)
  check_symbol_exists (__i386 "" HAS__i386_DEFINED)
  check_symbol_exists (__aarch64__ "" HAS__aarch64__DEFINED)
  check_symbol_exists (__ppc__ "" HAS__ppc__DEFINED)
  check_symbol_exists (__powerpc "" HAS__powerpc_DEFINED)
endif (NOT APPLE OR (APPLE AND (N_OSX_ARCHITECTURES LESS 2)))


# Set SystemC_TARGET_ARCH according to the detected
# OS/processor/compiler platform
if (NOT SystemC_TARGET_ARCH)
  if ($ENV{SYSTEMC_TARGET_ARCH})
    message (STATUS "Using SYSTEMC_TARGET_ARCH from the environment.")
    set (_TARGET_ARCH $ENV{SYSTEMC_TARGET_ARCH})
  elseif (APPLE)
    if (N_OSX_ARCHITECTURES GREATER 1)
      set (_TARGET_ARCH "macosxuniversal")
    elseif (HAS__i386_DEFINED)
      set (_TARGET_ARCH "macosx")
    elseif (HAS__x86_64__DEFINED)
      set (_TARGET_ARCH "macosx64")
    elseif (HAS__ppc__DEFINED)
      set (_TARGET_ARCH "macosxppc")
    elseif (HAS__ppc64__DEFINED)
      set (_TARGET_ARCH "macosxppc64")
    else (HAS__i386_DEFINED)
      set (_TARGET_ARCH "macosxunknown")
    endif (N_OSX_ARCHITECTURES GREATER 1)
  elseif (UNIX AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (HAS__i386_DEFINED)
      set (_TARGET_ARCH "linux")
    elseif (HAS__x86_64__DEFINED)
      set (_TARGET_ARCH "linux64")
    elseif (HAS__aarch64__DEFINED)
      set (_TARGET_ARCH "linuxaarch64")
    elseif (HAS__powerpc_DEFINED)
      # Untested platform.
      set (_TARGET_ARCH "linuxppc")
    elseif (HAS__sparc_DEFINED OR HAS__sparc__DEFINED)
      # Untested platform.
      set (_TARGET_ARCH "linuxsparc")
    else (HAS__i386_DEFINED)
      # Untested platform.
      set (_TARGET_ARCH "linuxunknown")
    endif (HAS__i386_DEFINED)
  elseif (UNIX AND CMAKE_SYSTEM_NAME MATCHES ".*BSD")
    if (HAS__i386_DEFINED)
      set (_TARGET_ARCH "bsd")
    elseif (HAS__x86_64__DEFINED)
      set (_TARGET_ARCH "bsd64")
    else (HAS__i386_DEFINED)
      # Untested platform.
      set (_TARGET_ARCH "bsdunknown")
    endif (HAS__i386_DEFINED)
  elseif (UNIX AND CMAKE_SYSTEM_NAME STREQUAL "SunOS"
          AND (HAS__sparc_DEFINED OR HAS__sparc__DEFINED))
    # Untested platform.
    set (_TARGET_ARCH "sparcOS5")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set (_TARGET_ARCH "gcc${_TARGET_ARCH}")
    endif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  elseif (UNIX AND CMAKE_SYSTEM_NAME STREQUAL "HP-UX" AND HAS__hppa_DEFINED)
    # Untested platform.
    set (_TARGET_ARCH "hpux11")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set (_TARGET_ARCH "gcc${_TARGET_ARCH}")
    endif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (HAS__LP64__DEFINED)
      set (_TARGET_ARCH "${_TARGET_ARCH}64")
    endif (HAS__LP64__DEFINED)
  elseif (MSVC)
    set (_TARGET_ARCH "msvc")
    if (CMAKE_CL_64)
      set (_TARGET_ARCH "${_TARGET_ARCH}64")
    endif (CMAKE_CL_64)
  elseif (CYGWIN)
    if (HAS__i386_DEFINED)
      set (_TARGET_ARCH "cygwin")
    elseif (HAS__x86_64__DEFINED)
      set (_TARGET_ARCH "cygwin64")
    else (HAS__i386_DEFINED)
      set (_TARGET_ARCH "cygwinunknown")
    endif (HAS__i386_DEFINED)
  elseif (WIN32
          AND ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
               OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")))
    # MinGW platform, as CYGWIN already has been handled
    if (HAS__i386_DEFINED)
      set (_TARGET_ARCH "mingw")
    elseif (HAS__x86_64__DEFINED)
      set (_TARGET_ARCH "mingw64")
    else (HAS__i386_DEFINED)
      set (_TARGET_ARCH "mingwunknown")
    endif (HAS__i386_DEFINED)
  else ($ENV{SYSTEMC_TARGET_ARCH})
    # Unknown platform, so guess a SystemC target architecture value.
    string (TOLOWER "${CMAKE_SYSTEM_NAME}${CMAKE_PROCESSOR_NAME}" _TARGET_ARCH)
  endif ($ENV{SYSTEMC_TARGET_ARCH})
  set (SystemC_TARGET_ARCH ${_TARGET_ARCH} CACHE STRING "SystemC Target Architecture")
  unset (_TARGET_ARCH)
endif (NOT SystemC_TARGET_ARCH)


list (APPEND CMAKE_PREFIX_PATH /opt/systemc)

IF (NOT SystemCLanguage_FOUND AND NOT TARGET SystemC::systemc)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
    find_package(SystemCLanguage REQUIRED)
ENDIF ()
message (STATUS "Using SystemC ${SystemCLanguage_VERSION} (${SystemCLanguage_DIR})")
IF ( "${SystemC_CXX_STANDARD}" AND NOT "${SystemC_CXX_STANDARD}" EQUAL "${CMAKE_CXX_STANDARD}")
  message (FATAL_ERROR "SystemC_CXX_STANDARD ${SystemC_CXX_STANDARD} does not match SystemCCCI_CXX_STANDARD ${CMAKE_CXX_STANDARD} (-DCMAKE_CXX_STANDARD)")
ENDIF ()



###############################################################################
# Set the installation paths
###############################################################################

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  if (IS_ABSOLUTE $ENV{SYSTEMC_HOME})
    set (_CMAKE_INSTALL_PREFIX $ENV{SYSTEMC_HOME})
  elseif (UNIX)
    set (_CMAKE_INSTALL_PREFIX "/opt/systemc")
  elseif (WIN32)
    file (TO_CMAKE_PATH "$ENV{ProgramFiles}" _WIN32_INSTALL_PREFIX)
    set (_CMAKE_INSTALL_PREFIX "${_WIN32_INSTALL_PREFIX}/SystemC")
    unset (_WIN32_INSTALL_PREFIX)
  else (EXISTS $ENV{SYSTEMC_ROOT_DIR})
    set (_CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/systemc")
  endif (IS_ABSOLUTE $ENV{SYSTEMC_HOME})
  set (CMAKE_INSTALL_PREFIX ${_CMAKE_INSTALL_PREFIX} CACHE PATH "Installation root directory" FORCE)
  unset (_CMAKE_INSTALL_PREFIX)
endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# Set the installation paths for headers, libraries, and documentation.
set (CMAKE_INSTALL_DOCDIR "share/doc/systemc" CACHE PATH
     "Documentation installation directory") # otherwise mixed-case
if (INSTALL_TO_LIB_BUILD_TYPE_DIR)
  # Install libraries to lib-${CMAKE_BUILD_TYPE} so that different build variants
  # of the library can be installed in parallel to the same SystemC root
  # directory. The find_package() mechanism in config-mode of CMake will then
  # automatically find the correct libraries matching the chosen
  # ${CMAKE_BUILD_TYPE}.
  set (_INSTALL_LIBDIR lib-${CMAKE_BUILD_TYPE})
  string (TOLOWER ${_INSTALL_LIBDIR} _INSTALL_LIBDIR)
  set (CMAKE_INSTALL_LIBDIR ${_INSTALL_LIBDIR} CACHE PATH
       "Object code libraries installation directory" FORCE)
  unset (_INSTALL_LIBDIR)
elseif (INSTALL_TO_LIB_TARGET_ARCH_DIR)
  # Some build systems assume to find SystemC in lib-<target-arch>
  set (CMAKE_INSTALL_LIBDIR lib-${SystemC_TARGET_ARCH} CACHE PATH
       "Object code libraries installation directory" FORCE)
endif (INSTALL_TO_LIB_BUILD_TYPE_DIR)
include (GNUInstallDirs)

# Install symlink lib-<target-arch> to lib-${CMAKE_BUILD_TYPE}
if (INSTALL_LIB_TARGET_ARCH_SYMLINK AND CMAKE_HOST_UNIX)
  install (CODE "
    EXECUTE_PROCESS(COMMAND cmake -E create_symlink ${CMAKE_INSTALL_LIBDIR} lib-${SystemC_TARGET_ARCH}
                    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX})
  ")
endif (INSTALL_LIB_TARGET_ARCH_SYMLINK AND CMAKE_HOST_UNIX)

if (INSTALL_TO_LIB_BUILD_TYPE_DIR OR INSTALL_TO_LIB_TARGET_ARCH_DIR)
  set (SystemCCCI_INSTALL_CMAKEDIR share/cmake/SystemCCCI CACHE PATH
       "CMake package configuration installation directory" FORCE)
else (INSTALL_TO_LIB_BUILD_TYPE_DIR OR INSTALL_TO_LIB_TARGET_ARCH_DIR)
  set (SystemCCCI_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/SystemCCCI CACHE PATH
       "CMake package configuration installation directory for the SystemCCCI package.")
endif (INSTALL_TO_LIB_BUILD_TYPE_DIR OR INSTALL_TO_LIB_TARGET_ARCH_DIR)
set(SystemCCCI_INSTALL_FULL_CMAKEDIR "${CMAKE_INSTALL_PREFIX}/${SystemCCCI_INSTALL_CMAKEDIR}")


###############################################################################
# Configure RPATH so that all targets find the libraries they link to
###############################################################################

# Starting from CMake 2.8.12 use CMake's full RPATH support on Mac OS X
set (CMAKE_MACOSX_RPATH TRUE)

# Use, i.e., don't skip, the full RPATH for the build tree.
set (CMAKE_SKIP_BUILD_RPATH  FALSE)
# For build, don't use the install RPATH already (but later on when
# installing).
set (CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
# Set the RPATH to be used when installing.
set (CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
# Add the automatically determined parts of the RPATH, which point to
# directories outside the build tree, to the install RPATH.
set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Set the RPATH to be used when installing, but only if it's not a system directory
list (FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_FULL_LIBDIR}" isSystemDir)
if ("${isSystemDir}" STREQUAL "-1")
  set (CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
endif ("${isSystemDir}" STREQUAL "-1")



###############################################################################
# Configure status
###############################################################################

message (STATUS "========================================================================")
message (STATUS "Settings to build SystemC CCI ${SystemCCCI_VERSION} (${SystemCCCI_VERSION_RELEASE_DATE})")
message (STATUS "------------------------------------------------------------------------")
message (STATUS "BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}")
message (STATUS "BUILD_SOURCE_DOCUMENTATION = ${BUILD_SOURCE_DOCUMENTATION}")
message (STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message (STATUS "SystemC_TARGET_ARCH = ${SystemC_TARGET_ARCH}")
message (STATUS "SystemCCCI_VERSION = ${SystemCCCI_VERSION}")
message (STATUS "INSTALL_TO_LIB_BUILD_TYPE_DIR = ${INSTALL_TO_LIB_BUILD_TYPE_DIR}")
message (STATUS "INSTALL_TO_LIB_TARGET_ARCH_DIR = ${INSTALL_TO_LIB_TARGET_ARCH_DIR}")
message (STATUS "INSTALL_LIB_TARGET_ARCH_SYMLINK = ${INSTALL_LIB_TARGET_ARCH_SYMLINK}")
message (STATUS "------------------------------------------------------------------------")
message (STATUS "CMAKE_CXX_STANDARD = ${CMAKE_CXX_STANDARD}")
message (STATUS "CMAKE_CXX_STANDARD_REQUIRED = ${CMAKE_CXX_STANDARD_REQUIRED}")
message (STATUS "CMAKE_SYSTEM = ${CMAKE_SYSTEM}")
message (STATUS "CMAKE_SYSTEM_PROCESSOR = ${CMAKE_SYSTEM_PROCESSOR}")
message (STATUS "QT_ARCH = ${QT_ARCH}")
if (DEFINED CMAKE_OSX_ARCHITECTURES)
  message (STATUS "CMAKE_OSX_ARCHITECTURES = ${CMAKE_OSX_ARCHITECTURES}")
endif (DEFINED CMAKE_OSX_ARCHITECTURES)
if (DEFINED CMAKE_OSX_DEPLOYMENT_TARGET)
  message (STATUS "CMAKE_OSX_DEPLOYMENT_TARGET = ${CMAKE_OSX_DEPLOYMENT_TARGET}")
endif (DEFINED CMAKE_OSX_DEPLOYMENT_TARGET)
if (DEFINED CMAKE_OSX_SYSROOT)
  message (STATUS "CMAKE_OSX_SYSROOT = ${CMAKE_OSX_SYSROOT}")
endif(DEFINED CMAKE_OSX_SYSROOT)
if (DEFINED Threads_FOUND)
  message (STATUS "Threads_FOUND = ${Threads_FOUND}")
endif (DEFINED Threads_FOUND)
if (DEFINED CMAKE_USE_PTHREADS_INIT)
  message (STATUS "CMAKE_USE_PTHREADS_INIT = ${CMAKE_USE_PTHREADS_INIT}")
endif (DEFINED CMAKE_USE_PTHREADS_INIT)
if (DEFINED CMAKE_USE_WIN32_THREADS_INIT)
  message (STATUS "CMAKE_USE_WIN32_THREADS_INIT = ${CMAKE_USE_WIN32_THREADS_INIT}")
endif (DEFINED CMAKE_USE_WIN32_THREADS_INIT)
if (DEFINED CMAKE_THREAD_LIBS_INIT)
  message (STATUS "CMAKE_THREAD_LIBS_INIT = ${CMAKE_THREAD_LIBS_INIT}")
endif (DEFINED CMAKE_THREAD_LIBS_INIT)
message (STATUS "------------------------------------------------------------------------")
message (STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message (STATUS "CMAKE_INSTALL_BINDIR = ${CMAKE_INSTALL_BINDIR}")
message (STATUS "CMAKE_INSTALL_DOCDIR = ${CMAKE_INSTALL_DOCDIR}")
message (STATUS "CMAKE_INSTALL_INCLUDEDIR = ${CMAKE_INSTALL_INCLUDEDIR}")
message (STATUS "CMAKE_INSTALL_LIBDIR = ${CMAKE_INSTALL_LIBDIR}")
message (STATUS "INSTALL_CMAKEDIR = ${INSTALL_CMAKEDIR}")
message (STATUS "========================================================================")


###############################################################################
# Subdirectories
###############################################################################

add_subdirectory (docs)
add_subdirectory (src)
add_subdirectory (examples)


###############################################################################
# Install README files
###############################################################################

install (FILES AUTHORS
               INSTALL.md
               cmake/INSTALL_USING_CMAKE
               LICENSE
               NOTICE
               README.md
               RELEASENOTES.md
         DESTINATION ${CMAKE_INSTALL_DOCDIR}
         COMPONENT doc)


###############################################################################
# Provide package of the SystemC library to other programs using CMake
###############################################################################

# Export the cci library target for usage by other programs
# ... to local build tree
export (TARGETS cci
        NAMESPACE SystemC::
        FILE SystemCCCITargets.cmake)
# ... to installation tree
install (EXPORT SystemCCCITargets
         NAMESPACE SystemC::
         DESTINATION ${SystemCCCI_INSTALL_CMAKEDIR}
         COMPONENT dev)

# Register the SystemCCCI packages for use from the
# build tree. (This registers the build tree with the global CMake-registry.)
export (PACKAGE SystemCCCI)

# Create the <Package>Config.cmake and <Package>ConfigVersion files
include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/SystemCCCIConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/SystemCCCIConfig.cmake
  INSTALL_DESTINATION ${SystemCCCI_INSTALL_CMAKEDIR})
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/SystemCCCIConfigVersion.cmake"
  VERSION ${SystemCCCI_VERSION_MAJOR}.${SystemCCCI_VERSION_MINOR}.${SystemCCCI_VERSION_PATCH}.${SystemCCCI_VERSION_RELEASE_DATE}
  COMPATIBILITY AnyNewerVersion
)

# Install the <Package>Config.cmake and <Package>ConfigVersion.cmake
install(FILES "${PROJECT_BINARY_DIR}/SystemCCCIConfig.cmake"
              "${PROJECT_BINARY_DIR}/SystemCCCIConfigVersion.cmake"
        DESTINATION "${SystemCCCI_INSTALL_CMAKEDIR}"
        COMPONENT dev)
