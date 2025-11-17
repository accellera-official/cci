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

cmake_minimum_required (VERSION 3.5)
cmake_policy(SET CMP0001 NEW)

project (SystemCCCI CXX C)

set(SystemCCCI_VERSION_FILE "${PROJECT_SOURCE_DIR}/cci/common/cci_version.h")
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
# CMAKE_VERBOSE_MAKEFILE        Generate a verbose Makefile (default: OFF).
#
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

option(CCI_ENABLE_CFG "Build SystemCCI configuration library" ON)
option(SYSTEMCCCI_BUILD_TESTS "Build tests & examples" ON)
option(CCI_ENABLE_INSPECTION "Build SystemCCI inspection library" ON)
option(BUILD_SOURCE_DOCUMENTATION "Build source documentation with Doxygen." OFF)

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
# Configure status
###############################################################################

message (STATUS "======================================================================================================")
message (STATUS "Settings to build SystemC CCI Configuration ${SystemCCCI_VERSION} (${SystemCCCI_VERSION_RELEASE_DATE})")
message (STATUS "------------------------------------------------------------------------------------------------------")
message (STATUS "BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}")
message (STATUS "BUILD_SOURCE_DOCUMENTATION = ${BUILD_SOURCE_DOCUMENTATION}")
message (STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message (STATUS "SystemCCCI_VERSION = ${SystemCCCI_VERSION}")
message (STATUS "INSTALL_TO_LIB_BUILD_TYPE_DIR = ${INSTALL_TO_LIB_BUILD_TYPE_DIR}")
message (STATUS "INSTALL_TO_LIB_TARGET_ARCH_DIR = ${INSTALL_TO_LIB_TARGET_ARCH_DIR}")
message (STATUS "INSTALL_LIB_TARGET_ARCH_SYMLINK = ${INSTALL_LIB_TARGET_ARCH_SYMLINK}")
message (STATUS "------------------------------------------------------------------------------------------------------")
message (STATUS "CMAKE_CXX_STANDARD = ${CMAKE_CXX_STANDARD}")
message (STATUS "CMAKE_CXX_STANDARD_REQUIRED = ${CMAKE_CXX_STANDARD_REQUIRED}")
message (STATUS "------------------------------------------------------------------------------------------------------")
message (STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message (STATUS "CMAKE_INSTALL_BINDIR = ${CMAKE_INSTALL_BINDIR}")
message (STATUS "CMAKE_INSTALL_DOCDIR = ${CMAKE_INSTALL_DOCDIR}")
message (STATUS "CMAKE_INSTALL_INCLUDEDIR = ${CMAKE_INSTALL_INCLUDEDIR}")
message (STATUS "CMAKE_INSTALL_LIBDIR = ${CMAKE_INSTALL_LIBDIR}")
message (STATUS "INSTALL_CMAKEDIR = ${INSTALL_CMAKEDIR}")
message (STATUS "======================================================================================================")


set(CCI_INTERFACE_MEMBERS)

if(CCI_ENABLE_CFG)
    add_subdirectory(configuration)
    set(CCI_INTERFACE_MEMBERS ${CCI_INTERFACE_MEMBERS} cci-config)
endif()
if(CCI_ENABLE_INSPECTION)
    add_subdirectory(inspection)
    set(CCI_INTERFACE_MEMBERS ${CCI_INTERFACE_MEMBERS} cci-inspection)
endif()
if(BUILD_SOURCE_DOCUMENTATION)
    add_subdirectory (docs)
endif()

# Add all collected components to the SystemC::cci interface library.
add_library(cci INTERFACE)
target_link_libraries(cci INTERFACE ${CCI_INTERFACE_MEMBERS})
add_library(SystemC::cci ALIAS cci)
install(TARGETS cci EXPORT SystemCCCITargets COMPONENT dev)

###############################################################################
# Install README files
###############################################################################

install (FILES INSTALL.md
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
# Export individual cci library targets along the interface library cci
export (TARGETS ${CCI_INTERFACE_MEMBERS} cci
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
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SystemCCCIConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/SystemCCCIConfigVersion.cmake"
        DESTINATION "${SystemCCCI_INSTALL_CMAKEDIR}"
        COMPONENT dev)

# Install version header
install (DIRECTORY ${PROJECT_SOURCE_DIR}/cci/common
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cci
         COMPONENT dev)
