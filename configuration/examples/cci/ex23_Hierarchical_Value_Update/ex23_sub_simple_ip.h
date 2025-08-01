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
 * @file   ex23_sub_simple_ip.h
 * @author Lei Liang, Ericsson
 */

#ifndef EXAMPLES_EX23_HIERARCHICAL_VALUE_UPDATE_EX23_SUB_SIMPLE_IP_H_
#define EXAMPLES_EX23_HIERARCHICAL_VALUE_UPDATE_EX23_SUB_SIMPLE_IP_H_

#include <cci_configuration>
#include "xreport.hpp"

/**
 *  @class  ex23_sub_simple_ip
 *  @brief  This ex23_sub_simple_ip class owns three CCI params with
 *          one named 'log_level'. This class is instantiated inside
 *          ex23_simple_ip as a member(sub module).
 */
SC_MODULE(ex23_sub_simple_ip) {
    /**
     *  @fn     void ex23_sub_simple_ip()
     *  @brief  class constructor
     *  @return void
     */
    SC_CTOR(ex23_sub_simple_ip)
    : log_level("log_level", 0)
    , int_param1("int_param1", 0)
    , int_param2("int_param2", 0) {
    }

private:
    /// Parameters of int types
    cci::cci_param<int> log_level; ///the target log_level to be updated
    cci::cci_param<int> int_param1;///int parameter to demo hierarchical params
    cci::cci_param<int> int_param2;///int parameter to demo hierarchical params
};

#endif //EXAMPLES_EX23_HIERARCHICAL_VALUE_UPDATE_EX23_SUB_SIMPLE_IP_H_
