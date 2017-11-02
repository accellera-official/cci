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
        m_broker(cci::cci_get_broker())
    {
        SC_THREAD(execute);
    }

    static bool
    log_level_param_predicate(const cci::cci_param_untyped_handle handle)
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
        // Target value for all log_level params
        cci::cci_value target_value(500);

        XREPORT("execute: List all parameters inside simple_ip");

        std::vector <cci::cci_param_untyped_handle> param_list =
                m_broker.get_param_handles();

        for (std::vector<cci::cci_param_untyped_handle>::iterator it =
                param_list.begin(); it != param_list.end(); ++it) {
            std::cout << (*it).get_name() << " = ";
            std::cout << (*it).get_cci_value();
            std::cout << std::endl;
        }

        XREPORT("execute: List all 'log_level' parameter inside simple_ip");

        cci::cci_param_predicate pred_log_level(
                &ex23_config_ip::log_level_param_predicate);

        cci::cci_param_range log_level_filtered_range =
                m_broker.get_param_handles(pred_log_level);

        for (cci::cci_param_range::iterator it =
                log_level_filtered_range.begin();
                it != log_level_filtered_range.end();
                ++it) {
            if((*it).is_valid()) {
                std::cout << (*it).get_name() << " = ";
                std::cout << (*it).get_cci_value();
                std::cout << std::endl;
            }
        }

        XREPORT("execute: Update all 'log_Level' parameters' value to 500");

        for (cci::cci_param_range::iterator it =
                log_level_filtered_range.begin();
                it != log_level_filtered_range.end();
                ++it) {
            if((*it).is_valid()) {
                (*it).set_cci_value(target_value);
            }
        }

        XREPORT("execute: List all 'log_Level' parameter inside simple_ip");

        for (cci::cci_param_range::iterator it =
                log_level_filtered_range.begin();
                it != log_level_filtered_range.end();
                ++it) {
            if((*it).is_valid()) {
                std::cout << (*it).get_name() << " = ";
                std::cout << (*it).get_cci_value();
                std::cout << std::endl;
            }
        }
    }

private:
    cci::cci_broker_handle m_broker; ///< CCI configuration handle
};


#endif //EXAMPLES_EX23_HIERARCHICAL_VALUE_UPDATE_EX23_CONFIG_IP_H_
