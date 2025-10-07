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

function(add_example NAME)
    string(PREPEND EXAMPLE_NAME "inspection/example/" ${NAME})
    file(GLOB SRC_FILES ${NAME}/*.cpp)
    add_executable(${NAME} ${SRC_FILES})
    target_link_libraries(${NAME} cci-inspection)
    add_test(NAME ${EXAMPLE_NAME} COMMAND ${NAME})
endfunction()

file(GLOB EXAMPLES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ex*/)
foreach(example ${EXAMPLES})
    add_example(${example})
endforeach()
