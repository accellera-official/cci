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

#include <cci_configuration>
#include <systemc.h>
#include <iostream>
#include <string>
#include "cci_cfg/cci_originator.h"
#include "cci_cfg/cci_broker_manager.h"
/**
 *  @struct user_data_type
 *  @brief  User defined data structure
 */
struct user_data_type {
  friend std::ostream& operator <<(std::ostream& os, const user_data_type& ud);
  friend bool operator==(const user_data_type&, const user_data_type&);

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
    : public cci::cci_param_if {

	// Virtual function in cci_param_impl_if
  /**
   *  @fn     void set(const user_data_type& val)
   *  @brief  Function to set the value of the parameter
   *  @param  val The value to assign to the parameter
   *  @return void
   */
  void set(const user_data_type& val);

  /**
   *  @fn     user_data_type& get_value()
   *  @brief  Function to get the value of the parameter
   *  @return user's parameter value
   */
  const user_data_type& get_value() const;

  /**
   *  @fn     const void* get_raw_value() const
   *  @brief  Function to get the user defined type for the parameter
   *  @return The pointer to user_data_type of the parameter.
   */
  const void * get_raw_value(const cci::cci_originator&) const;

   /**
   *  @fn     void set_raw_value(const void *vp, const cci::cci_originator &originator)
   *  @brief  Function to set the value of the parameter.
   *  @param  vp The pointer to the value of assignment
   *  @param  originator reference to the originator
   *  @return void
   */
  void set_raw_value(const void *vp, const cci::cci_originator &originator);

  /**
   *  @fn     void set_raw_value(const void *vp, const void *pwd)
   *  @brief  Function to set the value of the parameter along with the lock password.
   *  @param  vp The pointer to the value of assignment
   *  @param  pwd  The password for the parameter
   *  @param  originator reference to the originator
   *  @return void
   */
  void set_raw_value(const void *vp, const void *pwd,
      const cci::cci_originator &originator);

  /**
   *  @fn     cci::cci_value get_default_cci_value()
   *  @brief  Retrieve the default CCI value of the parameter
   *  @return The user data type for the value of the parameter
   */
  cci::cci_value get_default_cci_value() const;

  /**
   *  @fn     const user_data_type& get_raw_default_value()
   *  @brief  Retrieve the default value of the parameter
   *  @return The user data type for the value of the parameter
   */
  const void* get_raw_default_value() const;

  // Virtual function in cci_base_param_impl_if

  /**
   *  @fn     const cci::cci_data_type get_data_category() const
   *  @brief  Function to retrive the basic type of the parameter
   *  @return The basic type.
   */
  cci::cci_param_data_category get_data_category() const;

  /**
   *  @fn     void set_cci_value(const cci::cci_value& val)
   *  @brief  Function to set the value of the parameter using cci_value
   *  @param  val The val to assign to the parameter
   *  @return void
   */
  void set_cci_value(const cci::cci_value& val);

  /**
   *  @fn     void set_value(const cci::cci_value& val,const cci::cci_originator& originator)
   *  @brief  Function to set the value of the parameter
   *  @param  val The val to assign to the parameter
   *  @param  originator reference to the originator
   *  @return void
   */
  void set_cci_value(const cci::cci_value& val, const cci::cci_originator& originator);

  /**
   *  @fn     cci::cci_value get_cci_value() const
   *  @brief  Function to retrieve the cci_value of the parameter
   *  @return The cci_value of the parameter
   */
  cci::cci_value get_cci_value() const;

  /**
   *  @fn     cci::cci_value get_cci_value(const cci::cci_originator&) const
   *  @brief  Function to retrieve the cci_value of the parameter
   *  @return The cci_value of the parameter
   */
  cci::cci_value get_cci_value(const cci::cci_originator&) const;

  /**
   *  @fn     cci::cci_param_mutable_type get_mutable_type() const
   *  @brief  Function to retrieve the parameter mutable type
   *  @return The mutable type of the parameter
   */
  cci::cci_param_mutable_type get_mutable_type() const;

  /**
   *  @fn     void set_description(const std::string& desc)
   *  @brief  Function to add description (descirption) to the parameter
   *  @param  desc The description to add
   *  @return void
   */
  void set_description(const std::string& desc);

  /**
   *  @fn     std::string get_description() const
   *  @brief  Function to retrieve the description of the parameter
   *  @return The description in a string
   */
  std::string get_description() const;


  /// @copydoc cci_param_untyped::add_metadata
  void add_metadata(const std::string &name, const cci::cci_value &value,
	  const std::string &desc = "");

  /// @copydoc cci_param_untyped::get_metadata
  cci::cci_value_map get_metadata() const;


  /**
   *  @fn     bool is_default_value()
   *  @brief  Function to return true if the value if still the default
   *  @return True of false depending on whether the value is still the default or not
   */
  bool is_default_value() const;

  /**
   *  @fn     void reset
   *  @brief  reset the param to the default/initial value
   *  @return none
   */
  void reset();

  
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
   *  @fn     std::type_info& get_type_info() const
   *  @brief  Function to get the type info of a parameter
   *  @return Type info of the parameter
   */
  const std::type_info& get_type_info() const;

  /**
   *  @fn     const std::string& get_name() const
   *  @brief  Function to get the name of the parameter
   *  @return The string representation of the name of the parameter
   */
  const std::string& get_name() const;

  /**
   *  @fn     bool is_handle()
   *  @brief  check if this is an handle
   *  @return True or false on whether it is an handle
   */
  bool is_handle() const;

  /**
   * @param cb Untyped pre write callback handle
   * @param cci_originator Originator
   *
   * @return Untyped callback handle
   */
  cci::cci_callback_untyped_handle
  register_pre_write_callback(const cci::cci_callback_untyped_handle &cb,
                              const cci::cci_originator &orig);

  /**
   * @param cb Untyped pre write callback handle
   * @param cci_originator Originator
   *
   * @return True if unregister is a success. Otherwise False.
   */
  bool
  unregister_pre_write_callback(const cci::cci_callback_untyped_handle &cb,
                                const cci::cci_originator &orig);

  /**
   * @param cb Untyped post write callback handle
   * @param cci_originator Originator
   *
   * @return Untyped callback handle
   */
  cci::cci_callback_untyped_handle
  register_post_write_callback(const cci::cci_callback_untyped_handle &cb,
                               const cci::cci_originator &orig);

  /**
   * @param cb Untyped post write callback handle
   * @param cci_originator Originator
   *
   * @return True if unregister is a success. Otherwise False.
   */
  bool
  unregister_post_write_callback(const cci::cci_callback_untyped_handle &cb,
                                 const cci::cci_originator &orig);

  /**
   * @param cb Untyped pre read callback handle
   * @param cci_originator Originator
   *
   * @return Untyped callback handle
   */
  cci::cci_callback_untyped_handle
  register_pre_read_callback(const cci::cci_callback_untyped_handle &cb,
                             const cci::cci_originator &orig);

  /**
   * @param cb Untyped pre read callback handle
   * @param cci_originator Originator
   *
   * @return True if unregister is a success. Otherwise False.
   */
  bool
  unregister_pre_read_callback(const cci::cci_callback_untyped_handle &cb,
                               const cci::cci_originator &orig);

  /**
   * @param cb Untyped post read callback handle
   * @param cci_originator Originator
   *
   * @return Untyped callback handle
   */
  cci::cci_callback_untyped_handle
  register_post_read_callback(const cci::cci_callback_untyped_handle &cb,
                              const cci::cci_originator &orig);

  /**
   * @param cb Untyped post read callback handle
   * @param cci_originator Originator
   *
   * @return True if unregister is a success. Otherwise False.
   */
  bool
  unregister_post_read_callback(const cci::cci_callback_untyped_handle &cb,
                                const cci::cci_originator &orig);

  /**
   * @param cci_originator Originator
   *
   * @return True if unregister is a success. Otherwise False.
   */
  bool unregister_all_callbacks(const cci::cci_originator &orig);

  /**
   *  @fn     bool has_callback()
   *  @brief  Function to determine if the parameter has callbacks registered or not
   *  @return True or false depending on whether callbacks are registered
   */
  bool has_callbacks() const;

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
  *  @fn     bool equals(const cci_param_handle& rhs) const;
  *  @param  rhs reference to another cci_param_if implementation
  *  @return True if both values are equal
  */
  bool equals(const cci::cci_param_if& rhs) const;

  /**
  *  @fn     bool equals(const cci_param_untyped_handle& rhs) const;
  *  @param  rhs reference to another cci_param_untyped_handle implementation
  *  @return True if both values are equal
  */
  bool equals(const cci::cci_param_untyped_handle &rhs) const ;

  /// Initialize 
  void init();
  
  /// Destroy
  void destroy();

  /// Add a parameter handle
  void add_param_handle(cci::cci_param_untyped_handle* param_handle);

  /// Remove a parameter handle
  void remove_param_handle(cci::cci_param_untyped_handle* param_handle);

  /// Get originator
  cci::cci_originator get_originator() const;
  
  /**
   *  @fn     cci::cci_originator* get_latest_write_originator() const
   *  @brief  Retrieve the last person to write to the parameter
   *  @return The last person to write to the parameter
   */
  cci::cci_originator& get_latest_write_originator() const {
    static cci::cci_originator originator = cci::cci_originator();
    return originator; /* TODO */
  }

  cci::cci_param_untyped_handle create_param_handle(const cci::cci_originator& originator) const{
    return cci::cci_param_untyped_handle(
            (*(const_cast<cci_param_user_data_type*>(this))),
            cci::cci_originator("cci_param_user_data_type"));
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
  cci::cci_value c_value;  ///< cci_value of the parameter
  void * l_password;  ///< lock password
  std::string description;  ///< parameter description
  std::string nam;  ///< the name of the parameter
  cci::cci_broker_handle m_broker; ///broker pointer used to register parameter
};
// ex19_parameter_impl_by_abc

#endif  // EXAMPLES_EX19_API_AND_PARAMETER_IMPLEMENTATIONS_FROM_DIFFERENT_PROVIDERS_EX19_PARAMETER_IMPL_BY_ABC_H_
