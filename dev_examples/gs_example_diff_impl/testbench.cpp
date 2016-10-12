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
 * @file   testbench.cpp
 * @author Christian Schroeder, GreenSocs
 */

#include <systemc>
#include <cci_configuration>

#include "OwnerModuleA.h"
#include "OwnerModuleB.h"

/// Example testbench for the CCI example demonstrating that it is possible 
/// to use multiple parameter implementations (for _different_ data types) within one simulation
/**
 * OwnerModuleA is linked against the proof-of-concept parameter implementation,
 * OwnerModuleB is linked against both the poc a parameter implementation for an additional datatype.
 */
int sc_main(int argc, char *argv[]) {
  //sc_core::sc_report_handler::set_actions(sc_core::SC_ERROR, sc_core::SC_ABORT);
  //sc_core::sc_report_handler::set_actions(sc_core::SC_WARNING, sc_core::SC_ABORT);

  // Note: this prevents from catching the error!!
  //sc_core::sc_report_handler::set_actions("/ASI/CCI/set_param_failed",  sc_core::SC_DISPLAY);
  sc_core::sc_report_handler::set_actions("/ASI/CCI/cci_value_failure", sc_core::SC_DISPLAY);


  OwnerModuleA     ownerA     ("OwnerA");
  OwnerModuleB     ownerB     ("OwnerB");

  // test == operator
  std::cout << "ownerA.int_param == ownerB.int_param: " << 
  ((ownerA.int_param == ownerB.int_param) ? "true" : "false") << std::endl;

  
  std::cout << std::endl << "------ sc_start() ----------------" << std::endl << std::endl;
  sc_core::sc_start(); 
  std::cout << std::endl << "------ sc_start() returned -------" << std::endl << std::endl;
  
  return EXIT_SUCCESS; 
  
}
