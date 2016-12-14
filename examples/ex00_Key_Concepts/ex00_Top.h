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
 *  @file   ex00_Top.h
 *  @brief  This file constructs the "subsystem1" module.
 *  @author Sonal Poddar, Intel
 */

#ifndef EXAMPLES_EX00_KEY_CONCEPTS_EX00_TOP_H_
#define EXAMPLES_EX00_KEY_CONCEPTS_EX00_TOP_H_

#include <cci_configuration>
#include <string>
#include "ex00_Subsystem.h"


/**
 *  @class  ex00_Top
 */
SC_MODULE(ex00_Top) {
 public:
  /**
   *  @fn     void ex00_Top()
   *  @brief  The constructor for the class.
   *  @return void
   */
  SC_CTOR(ex00_Top) : subsystem1("subsystem1")
  {
  }

ex00_Subsystem subsystem1;
};

// ex00_Top

#endif  // EXAMPLES_EX00_KEY_CONCEPTS_EX00_TOP_H_




