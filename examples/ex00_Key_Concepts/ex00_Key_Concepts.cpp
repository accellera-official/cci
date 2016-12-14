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
 *  @file   ex00_Key_Concepts.cpp
 *  @brief  A testbench that demonstrates all the fundamentals
 *          of CCI standard.
 *  @author Sonal Poddar, Intel
 */

#include "ex00_Top.h"
#include "ex00_Param_User_Class.h"

/**
 *  @fn     int sc_main(int argc, char* argv[])
 *  @brief  A testbench for key concepts used in
 *          CCI standard.
 *  @param  argc  An integer for the number of arguments
 *  @param  argv  An array with the input arguments
 *  @return and integer of successful execution
 */

int sc_main(int argc, char *argv[]) {
  ex00_Top top("top");
  ex00_Param_User_Class user_class;
  SC_REPORT_INFO("sc_main", "Begin Simulation.");
  sc_core::sc_start();
  user_class.update_param();
  SC_REPORT_INFO("sc_main", "End Simulation.");

  return EXIT_SUCCESS;
}
