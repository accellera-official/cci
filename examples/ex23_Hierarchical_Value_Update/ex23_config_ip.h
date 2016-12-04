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
 * @file   ex23_config_ip.h
 * @brief  Configure simple_ip's log_level parameters
 * @author Lei Liang, Ericsson
 */
#ifndef EXAMPLES_EX23_HIERARCHICAL_VALUE_UPDATE_EX23_CONFIG_IP_H_
#define EXAMPLES_EX23_HIERARCHICAL_VALUE_UPDATE_EX23_CONFIG_IP_H_

#include <cci_configuration>
#include "xreport.hpp"

/**
 *  @class  ex23_config_ip
 *  @brief  The config_ip class, which set sim_ip's(itself and its sub_ip)
 *          parameter with the name "log_level" from 0 to 500
 */
SC_MODULE(ex23_config_ip) {
public:
    /**
     *  @fn     void ex23_config_ip
     *  @brief  The constructor for the class
     *  @return void
     */
    SC_CTOR(ex23_config_ip):
        m_cci(cci::cci_broker_manager::get_current_broker(
                cci::cci_originator(*this)))
    {
        SC_THREAD(execute);
    }

    static bool
    string_type_predicate(const cci::cci_param_untyped_handle handle)
    {
        return (handle.get_name().substr(handle.get_name().rfind(".")+1)
                =="log_level");
    }

    /**
     *  @fn     void execute()
     *  @brief  The main function which update all "log_level" parameters of
     *          sim_ip(itself and its sub_ip)
     *  @return void
     */
    void execute() {
        const std::string tar_param = "*.log_level";

        // Target value for all log_level
        cci::cci_value tar_value(500);

        XREPORT("execute: List all parameters inside simple_ip");

        std::vector <cci::cci_param_untyped_handle> param_list =
                m_cci.get_param_handles();

        for (std::vector<cci::cci_param_untyped_handle>::iterator it =
                param_list.begin(); it != param_list.end(); ++it) {
            std::cout << (*it).get_name() << " = ";
            std::cout << (*it).get_cci_value();
            std::cout << std::endl;
        }

        XREPORT("execute: List all 'log_level' parameter inside simple_ip");

        cci::cci_param_predicate pred_it(
                &ex23_config_ip::string_type_predicate);

        cci::cci_param_filter_iterator integer_only_pfi =
                m_cci.get_param_handles(pred_it);

        for ( ; integer_only_pfi != integer_only_pfi.end();
                ++integer_only_pfi) {
            if((*integer_only_pfi).is_valid()) {
                std::cout << (*integer_only_pfi).get_name() << " = ";
                std::cout << (*integer_only_pfi).get_cci_value();
                std::cout << std::endl;
            }
        }

        XREPORT("execute: Update all 'log_Level' parameters' value to 500");

        integer_only_pfi = integer_only_pfi.begin();

        for ( ; integer_only_pfi != integer_only_pfi.end();
                ++integer_only_pfi) {
            if((*integer_only_pfi).is_valid()) {
                (*integer_only_pfi).set_cci_value(tar_value);
            }
        }

        XREPORT("execute: List all 'log_Level' parameter inside simple_ip");

        integer_only_pfi = integer_only_pfi.begin();

        for ( ; integer_only_pfi != integer_only_pfi.end();
                ++integer_only_pfi) {
            if((*integer_only_pfi).is_valid()) {
                std::cout << (*integer_only_pfi).get_name() << " = ";
                std::cout << (*integer_only_pfi).get_cci_value();
                std::cout << std::endl;
            }
        }
    }

private:
    cci::cci_broker_if& m_cci; ///< CCI configuration handle
};


#endif //EXAMPLES_EX23_HIERARCHICAL_VALUE_UPDATE_EX23_CONFIG_IP_H_
