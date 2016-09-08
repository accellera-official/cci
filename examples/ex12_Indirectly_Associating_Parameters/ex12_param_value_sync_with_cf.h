/*****************************************************************************
  Copyright 2006-2014 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010-2015 CircuitSutra Technologies Pvt. Ltd.
  All rights reserved.

  Copyright 2016 Ericsson
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
 *  @file     param_value_sync_with_cf.h
 *  @brief    This file declares and defines the 'param_value_sync_with_cf' class that
 *            registers callbacks on the cci-parameters of the owner modules.  This
 *            registration of callbacks is in order to establish synchronization between
 *            the respective cci-parameters
 *  @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date     24th August, 2011 (Wednesday)
 */

#ifndef EXAMPLES_EX12_INDIRECTLY_ASSOCIATING_PARAMETERS_EX12_PARAM_VALUE_SYNC_WITH_CF_H_
#define EXAMPLES_EX12_INDIRECTLY_ASSOCIATING_PARAMETERS_EX12_PARAM_VALUE_SYNC_WITH_CF_H_

#include <cci_configuration>
#include <cassert>
#include <vector>
#include <sstream>
#include <string>

/**
 *  @class  ex12_param_value_sync_with_cf
 *  @brief  This module registers the post_write callbakcs on the owners parameters
 *          in order to update an owner cci_parameter directly when another cci_parameter
 *          value is modified
 */
SC_MODULE(ex12_param_value_sync_with_cf) {
 public:
  SC_HAS_PROCESS(ex12_param_value_sync_with_cf);

  /**
   *  @fn     ex12_param_value_sync_with_cf(sc_core::sc_module_name _name, std::vector<cci::cci_param_handle> BaseParamList)
   *  @brief  The class constructor
   *  @param  _name The name of the class
   *  @param  BaseParamList The list of CCI parameter handles
   *  @return void
   */
  ex12_param_value_sync_with_cf(sc_core::sc_module_name _name,
      std::vector<cci::cci_param_handle> BaseParamList)
      // Define an originator in order to get hold of the default broker
      : ValueSyncOriginator("ValueSyncOriginator") {
    // Get handle of the broker responsible for the class/module
    ValueSyncBrokerIF =
        &cci::cci_broker_manager::get_current_broker(ValueSyncOriginator);

    // Copy the list of selected base parameters to a local std::vector
    returnBaseParamList = BaseParamList;

    for (unsigned int i = 1; i < returnBaseParamList.size(); i++) {

      // Determine the 'conversion factor' b/w the two cci_base_params
      double conversion_factor = multiplyWithConversionFactor(
          returnBaseParamList[0].get_name(),
          returnBaseParamList[i].get_name());

      // Synchonize the second cci_base_param value to the first using the
      // conversion factor determined above
      synchValuesWithCF(returnBaseParamList[0],
                        returnBaseParamList[i],
                        conversion_factor);
    }
  }

  /**
   *  @fn     cci::callback_return_type write_callback(const cci::cci_base_param& _base_param_1, const cci::callback_type& cb_reason, cci::cci_base_param& _base_param_2)
   *  @brief  Post wirte and pre write callback implementation
   *  @param  _base_param_1 The parameter to sync from
   *  @param  cb_reason The reason for the callback
   *  @param  _base_param_2 The parameter to sync to
   *  @return The exit status of the callback
   */
  cci::callback_return_type
      write_callback(const cci::cci_param_handle& _base_param_1,
                     const cci::callback_type& cb_reason,
                     cci::cci_param_handle * _base_param_2,
                     double conv_fact) {
    // Decision on Pre-Write & Post-Write callbacks
    XREPORT("[PARAM_VALUE_SYNC - post_write callback] : Parameter Name : "
            << _base_param_1.get_name() << "\tValue : "
            << _base_param_1.get_cci_value().to_json());

    cci::cci_value freq = _base_param_1.get_cci_value();
    sc_assert( freq.is_number() );
    freq.set_double( freq.get_number() * conv_fact );
    _base_param_2->set_cci_value( freq );

    return cci::return_nothing;
  }

  /**
   *  @fn     double multiplyWithConversionFactor(std::string parent_str, std::string child_str)
   *  @brief  Function to calculate the conversion factor to be multiplied with
   *          the 'main_clk_Hz' parameter of the PARAM_VALUE_SYNC while assigning
   *          the same value to the owner parameters consitsent with their units
   *  @param  parent_str  The name of the parent
   *  @param  child_str The name of the child
   *  @return A double representing the conversion factor to convert from parent to child
   */
  double multiplyWithConversionFactor(std::string parent_str,
                                      std::string child_str) {
    double returnValue = 0.0;

    char* str1 = &parent_str[0];
    char* str2 = &child_str[0];
    char* ans1 = strrchr(str1, '_');
    char* ans2 = strrchr(str2, '_');
    std::string s1(ans1);
    std::string s2(ans2);

    if ((s1 == "_Hz") && (s2 == "_Hz")) {
      returnValue = 1.0;
    } else if ((s1 == "_Hz") && (s2 == "_KHz")) {
      returnValue = 0.001;
    } else if ((s1 == "_Hz") && (s2 == "_MHz")) {
      returnValue = 0.000001;
    } else if ((s1 == "_KHz") && (s2 == "_Hz")) {
      returnValue = 1000.0;
    } else if ((s1 == "_KHz") && (s2 == "_KHz")) {
      returnValue = 1.0;
    } else if ((s1 == "_KHz") && (s2 == "_MHz")) {
      returnValue = 0.001;
    } else if ((s1 == "_MHz") && (s2 == "_Hz")) {
      returnValue = 1000000.0;
    } else if ((s1 == "_MHz") && (s2 == "_KHz")) {
      returnValue = 1000.0;
    } else if ((s1 == "_MHz") && (s2 == "_MHz")) {
      returnValue = 1.0;
    }

    XREPORT("Parameter1_str: " << parent_str);
    XREPORT("Parameter2_str : " << child_str);
    XREPORT("ConversionFactor : " << returnValue);

    return returnValue;
  }

  /**
   *  @fn     void synchValuesWithCF(cci::cci_param_handle _param_handle_1, cci::cci_param_handle _param_handle_2, double conv_fact)
   *  @brief  Function for syncronizing the values of the cci_parameter of the
   *          OWNER modules via the PARAM_VALUE_SYNC.
   *  @param  _param_handle_1 The first parameter
   *  @param  _param_handle_2 The second parameter
   *  @param  conv_fact The conversion factor to convert from param 1 to param 2
   *  @return void
   */
  void synchValuesWithCF(cci::cci_param_handle _param_handle_1,
                         cci::cci_param_handle _param_handle_2,
                         double conv_fact) {
    // In order to synchronize even the default values of the owner modules,
    // use cci_base_param of one parameter as reference, write the same value
    // (using conv_fact) to the other pararmeter's cci_base_param using
    // generic cci_value APIs manually
    cci::cci_value freq = _param_handle_1.get_cci_value();
    sc_assert( freq.is_number() );
    freq.set_double( freq.get_number() * conv_fact );
    _param_handle_2.set_cci_value( freq );

    // TODO: fixme
    /*post_write_cb_vec.push_back(
        _param_handle_1.register_callback(cci::post_write,
                                         this,
                                         cci::bind(&ex12_param_value_sync_with_cf::write_callback,
                                                   this, _1, _2,
                                                   _param_handle_2, conv_fact)));

    post_write_cb_vec.push_back(
        _param_handle_2->register_callback(cci::post_write,
                                         this,
                                         cci::bind(&ex12_param_value_sync_with_cf::write_callback,
                                                   this, _1, _2,
                                                   _param_handle_1, (1.0 / conv_fact))));*/
  }

 private:
  cci::cci_originator ValueSyncOriginator; ///< Declaring a CCI configuration broker interface instance
  cci::cci_broker_if* ValueSyncBrokerIF; ///< Declaring a CCI configuration broker interface instance
  std::vector<cci::shared_ptr<cci::callb_adapt> > post_write_cb_vec; ///< Callback Adaptor Objects
  std::vector<cci::cci_param_handle> returnBaseParamList; ///< vector storing the owner param references to CCI parameter handles
};
// ex12_param_value_sync_with_cf

#endif  // EXAMPLES_EX12_INDIRECTLY_ASSOCIATING_PARAMETERS_EX12_PARAM_VALUE_SYNC_WITH_CF_H_
