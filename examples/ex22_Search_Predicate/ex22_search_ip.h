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

#ifndef EXAMPLES_EX22_SEARCH_PREDICATE_EX22_SEARCH_IP_H_
#define EXAMPLES_EX22_SEARCH_PREDICATE_EX22_SEARCH_IP_H_

#include <cci_configuration>
#include <cassert>
#include <string>
#include "xreport.hpp"

/**
 * @file   ex22_search_ip.h
 * @author Guillaume Delbergue, GreenSocs / Ericsson
 */

/**
 *  @class  ex22_search_ip
 *  @brief  The search_ip class, which search sim_ip's parameter
 */
SC_MODULE(ex22_search_ip) {
public:
    /**
     *  @fn     void ex22_search_ip
     *  @brief  The constructor for the class
     *  @return void
     */
    SC_CTOR(ex22_search_ip):
        m_cci(cci::cci_broker_manager::get_current_broker(
                cci::cci_originator(*this)))
    {
        SC_THREAD(execute);
    }

    static bool
    integer_type_predicate(const cci::cci_param_untyped_handle handle)
    {
        return (handle.get_basic_type() == cci::CCI_NUMBER_DATA);
    }

    static bool
    string_type_predicate(const cci::cci_param_untyped_handle handle)
    {
        return (handle.get_basic_type() == cci::CCI_STRING_DATA);
    }

    bool
    value_in_range_predicate(const cci::cci_param_untyped_handle handle,
                             double greater_eq, double lower)
    {
        if(handle.get_basic_type() == cci::CCI_REAL_DATA
           || handle.get_basic_type() == cci::CCI_NUMBER_DATA) {
            return (handle.get_cci_value().get_double() >= greater_eq
                    && handle.get_cci_value().get_double() < lower);
        }
        return false;
    }

    bool name_contains_character_predicate(
            const cci::cci_param_untyped_handle handle, const char character)
    {
        return handle.get_name().find(character) != std::string::npos;
    }

    /**
     *  @fn     void execute()
     *  @brief  The main function which search parameters of sim_ip
     *  @return void
     */
    void execute() {

        XREPORT("execute: Original list");
        std::vector <cci::cci_param_untyped_handle> param_list =
                m_cci.get_param_handles();
        for (std::vector<cci::cci_param_untyped_handle>::iterator it =
                param_list.begin(); it != param_list.end(); ++it) {
            std::cout << (*it).get_name() << " = ";
            std::cout << (*it).get_cci_value();
            std::cout << std::endl;
        }

        XREPORT("execute: Integers only");
        cci::cci_param_predicate pred_it(
                &ex22_search_ip::integer_type_predicate);
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

        XREPORT("execute: String only");
        cci::cci_param_predicate pred_st(
                &ex22_search_ip::string_type_predicate);
        cci::cci_param_filter_iterator string_only_pfi =
                m_cci.get_param_handles(pred_st);
        for ( ; string_only_pfi != string_only_pfi.end(); ++string_only_pfi) {
            if ((*string_only_pfi).is_valid()) {
                std::cout << (*string_only_pfi).get_name() << " = ";
                std::cout << (*string_only_pfi).get_cci_value();
                std::cout << std::endl;
            }
        }

        XREPORT("execute: Numbers greater than or equal to 3 and smaller than "
                        "85 only");
        cci::cci_param_predicate pred_vir(sc_bind(
            &ex22_search_ip::value_in_range_predicate, this,
                    sc_unnamed::_1, 3, 85));
        cci::cci_param_filter_iterator value_in_range_pfi =
                m_cci.get_param_handles(pred_vir);
        while(value_in_range_pfi != value_in_range_pfi.end()) {
            std::cout << (*value_in_range_pfi).get_name() << " = ";
            std::cout << (*value_in_range_pfi).get_cci_value();
            std::cout << std::endl;
            ++value_in_range_pfi;
        }

        XREPORT("execute: Parameter name contains at least 't' only");
        cci::cci_param_predicate pred_ncc(sc_bind(
            &ex22_search_ip::name_contains_character_predicate, this,
                    sc_unnamed::_1, 't'));
        cci::cci_param_filter_iterator letter_pfi =
                m_cci.get_param_handles(pred_ncc);
        while(letter_pfi != letter_pfi.end()) {
            std::cout << (*letter_pfi).get_name() << " = ";
            std::cout << (*letter_pfi).get_cci_value();
            std::cout << std::endl;
            ++letter_pfi;
        }
    }

private:
    cci::cci_broker_if& m_cci; ///< CCI configuration handle
};
// ex22_search_ip

#endif  // EXAMPLES_EX22_SEARCH_PREDICATE_EX22_SEARCH_IP_H_
