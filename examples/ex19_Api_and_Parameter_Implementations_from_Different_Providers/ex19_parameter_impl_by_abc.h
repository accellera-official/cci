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

/*!
 * \file     parameter_impl_by_abc.h
 * \brief    This file demonstrates the user how to declare and define a parameter
 *           implementation other than what is provided (by GreenSoCs as reference
 *           implementation)
 * \date     14th December, 2011
 * \author   Girish Verma, CircuitSutra Technologies     <girish@circuitsutra.com>
 *           P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 */

#ifndef EXAMPLES_EX19_API_AND_PARAMETER_IMPLEMENTATIONS_FROM_DIFFERENT_PROVIDERS_EX19_PARAMETER_IMPL_BY_ABC_H_
#define EXAMPLES_EX19_API_AND_PARAMETER_IMPLEMENTATIONS_FROM_DIFFERENT_PROVIDERS_EX19_PARAMETER_IMPL_BY_ABC_H_

#include <stdint.h>
#include <cci>
#include <core/cci_shared_ptr.h>
#include <systemc.h>
#include <iostream>
#include <string>

/// A user-defined data structure
struct user_data_type {
  friend std::ostream& operator <<(std::ostream& os, const user_data_type& ud);

  user_data_type(uint64_t saddr, uint64_t daddr, uint32_t idx)
      : s_address(saddr),
        d_address(daddr),
        index(idx) {
  }

  uint64_t s_address;
  uint64_t d_address;
  uint32_t index;
};
// End of USER DATA STRUCTURE

struct cci_param_user_data_type
    : public cci::cnf::cci_param_impl_if<user_data_type,
                                         cci::cnf::mutable_param> {
  // Virtual function in cci_param_impl_if
  void set(const user_data_type& val);

  const user_data_type& get() const;

  void set(const user_data_type& val, void* lock_pwd);

  std::string json_serialize(const user_data_type& val) const;

  void json_deserialize(user_data_type& target_val, const std::string& str);

  const user_data_type& get_default_value();

  // Virtual function in cci_base_param_impl_if
  void json_deserialize(const std::string& json_string);

  std::string json_serialize() const;

  const cci::cnf::basic_param_type get_basic_type() const;

  void set_value(const cci::cnf::cci_value& val);

  cci::cnf::cci_value get_value();

  void set_documentation(const std::string& doc);

  std::string get_documentation() const;

  bool is_default_value();

  bool is_invalid_value();

  void set_invalid_value();

  bool is_initial_value() const;

  const std::string& get_name() const;

  cci::shared_ptr<cci::cnf::callb_adapt>
      register_callback(const cci::cnf::callback_type type, void* observer,
                        cci::cnf::param_callb_func_ptr function);

  cci::shared_ptr<cci::cnf::callb_adapt>
      register_callback(const cci::cnf::callback_type type,
                        cci::shared_ptr<cci::cnf::callb_adapt> callb);

  void unregister_all_callbacks(void* observer);

  bool unregister_callback(cci::shared_ptr<cci::cnf::callb_adapt> callb);

  bool unregister_callback(cci::cnf::callb_adapt * callb);

  bool has_callbacks();

  bool lock(void* pwd = NULL);

  bool unlock(void* pwd = NULL);

  bool is_locked() const;

  cci::cnf::cci_originator* get_latest_write_originator() const {
    return NULL; /* TODO */
  }

  // Constructor
  cci_param_user_data_type(const std::string& _name,
                           const user_data_type & _dvalue);

  // data member
  user_data_type value;
  user_data_type default_value;
  bool default_flag;
  bool invalid_flag;
  bool initial_flag;
  bool lock_flag;
  bool callback_flag;
  cci::cnf::cci_value c_value;
  void * l_password;
  std::string documentation;
  std::string nam;
};
/// ex19_parameter_impl_by_abc

#endif  // EXAMPLES_EX19_API_AND_PARAMETER_IMPLEMENTATIONS_FROM_DIFFERENT_PROVIDERS_EX19_PARAMETER_IMPL_BY_ABC_H_
