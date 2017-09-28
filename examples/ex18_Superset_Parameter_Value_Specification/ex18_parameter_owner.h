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
 *  @file     parameter_owner.h
 *  @brief    This class declares few of the cci-parameters which are
 *            initialized with preset values either from the testbench (main.cpp)
 *            file or the configuration file reader & parser.
 *  @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date     21st July, 2011 (Thursday)
 */

#ifndef EXAMPLES_EX18_SUPERSET_PARAMETER_VALUE_SPECIFICATION_EX18_PARAMETER_OWNER_H_
#define EXAMPLES_EX18_SUPERSET_PARAMETER_VALUE_SPECIFICATION_EX18_PARAMETER_OWNER_H_

#include <cci_configuration>

/**
 *  @class  ex18_parameter_owner
 *  @brief  This SC_MODULE class instantiates two 'integer' type cci-parameters along
 *          with default values.  In this case, both the parameters have been provided
 *          with preset values from the testbench file
 */
SC_MODULE(ex18_parameter_owner) {
 public:
  /**
   *  @fn     ex18_parameter_owner()
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex18_parameter_owner)
     : int_param_1("int_param_1", 10),
       int_param_2("int_param_2", 20) {}

 private:
  // Declare the cci-parameters of type 'int' and 'std::string'
  cci::cci_param<int> int_param_1; ///< An int CCI parameter
  cci::cci_param<int> int_param_2; ///< An int CCI parameter
};
// ex18_parameter_owner

#endif  // EXAMPLES_EX18_SUPERSET_PARAMETER_VALUE_SPECIFICATION_EX18_PARAMETER_OWNER_H_
