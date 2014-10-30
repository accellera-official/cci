/*******************************************************************************
 * The following code is derived, directly or indirectly, from the SystemC
 * source code Copyright (c) 1996-2010 by all Contributors.
 * All Rights reserved.
 * 
 * The contents of this file are subject to the restrictions and limitations
 * set forth in the SystemC Open Source License Version 2.2.0 (the "License");
 * One may not use this file except in compliance with such restrictions and
 * limitations.  One may obtain instructions on how to receive a copy of the
 * License at http://www.systemc.org/.  Software distributed by Contributors
 * under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 * ANY KIND, either express or implied. See the License for the specific
 * language governing rights and limitations under the License.
 * *******************************************************************************/

/**
 * @file     param_value_sync_with_cf.h
 * @brief    This file declares and defines the 'param_value_sync_with_cf' class that
 *           registers callbacks on the cci-parameters of the owner modules.  This
 *           registration of callbacks is in order to establish synchronization between
 *           the respective cci-parameters
 * @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date     24th August, 2011 (Wednesday)
 */

#ifndef EXAMPLES_EX12_INDIRECTLY_ASSOCIATING_PARAMETERS_EX12_PARAM_VALUE_SYNC_WITH_CF_H_
#define EXAMPLES_EX12_INDIRECTLY_ASSOCIATING_PARAMETERS_EX12_PARAM_VALUE_SYNC_WITH_CF_H_

#include <cci>
#include <cassert>
#include <vector>
#include <sstream>
#include <string>

/// This module registers the post_write callbakcs on the owners parameters
/// in order to update an owner cci_parameter directly when another
/// cci_parameter value is modified
SC_MODULE(ex12_param_value_sync_with_cf) {
 public:
  SC_HAS_PROCESS(ex12_param_value_sync_with_cf);

  ex12_param_value_sync_with_cf(sc_module_name _name,
      std::vector<cci::cnf::cci_base_param *> BaseParamList)
      /// Define an originator in order to get hold of the default broker
      : ValueSyncOriginator("ValueSyncOriginator") {
    /// Get handle of the broker responsible for the class/module
    ValueSyncBrokerIF =
        &cci::cnf::cci_broker_manager::get_current_broker(ValueSyncOriginator);

    /// Copy the list of selected base parameters to a local std::vector
    returnBaseParamList = BaseParamList;

    for (unsigned int i = 1; i < returnBaseParamList.size(); i++) {
      float conversion_factor;

      /// Determine the 'conversion factor' b/w the two cci_base_params
      conversion_factor = multiplyWithConversionFactor(
          returnBaseParamList[0]->get_name(),
          returnBaseParamList[i]->get_name());

      /// Synchonize the second cci_base_param value to the first using the
      /// conversion factor determined above
      synchValuesWithCF(returnBaseParamList[0],
                        returnBaseParamList[i],
                        conversion_factor);
    }
  }

  /// post write and pre write callback implementation
  cci::cnf::callback_return_type
      write_callback(const cci::cnf::cci_base_param& _base_param_1,
                     const cci::cnf::callback_type& cb_reason,
                     cci::cnf::cci_base_param * _base_param_2,
                     float conv_fact) {
    // Decision on Pre-Write & Post-Write callbacks
    XREPORT("[PARAM_VALUE_SYNC - post_write callback] : Parameter Name : "
            << _base_param_1.get_name() << "\tValue : "
            << _base_param_1.json_serialize());

    float freq = atof((_base_param_1.json_serialize()).c_str());
    float operand1 = freq * conv_fact;
    std::stringstream ss;
    ss.clear();
    ss.str("");
    ss << operand1;
    _base_param_2->json_deserialize(ss.str());

    return cci::cnf::return_nothing;
  }

  /// function to calculate the conversion factor to be multiplied with the
  /// 'main_clk_Hz' parameter of the PARAM_VALUE_SYNC while assigning the same
  /// value to the owner parameters consitsent with their units
  float multiplyWithConversionFactor(std::string parent_str,
                                     std::string child_str) {
    float returnValue = 0.0;

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

  /// function for syncronizing the values of the cci_parameter of the OWNER
  /// modules via the PARAM_VALUE_SYNC
  void synchValuesWithCF(cci::cnf::cci_base_param * _base_param_1,
                         cci::cnf::cci_base_param * _base_param_2,
                         float conv_fact) {
    /// In order to synchronize even the default values of the owner modules,
    /// use cci_base_param of one parameter as reference, write the same value
    /// (using CF) to the other pararmeter's cci_base_param using
    /// JSON serialize/deserialize APIs manually
    float CF = multiplyWithConversionFactor(_base_param_1->get_name(),
                                            _base_param_2->get_name());
    float freq = atof((_base_param_1->json_serialize()).c_str());
    float operand1 = freq * CF;
    std::stringstream ss;
    ss.clear();
    ss.str("");
    ss << operand1;
    _base_param_2->json_deserialize(ss.str());

    post_write_cb_vec.push_back(
        _base_param_1->register_callback(cci::cnf::post_write,
                                         this,
                                         cci::bind(&ex12_param_value_sync_with_cf::write_callback,
                                                   this, _1, _2,
                                                   _base_param_2, conv_fact)));

    post_write_cb_vec.push_back(
        _base_param_2->register_callback(cci::cnf::post_write,
                                         this,
                                         cci::bind(&ex12_param_value_sync_with_cf::write_callback,
                                                   this, _1, _2,
                                                   _base_param_1, (1.0 / conv_fact))));
  }

 private:
  // Declaring a CCI configuration broker interface instance
  cci::cnf::cci_originator ValueSyncOriginator;

  // Declaring a CCI configuration broker interface instance
  cci::cnf::cci_cnf_broker_if* ValueSyncBrokerIF;

  // Callback Adaptor Objects
  std::vector<cci::shared_ptr<cci::cnf::callb_adapt> > post_write_cb_vec;

  // vector storing the owner param references to CCI base parameter pointers
  std::vector<cci::cnf::cci_base_param*> returnBaseParamList;
};
// ex12_param_value_sync_with_cf

#endif  // EXAMPLES_EX12_INDIRECTLY_ASSOCIATING_PARAMETERS_EX12_PARAM_VALUE_SYNC_WITH_CF_H_
