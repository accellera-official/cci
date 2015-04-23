/*****************************************************************************
  Copyright 2006-2014 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010-2015 CircuitSutra Technologies Pvt. Ltd.
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
 *  @file     parameter_impl_by_abc.h
 *  @brief    This file demonstrates the user how to declare and define a parameter
 *            implementation other than what is provided (by GreenSoCs as reference
 *            implementation)
 *  @date     14th December, 2011
 *  @author   Girish Verma, CircuitSutra Technologies     <girish@circuitsutra.com>
 *            P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 */

#ifndef EXAMPLES_EX19_API_AND_PARAMETER_IMPLEMENTATIONS_FROM_DIFFERENT_PROVIDERS_EX19_PARAMETER_IMPL_BY_ABC_H_
#define EXAMPLES_EX19_API_AND_PARAMETER_IMPLEMENTATIONS_FROM_DIFFERENT_PROVIDERS_EX19_PARAMETER_IMPL_BY_ABC_H_

#include <cci>
#include <systemc.h>
#include <iostream>
#include <string>

/**
 *  @struct user_data_type
 *  @brief  User defined data structure
 */
struct user_data_type {
  friend std::ostream& operator <<(std::ostream& os, const user_data_type& ud);

  user_data_type(sc_dt::uint64 saddr, sc_dt::uint64 daddr, unsigned idx)
      : s_address(saddr),
        d_address(daddr),
        index(idx) {
  }

  sc_dt::uint64 s_address;
  sc_dt::uint64 d_address;
  unsigned      index;
};
// End of USER DATA STRUCTURE

/**
 *  @struct cci_param_user_data_type
 *  @brief  User defined type cci parameter struct.
 */
struct cci_param_user_data_type
    : public cci::cnf::cci_param_impl_if<user_data_type,
                                         cci::cnf::mutable_param> {
  // Virtual function in cci_param_impl_if
  /**
   *  @fn     void set(const user_data_type& val)
   *  @brief  Function to set the value of the parameter
   *  @param  val The value to assign to the parameter
   *  @return void
   */
  void set(const user_data_type& val);

  /**
   *  @fn     const user_data_type& get() const
   *  @brief  Function to get the user defined type for the parameter
   *  @return The user_data_type of the parameter.
   */
  const user_data_type& get() const;

  /**
   *  @fn     void set(const user_data_type& val, void* lock_pwd)
   *  @brief  Function to set the value of the parameter along with the lock password.
   *  @param  val The value to assign
   *  @param  lock_pwd  The password for the parameter
   *  @return void
   */
  void set(const user_data_type& val, void* lock_pwd);

  /**
   *  @fn     std::string json_serialize(const user_data_type& val) const
   *  @brief  Serialize the value to a string
   *  @param  val The value to serialize
   *  @return A string representation of the value
   */
  std::string json_serialize(const user_data_type& val) const;

  /**
   *  @fn     void json_deserialize(user_data_type& target_val, const std:string& str)
   *  @brief  Function deserialize the string representation into the target value type.
   *  @param  target_val  The user defined value type to assign the string to
   *  @param  str The string to be deserialized into the value
   *  @return void
   */
  void json_deserialize(user_data_type& target_val, const std::string& str);

  /**
   *  @fn     const user_data_type& get_default_value()
   *  @brief  Retrieve the default value of the parameter
   *  @return The user data type for the value of the parameter
   */
  const user_data_type& get_default_value();

  // Virtual function in cci_base_param_impl_if

  /**
   *  @fn     void json_deserialize(const std::string& json_string)
   *  @brief  Function to deserialize the string to the value
   *  @param  json_string The JSON string to be deserialized
   *  @return void
   */
  void json_deserialize(const std::string& json_string);

  /**
   *  @fn     std::string json_serilize() const
   *  @brief  Function to serialize the value of the parameter into a JSON string
   *  @return A string representing the value of the parameter
   */
  std::string json_serialize() const;

  /**
   *  @fn     const cci::cnf::basic_param_type get_basic_type() const
   *  @brief  Function to retrive the basic type of the parameter
   *  @return The basic type.
   */
  const cci::cnf::basic_param_type get_basic_type() const;

  /**
   *  @fn     void set_value(const cci::cnf::cci_value& val)
   *  @brief  Function to set the value of the parameter
   *  @param  val The val to assign to the parameter
   *  @return void
   */
  void set_value(const cci::cnf::cci_value& val);

  /**
   *  @fn     cci::cnf::cci_value get_value() const
   *  @brief  Function to retrieve the value of the parameter
   *  @return The cci_value of the parameter
   */
  cci::cnf::cci_value get_value() const;

  /**
   *  @fn     void set_documentation(const std::string& doc)
   *  @brief  Function to add documentation (descirption) to the parameter
   *  @param  doc The description to add
   *  @return void
   */
  void set_documentation(const std::string& doc);

  /**
   *  @fn     std::string get_documentation() const
   *  @brief  Function to retrieve the documentation of the parameter
   *  @return The documentation in a string
   */
  std::string get_documentation() const;

  /**
   *  @fn     bool is_default_value()
   *  @brief  Function to return true if the value if still the default
   *  @return True of false depending on whether the value is still the default or not
   */
  bool is_default_value();

  /**
   *  @fn     bool is_invalid_value()
   *  @brief  Function to determine whether the value in the parameter is valid
   *  @return True or false depending on whether the value is valid or not
   */
  bool is_invalid_value();

  /**
   *  @fn     void set_invalid_value()
   *  @brief  Function to set the value of the parameter as invalid
   *  @return void
   */
  void set_invalid_value();

  /**
   *  @fn     bool is_initial_value() const
   *  @brief  Function to determine if the value is the initial value
   *  @return True or false depending on whether the value is the initial value or not
   */
  bool is_initial_value() const;

  /**
   *  @fn     const std::string& get_name() const
   *  @brief  Function to get the name of the parameter
   *  @return The string representation of the name of the parameter
   */
  const std::string& get_name() const;

  /**
   *  @fn     cci::chared_ptr<cci::cnf::callb_adapt> register_callback(const cci::cnf::callback_type type, void* observer, cci::cnf::param_callb_func_ptr function)
   *  @brief  Function to register callbacks
   *  @param  type  The type of callback to register
   *  @param  observer  Observer for the callback
   *  @param  function  Callback function to be called
   *  @return Pointer to the callback function
   */
  cci::shared_ptr<cci::cnf::callb_adapt>
      register_callback(const cci::cnf::callback_type type, void* observer,
                        cci::cnf::param_callb_func_ptr function);

  /**
   *  @fn     cci::chared_ptr<cci::cnf::callb_adapt> register_callback(const cci::cnf::callback_type type, cci::shared_ptr<cci::cnf::callb_adapt> callb)
   *  @brief  Function to register callbacks
   *  @param  type  The type of callback to register
   *  @param  callb Pointer to the callback function
   *  @return Pointer to the callback function
   */
  cci::shared_ptr<cci::cnf::callb_adapt>
      register_callback(const cci::cnf::callback_type type,
                        cci::shared_ptr<cci::cnf::callb_adapt> callb);

  /**
   *  @fn     void unregister_all_callbacks(void* observer)
   *  @brief  Function to unregister all callbacks with a given observer
   *  @param  observer  The observer to unregister the callbacks for
   *  @return void
   */
  void unregister_all_callbacks(void* observer);

  /**
   *  @fn     bool unregister_callback(cci::shared_ptr<cci::cnf::callb_adpt> callb)
   *  @brief  Function to unregister a particular callback function
   *  @param  callb The function to be unregistered
   *  @return True or false depending on whether it was successfully unregistered
   */
  bool unregister_callback(cci::shared_ptr<cci::cnf::callb_adapt> callb);

  /**
   *  @fn     bool unregister_callback(cci::cnf::callb_adapt* callb)
   *  @brief  Function to unregister a particular callback function
   *  @param  callb The function to be unregistered
   *  @return A true or false depending on whether the function was unregistered or not
   */
  bool unregister_callback(cci::cnf::callb_adapt * callb);

  /**
   *  @fn     bool has_callback()
   *  @brief  Function to determine if the parameter has callbacks registered or not
   *  @return True or false depending on whether callbacks are registered
   */
  bool has_callbacks();

  /**
   *  @fn     bool lock(void* pwd = NULL)
   *  @brief  Lock the paramter
   *  @param  pwd The password to unlock the paramter
   *  @return True or false on whether the lock was successful
   */
  bool lock(void* pwd = NULL);

  /**
   *  @fn     bool unlock(void* pwd = NULL)
   *  @brief  Function to unlock the parameter
   *  @param  pwd The password to unlock the parameter
   *  @return True or false depending on whether the parameter was unlocked
   */
  bool unlock(void* pwd = NULL);

  /**
   *  @fn     bool is_locked()
   *  @brief  Function to determine if the parameter is locked or not
   *  @return True or false depending on whether or not the parameter is locked
   */
  bool is_locked() const;

  /**
   *  @fn     cci::cnf::cci_originator* get_latest_write_originator() const
   *  @brief  Retrieve the last person to write to the parameter
   *  @return The last person to write to the parameter
   */
  cci::cnf::cci_originator* get_latest_write_originator() const {
    return NULL; /* TODO */
  }

  /**
   *  @fn     cci_param_user_data_type(const std::string& _name, const user_data_type& _dvalue)
   *  @brief  The constructor for the class
   *  @param  _name The name for the parameter
   *  @param  _dvalue The value to assign to the parameter
   *  @return void
   */
  cci_param_user_data_type(const std::string& _name,
                           const user_data_type & _dvalue);

  // data member
  user_data_type value; ///< The value of the parameter
  user_data_type default_value; ///< The default value for the parameter
  bool default_flag;  ///< flag to show whether it is default value
  bool invalid_flag;  ///< flag to show value is invalid
  bool initial_flag;  ///< flag to show value is the initial value
  bool lock_flag; ///< flag to indicate the value is the initial value
  bool callback_flag; ///< flag to indicate callbacks are registered
  cci::cnf::cci_value c_value;  ///< cci_value of the parameter
  void * l_password;  ///< lock password
  std::string documentation;  ///< parametere documentation
  std::string nam;  ///< the name of the parameter
};
// ex19_parameter_impl_by_abc

#endif  // EXAMPLES_EX19_API_AND_PARAMETER_IMPLEMENTATIONS_FROM_DIFFERENT_PROVIDERS_EX19_PARAMETER_IMPL_BY_ABC_H_
