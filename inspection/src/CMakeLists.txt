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

add_library(cci-inspection cci/inspection/cci_inspection.cpp)
add_library(SystemC::cci-inspection ALIAS cci-inspection)
target_compile_features(cci-inspection PUBLIC cxx_std_17)
target_include_directories(cci-inspection
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(cci-inspection
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      VERSION ${SystemCCCI_VERSION}
                      SOVERSION ${SystemCCCI_SOVERSION})

install(TARGETS cci-inspection
    EXPORT  SystemCCCITargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT lib)

install(DIRECTORY ./
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT dev
        FILES_MATCHING PATTERN "*.h")
install(FILES "cci_inspection"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT dev)
