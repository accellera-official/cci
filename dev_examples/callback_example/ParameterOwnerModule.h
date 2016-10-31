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
 * @file   ParameterOwnerModule.h
 * @author Christian Schroeder, GreenSocs
 * @author Mark Burton, GreenSocs
 */

#ifndef __PARAMETEROWNERMODULE_H__
#define __PARAMETEROWNERMODULE_H__

#include <systemc>
#include "ex_globals.h"
#include "cci_configuration"


/// Module which owns some cci parameters.
class ParameterOwnerModule
: public sc_core::sc_module
{
  
public:
  
  SC_HAS_PROCESS(ParameterOwnerModule);
	
  /// Constructor
  ParameterOwnerModule(sc_core::sc_module_name name)
  : sc_module(name)
  , int_param ("int_param", 50 )
  , uint_param("uint_param", 12000)
  , uint_param2("uint_param2", 12)
  , str_param("str_param", cci::cci_value("This is a test string."))
  , bool_param("bool_param", false)
  {
      SC_THREAD(main_action);

      try {
          top_param = new cci::cci_param<int>("top.arbitrary.param",
                                              cci::cci_value(
                                                      "This is a param with top-level-name"),
                                              "A string parameter with absolute name.",
                                              cci::CCI_ABSOLUTE_NAME);
      }
      catch (sc_core::sc_report e) {
          std::cout << std::endl << name
                    << ": Unable to initialize 'Owner.top_param'" << std::endl;
          switch (cci::cci_report_handler::get_param_failure(e)) {
            case cci::CCI_VALUE_FAILURE:
              std::cout << e.what() << std::endl;
              break;
          }
      }
  }
  
  /// Main action to make tests with parameters.
  void main_action();
  
  /// Example parameter.
  cci::cci_param<int>             int_param;
  /// Example parameter.
  cci::cci_param<unsigned int>    uint_param;
  /// Example parameter.
  cci::cci_param<unsigned int>    uint_param2;
  /// Example parameter.
  cci::cci_param<std::string>     str_param;
  /// Example parameter.
  cci::cci_param<bool>            bool_param;
  /// Example parameter for top-level-name
  cci::cci_param<int>*            top_param;

};


#endif
