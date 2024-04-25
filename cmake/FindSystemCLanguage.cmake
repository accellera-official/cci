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
# FindSystemCLanguage.cmake --
# Cmake script for defining the SystemC library target from SYSTEMC_HOME
#
# Original Author: Jan Henrik Weinstock, MachineWare Gmbh, 2024
# Adapted for CCI by Lukas Jünger, MachineWare GmbH, 2024
#
###############################################################################

if(NOT DEFINED SystemC_TARGET_ARCH)
    if(DEFINED ENV{TARGET_ARCH})
        set(SystemC_TARGET_ARCH $ENV{TARGET_ARCH})
    elseif(DEFINED TARGET_ARCH)
        set(SystemC_TARGET_ARCH ${TARGET_ARCH})
    elseif(UNIX AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
        if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86")
            set(SystemC_TARGET_ARCH "linux")
        elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
            set(SystemC_TARGET_ARCH "linux64")
        elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
            set(SystemC_TARGET_ARCH "linuxaarch64")
        endif()
    elseif(MSVC)
        set(SystemC_TARGET_ARCH "msvc64")
    elseif(APPLE)
        if (CMAKE_SYSTEM_PROCESSOR MATCHES aarch64|arm64)
            set(SystemC_TARGET_ARCH "macosxarm64")
        elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
            set(SystemC_TARGET_ARCH "macosx64")
        endif()
    endif()
endif()

if(NOT DEFINED SystemC_TARGET_ARCH)
    message(FATAL_ERROR "Cannot determine SystemC TARGET_ARCH")
endif()

if(NOT TARGET SystemC::systemc)
    if(NOT DEFINED SYSTEMC_HOME AND DEFINED ENV{SYSTEMC_HOME})
        set(SYSTEMC_HOME $ENV{SYSTEMC_HOME})
    endif()
    find_path(SYSTEMC_INCLUDE_DIRS NAMES systemc
            HINTS ${SYSTEMC_HOME}/include)

    find_library(SYSTEMC_LIBRARIES NAMES libsystemc.a systemc
            HINTS "${SYSTEMC_HOME}/lib-${SystemC_TARGET_ARCH}"
            "${SYSTEMC_HOME}/lib")

    if(EXISTS ${SYSTEMC_INCLUDE_DIRS}/tlm/)
        list(APPEND SYSTEMC_INCLUDE_DIRS ${SYSTEMC_INCLUDE_DIRS}/tlm)
    endif()
endif()

set(SYSTEMC_VERSION "")
file(GLOB_RECURSE _sysc_ver_file ${SYSTEMC_HOME}/*/sysc/kernel/sc_ver.h)
if(NOT EXISTS ${_sysc_ver_file})
    message(FATAL_ERROR "Cannot find sc_ver.h")
endif()

file(STRINGS ${_sysc_ver_file} _systemc_ver REGEX
        "^#[\t ]*define[\t ]+SC_VERSION_(MAJOR|MINOR|PATCH)[\t ]+([0-9]+)$")
foreach(VPART MAJOR MINOR PATCH)
    foreach(VLINE ${_systemc_ver})
        if(VLINE MATCHES
                "^#[\t ]*define[\t ]+SC_VERSION_${VPART}[\t ]+([0-9]+)$")
            set(SYSTEMC_VERSION_${VPART} ${CMAKE_MATCH_1})
            if(SYSTEMC_VERSION)
                string(APPEND SYSTEMC_VERSION .${CMAKE_MATCH_1})
            else()
                set(SYSTEMC_VERSION ${CMAKE_MATCH_1})
            endif()
        endif()
    endforeach()
endforeach()

set(SystemCLanguage_VERSION ${SYSTEMC_VERSION})
set(SystemCLanguage_DIR ${SYSTEMC_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SystemCLanguage
        REQUIRED_VARS SYSTEMC_LIBRARIES SYSTEMC_INCLUDE_DIRS
        VERSION_VAR   SYSTEMC_VERSION)

if (SystemCLanguage_FOUND AND NOT TARGET SystemC::systemc)
    add_library(systemc UNKNOWN IMPORTED GLOBAL)
    set_target_properties(systemc PROPERTIES
            IMPORTED_LOCATION "${SYSTEMC_LIBRARIES}")
    target_include_directories(systemc INTERFACE "${SYSTEMC_INCLUDE_DIRS}")
    add_library(SystemC::systemc ALIAS systemc)
endif()

message(DEBUG "SYSTEMC_FOUND         " ${SYSTEMC_FOUND})
message(DEBUG "SYSTEMC_HOME          " ${SYSTEMC_HOME})
message(DEBUG "SystemC_TARGET_ARCH   " ${SystemC_TARGET_ARCH})
message(DEBUG "SYSTEMC_INCLUDE_DIRS  " ${SYSTEMC_INCLUDE_DIRS})
message(DEBUG "SYSTEMC_LIBRARIES     " ${SYSTEMC_LIBRARIES})
message(DEBUG "SYSTEMC_VERSION_MAJOR " ${SYSTEMC_VERSION_MAJOR})
message(DEBUG "SYSTEMC_VERSION_MINOR " ${SYSTEMC_VERSION_MINOR})
message(DEBUG "SYSTEMC_VERSION_PATCH " ${SYSTEMC_VERSION_PATCH})
message(DEBUG "SYSTEMC_VERSION       " ${SYSTEMC_VERSION})
