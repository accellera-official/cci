/*****************************************************************************
  Copyright 2006-2014 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010-2015 CircuitSutra Technologies Pvt. Ltd.
  All rights reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 *****************************************************************************/

/**
 *  @file     parameter_owner.h
 *  @brief    This class declares few of the cci-parameters which are
 *            initialized with initial values either from the testbench (main.cpp)
 *            file or the configuration file reader & parser.
 *  @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date     21st July, 2011 (Thursday)
 */

#ifndef EXAMPLES_EX18_SUPERSET_PARAMETER_VALUE_SPECIFICATION_EX18_PARAMETER_OWNER_H_
#define EXAMPLES_EX18_SUPERSET_PARAMETER_VALUE_SPECIFICATION_EX18_PARAMETER_OWNER_H_

#include <cci>

/// This SC_MODULE class instantiates two 'integer' type cci-parameters along
/// with default values.  In this case, both the parameters have been provided
/// with initial values from the testbench file
/// @see example#17 : Specifying Parameter Values via a Configuration File for
SC_MODULE(ex18_parameter_owner) {
 public:
  SC_CTOR(ex18_parameter_owner)
     : int_param_1("int_param_1", 10),
       int_param_2("int_param_2", 20) {}

 private:
  /// Declare the cci-parameters of type 'int' and 'std::string'
  cci::cnf::cci_param<int> int_param_1;
  cci::cnf::cci_param<int> int_param_2;
};
/// ex18_parameter_owner

#endif  // EXAMPLES_EX18_SUPERSET_PARAMETER_VALUE_SPECIFICATION_EX18_PARAMETER_OWNER_H_
