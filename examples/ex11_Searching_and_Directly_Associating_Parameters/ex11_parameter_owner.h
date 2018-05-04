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
 *  @file      parameter_owner.h
 *  @brief     This file contains the code which defines the OWNER module and its contents.
 *             The owner module is instantiated by the TOP_MODULE
 *  @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date      9th June, 2011 (Thursday)
 */

#ifndef EXAMPLES_EX11_SEARCHING_AND_DIRECTLY_ASSOCIATING_PARAMETERS_EX11_PARAMETER_OWNER_H_
#define EXAMPLES_EX11_SEARCHING_AND_DIRECTLY_ASSOCIATING_PARAMETERS_EX11_PARAMETER_OWNER_H_

#include <cci_configuration>
#include <string>
#include "xreport.hpp"

/**
 *  @class  ex11_parameter_owner
 *  @brief  This module creates cci_parameters with the names passed from
 *          ex11_top_module
 */
SC_MODULE(ex11_parameter_owner) {
 public:
  SC_HAS_PROCESS(ex11_parameter_owner);

  /**
   *  @fn     ex11_parameter_owner(sc_core::sc_module name _name, std::string _param_name = "clock_speed_Hz", int _param_value = 1000)
   *  @brief  The class constructor
   *  @param  _name The name of the class
   *  @param  _param_name The name of the parameter
   *  @param  _param_value  The value to be assigned to the parameter
   *  @return void
   */
  ex11_parameter_owner(sc_core::sc_module_name _name,
                       std::string _param_name = "clock_speed_Hz",
                       int _param_value = 1000) {
    clk = new cci::cci_param<int>(_param_name.c_str(), _param_value);

    XREPORT("[OWNER C_TOR] : Parameter Name   : " << clk->name()
            << ", Value : " << clk->get_value());
  }

 private :
  cci::cci_param<int>* clk;  ///< Declare an instance of mutable CCI parameter of type 'int'
};
// ex11_parameter_owner

#endif  // EXAMPLES_EX11_SEARCHING_AND_DIRECTLY_ASSOCIATING_PARAMETERS_EX11_PARAMETER_OWNER_H_
