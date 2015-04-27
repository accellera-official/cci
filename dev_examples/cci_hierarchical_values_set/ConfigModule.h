/*****************************************************************************
  Copyright 2006-2015 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2006-2014 OFFIS Institute for Information Technology
  All rights reserved.

  Copyright 2006-2015 Intel Corporation
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
 * @file   cci_hierarchical_values_set/ConfigModule.h
 * @author Lei Liang, Ericsson
 */
#ifndef __CONFIGMODULE_H__
#define __CONFIGMODULE_H__


#include <systemc>
#include "cci"

#define SC_INCLUDE_DYNAMIC_PROCESSES

class ConfigModule
: public sc_core::sc_module
{

public:

  SC_HAS_PROCESS(ConfigModule);

  ConfigModule(sc_core::sc_module_name name)
  {
    m_cci = &cci::cnf::cci_broker_manager::get_current_broker(
            cci::cnf::cci_originator(*this));
    assert(m_cci != NULL);
    SC_THREAD(execute);
  };

  void execute() {
    const std::string tar_param = "log_level";

    // Target value for all log_level
    cci::cnf::cci_value tar_value;
    tar_value.set<int>(500);

    std::cout << std::endl << "**** Parameter list and values: @ "<<sc_core::sc_time_stamp() << std::endl;

    // To get the all parameters
    std::vector<std::string> vec_all = m_cci->get_param_list();
    // To get parameter with patter (*.log_level)
    std::vector<std::string> vec_ll = m_cci->get_param_list("*.log_level");
    std::vector<std::string>::iterator it;

    for (it = vec_all.begin() ; it < vec_all.end(); it++) {
      std::cout<<*it<<" = ";
      std::cout<<m_cci->json_serialize(*it);
      std::cout<<std::endl;
    }

    wait(10,sc_core::SC_NS);

    std::cout << std::endl << "**** Set all log_level to 500: @ "<<sc_core::sc_time_stamp() << std::endl;


    for (it = vec_ll.begin() ; it < vec_ll.end(); it++) {
        (m_cci->get_param(*it))->set_value(tar_value);
    }

    wait(20,sc_core::SC_NS);

    std::cout << std::endl << "**** Parameter list and values: @ "<<sc_core::sc_time_stamp() << std::endl;

    for (it = vec_all.begin() ; it < vec_all.end(); it++) {
      std::cout<<*it<<" = ";
      std::cout<<m_cci->json_serialize(*it);
      std::cout<<std::endl;
    }

  }

  ~ConfigModule(){};

private:
  cci::cnf::cci_cnf_broker_if *m_cci;

};


#endif
