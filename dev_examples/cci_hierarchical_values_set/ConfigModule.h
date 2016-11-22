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
 * @file   cci_hierarchical_values_set/ConfigModule.h
 * @author Lei Liang, Ericsson
 */
#ifndef __CONFIGMODULE_H__
#define __CONFIGMODULE_H__


#include <systemc>
#include <cci_configuration>

#ifndef SC_INCLUDE_DYNAMIC_PROCESSES
#define SC_INCLUDE_DYNAMIC_PROCESSES
#endif

class ConfigModule
: public sc_core::sc_module
{

public:

  SC_HAS_PROCESS(ConfigModule);

  ConfigModule(sc_core::sc_module_name name):
          m_cci(cci::cci_broker_manager::get_broker())
  {
    SC_THREAD(execute);
  };

  void execute() {
    const std::string tar_param = "*.log_level";

    // Target value for all log_level
    cci::cci_value tar_value(500);;

    std::cout << std::endl << "**** Parameter list and values: @ "<<sc_core::sc_time_stamp() << std::endl;

    // To get the all parameters
    std::vector<std::string> vec_all = m_cci.get_param_list();
    // To get parameter with pattern (*.log_level)
	/// @todo replace pattern based search (not part of standard) with predicate once available.
    std::vector<std::string> vec_ll = m_cci.get_param_list(tar_param);
    std::vector<std::string>::iterator it;

    for (it = vec_all.begin() ; it < vec_all.end(); it++) {
      std::cout<<*it<<" = ";
      std::cout<<m_cci.get_cci_value(*it).to_json();
      std::cout<<std::endl;
    }

    wait(10,sc_core::SC_NS);

    std::cout << std::endl << "**** Set all log_level to 500: @ "<<sc_core::sc_time_stamp() << std::endl;


    for (it = vec_ll.begin() ; it < vec_ll.end(); it++) {
        (m_cci.get_param_handle(*it)).set_cci_value(tar_value);
    }

    wait(20,sc_core::SC_NS);

    std::cout << std::endl << "**** Parameter list and values: @ "<<sc_core::sc_time_stamp() << std::endl;

    for (it = vec_all.begin() ; it < vec_all.end(); it++) {
      std::cout<<*it<<" = ";
      std::cout<<m_cci.get_cci_value(*it).to_json();
      std::cout<<std::endl;
    }

  }

  ~ConfigModule(){};

private:
  cci::cci_broker_if& m_cci;

};


#endif
