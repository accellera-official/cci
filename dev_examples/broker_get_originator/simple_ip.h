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
 * @file   simple_ip.h
 * @author Lei Liang, Ericsson
 */

#ifndef BROKER_GET_ORIGINATOR_SIMPLE_IP_H_
#define BROKER_GET_ORIGINATOR_SIMPLE_IP_H_

#include <cci_configuration>

/**
 *  @class  simple_ip
 *  @brief  This simple_ip class owns one CCI param for which the originator will be
 *          got by global broker
 */
SC_MODULE(simple_ip) {
    /**
     *  @fn     void simple_ip()
     *  @brief  class constructor
     *  @return void
     */
    SC_CTOR(simple_ip)
    : int_param("int_param", 0)
    {}

private:
    /// Parameters of int types
    cci::cci_param<int> int_param;
};


#endif //BROKER_GET_ORIGINATOR_SIMPLE_IP_H_
