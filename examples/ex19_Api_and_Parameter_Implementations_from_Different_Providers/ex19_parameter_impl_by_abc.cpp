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
 *  @file      parameter_impl_by_abc.cpp
 *  @brief     This class demonstrates the parameter implementation by a
 *             different provider
 *  @date      14th December, 2011
 *  @author    Girish Verma, CircuitSutra Technologies     <girish@circuitsutra.com>
 *             P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 */

#include "ex19_parameter_impl_by_abc.h"

/**
 *  @fn     std::ostream& operator <<(std::ostream& os, const user_data_type& ud)
 *  @brief  Implementation of the << operator for the user defined type
 *  @param  os  The output stream
 *  @param  ud  The user defined type
 *  @return The output stream for the user defined data type
 */
std::ostream& operator <<(std::ostream& os, const user_data_type& ud) {
  os << "\n\t[PARAM IMPL] : Source Address : " << ud.s_address
     << "\tDestination Address : "
     << ud.d_address << "\tIndex : " << ud.index;

  return os;
}

/**
*  @fn     bool operator==(const user_data_type&, const user_data_type&)
*  @brief  Implementation of the << operator for the user defined type
*  @param  lhs  User defined type
*  @param  rhs  User defined type
*  @return true if both user defined types are equal
*/
bool operator==(const user_data_type& lhs, const user_data_type& rhs)
{
	return lhs.s_address == rhs.s_address &&
		lhs.d_address == rhs.d_address &&
		lhs.index == rhs.index;
}

/**
 *  @fn     cci_param_user_data_type::cci_param_user_data_type(const std::string& _name, const user_data_type& _dvalue)
 *  @brief  The class constructor
 *  @param  _name The name of the parameter
 *  @param  _dvalue The value to assign to the parameter
 *  @return void
 */
cci_param_user_data_type::cci_param_user_data_type(
    const std::string& _name, const user_data_type& _dvalue)
    : value(_dvalue),
      default_value(_dvalue),
      default_flag(true),
      invalid_flag(false),
      lock_flag(false),
      callback_flag(false),
      l_password(NULL),
      description("") {
  nam = _name;
  /* DO some hack for cci_value */
}

// Virtual function in cci_param_impl_if

/**
 *  @fn     void cci_param_user_data_type::set(const user_data_type& val)
 *  @brief  Function to set the value of the parameter
 *  @param  val The value to assign to the parameter
 *  @return void
 */
void cci_param_user_data_type::set(const user_data_type& val) {
  std::cout << "Function cci_param_user_data_type::set Called " << std::endl;
  value = val;
}

/**
 *  @fn     const user_data_type& cci_param_user_data_type::get() const
 *  @brief  Function to get the user defined type for the parameter
 *  @return The user_data_type of the parameter.
 */
const void* cci_param_user_data_type::get() const {
  std::cout << "Function cci_param_user_data_type::get Called " << std::endl;
  return &value;
}

/**
*  @fn     void cci_param_user_data_type::set(const user_data_type& val, void* lock_pwd)
*  @brief  Function to set the value of the parameter along with the lock password.
*  @param  val The value to assign
*  @return void
*/
void cci_param_user_data_type::set(const void* val) {
	std::cout << "Function cci_param_user_data_type::set Called " << std::endl;
	if (l_password == NULL) {
		value = *static_cast<const user_data_type*>(val);
	}
}

/**
 *  @fn     void cci_param_user_data_type::set(const user_data_type& val, void* lock_pwd)
 *  @brief  Function to set the value of the parameter along with the lock password.
 *  @param  val The value to assign
 *  @param  lock_pwd  The password for the parameter
 *  @return void
 */
void cci_param_user_data_type::set(const void* val, const void* lock_pwd) {
  std::cout << "Function cci_param_user_data_type::set Called " << std::endl;
  if (l_password == lock_pwd) {
    value = *static_cast<const user_data_type*>(val);
  }
}

/**
 *  @fn     std::string cci_param_user_data_type::json_serialize(const user_data_type& val) const
 *  @brief  Serialize the value to a string
 *  @param  val The value to serialize
 *  @return A string representation of the value
 */
std::string cci_param_user_data_type::json_serialize(
    const user_data_type& val) const {
  std::cout << "Function cci_param_user_data_type::json_serialize Called " << std::endl;
  return std::string("not IMplemented");
}

/**
 *  @fn     void cci_param_user_data_type::json_deserialize(user_data_type& target_val, const std:string& str)
 *  @brief  Function deserialize the string representation into the target value type.
 *  @param  target_val  The user defined value type to assign the string to
 *  @param  str The string to be deserialized into the value
 *  @return void
 */
void cci_param_user_data_type::json_deserialize(user_data_type& target_val,
                                                const std::string& str) {
  std::cout << "Function cci_param_user_data_type::json_deserialize Called " << std::endl;
  std::cout << "With Json string " << str << std::endl;
}

/**
 *  @fn     const user_data_type& cci_param_user_data_type::get_default_value()
 *  @brief  Retrieve the default value of the parameter
 *  @return The user data type for the value of the parameter
 */
const void* cci_param_user_data_type::get_default_value() const {
  std::cout << "Function cci_param_user_data_type::get_default_value Called " << std::endl;

  return &default_value;
}

// Virtual function in cci_base_param_impl_if

/**
 *  @fn     void cci_param_user_data_type::json_deserialize(const std::string& json_string)
 *  @brief  Function to deserialize the string to the value
 *  @param  json_string The JSON string to be deserialized
 *  @return void
 */
void cci_param_user_data_type::json_deserialize(
    const std::string& json_string) {
  std::cout << "Function cci_param_user_data_type::json_deserialize Called " << std::endl;
  std::cout << "With Json string " << json_string << std::endl;
}

/**
 *  @fn     std::string cci_param_user_data_type::json_serialize() const
 *  @brief  Function to serialize the value of the parameter into a JSON string
 *  @return A string representing the value of the parameter
 */
std::string cci_param_user_data_type::json_serialize() const {
  std::cout << "Function cci_param_user_data_type::json_serialize Called " << std::endl;
  return std::string("Not implemented");
}

/**
 *  @fn     const cci::basic_param_type cci_param_user_data_type::get_basic_type() const
 *  @brief  Function to retrive the basic type of the parameter
 *  @return The basic type.
 */
cci::basic_param_type cci_param_user_data_type::get_basic_type() const {
  std::cout << "Function cci_param_user_data_type::get_basic_type Called " << std::endl;

  return cci::param_type_list;
}

/**
 *  @fn     void cci_param_user_data_type::set_value(const cci::cci_value& val)
 *  @brief  Function to set the value of the parameter
 *  @param  val The val to assign to the parameter
 *  @return void
 */
void cci_param_user_data_type::set_value(const cci::cci_value& val) {
  std::cout << "Function cci_param_user_data_type::set_value Called " << std::endl;
  c_value = val;
}

/**
 *  @fn     cci::cci_value cci_param_user_data_type::get_value() const
 *  @brief  Function to retrieve the value of the parameter
 *  @return The cci_value of the parameter
 */
cci::cci_value cci_param_user_data_type::get_value() const {
  std::cout << "Function cci_param_user_data_type::get_value Called " << std::endl;
  return c_value;
}

/**
 *  @fn     void cci_param_user_data_type::set_description(const std::string& doc)
 *  @brief  Function to add description (descirption) to the parameter
 *  @param  doc The description to add
 *  @return void
 */
void cci_param_user_data_type::set_description(const std::string& desc) {
  std::cout << "Function cci_param_user_data_type::set_description Called " << std::endl;

  description = desc;
}

/**
 *  @fn     std::string cci_param_user_data_type::get_description() const
 *  @brief  Function to retrieve the description of the parameter
 *  @return The description in a string
 */
std::string cci_param_user_data_type::get_description() const {
  std::cout << "Function cci_param_user_data_type::get_description Called " << std::endl;
  return description;
}

/**
 *  @fn     bool cci_param_user_data_type::is_default_value()
 *  @brief  Function to return true if the value if still the default
 *  @return True of false depending on whether the value is still the default or not
 */
bool cci_param_user_data_type::is_default_value() {
  std::cout << "Function cci_param_user_data_type::is_default_value Called " << std::endl;

  return default_flag;
}

/**
 *  @fn     bool cci_param_user_data_type::is_invalid_value()
 *  @brief  Function to determine whether the value in the parameter is valid
 *  @return True or false depending on whether the value is valid or not
 */
bool cci_param_user_data_type::is_invalid_value() {
  std::cout << "Function cci_param_user_data_type::is_invalid_valueCalled " << std::endl;

  return invalid_flag;
}

/**
 *  @fn     void cci_param_user_data_type::set_invalid_value()
 *  @brief  Function to set the value of the parameter as invalid
 *  @return void
 */
void cci_param_user_data_type::set_invalid_value() {
  std::cout << "Function cci_param_user_data_type::set_invalid_value Called " << std::endl;
  invalid_flag = true;
}

/**
 *  @fn     bool is_initial_value() const
 *  @brief  Function to determine if the value is the initial value
 *  @return True or false depending on whether the value is the initial value or not
 */
bool cci_param_user_data_type::is_initial_value() const {
  std::cout << "Function cci_param_user_data_type::is_initial_value Called " << std::endl;
  return initial_flag;
}

/**
 *  @fn     const std::string& cci_param_user_data_type::get_name() const
 *  @brief  Function to get the name of the parameter
 *  @return The string representation of the name of the parameter
 */
const std::string& cci_param_user_data_type::get_name() const {
  std::cout << "Function cci_param_user_data_type::get_name Called " << std::endl;
  std::cout << "Return Name = " << nam << std::endl;

  return nam;
}

/**
 *  @fn     cci::chared_ptr<cci::callb_adapt> cci_param_user_data_type::register_callback(const cci::callback_type type, void* observer, cci::param_callb_func_ptr function)
 *  @brief  Function to register callbacks
 *  @param  type  The type of callback to register
 *  @param  observer  Observer for the callback
 *  @param  function  Callback function to be called
 *  @return Pointer to the callback function
 */
cci::shared_ptr<cci::callb_adapt>
    cci_param_user_data_type::register_callback(const cci::callback_type type,
                                                void* observer,
                                                cci::param_callb_func_ptr function) {
  std::cout << "Function cci_param_user_data_type::register_callback Called " << std::endl;
  /* Complex Later */

  static cci::shared_ptr<cci::callb_adapt> dummy;
  return dummy;  // dummy return value for now
}

/**
 *  @fn     cci::chared_ptr<cci::callb_adapt> cci_param_user_data_type::register_callback(const cci::callback_type type, cci::shared_ptr<cci::callb_adapt> callb)
 *  @brief  Function to register callbacks
 *  @param  type  The type of callback to register
 *  @param  callb Pointer to the callback function
 *  @return Pointer to the callback function
 */
cci::shared_ptr<cci::callb_adapt>
    cci_param_user_data_type::register_callback(const cci::callback_type type,
                                                cci::shared_ptr<cci::callb_adapt> callb) {
  std::cout << "Function cci_param_user_data_type::register_callback Called " << std::endl;
  /* Complex Later */
  return callb;  // dummy return value for now
}

/**
 *  @fn     void cci_param_user_data_type::unregister_all_callbacks(void* observer)
 *  @brief  Function to unregister all callbacks with a given observer
 *  @param  observer  The observer to unregister the callbacks for
 *  @return void
 */
void cci_param_user_data_type::unregister_all_callbacks(void* observer) {
  std::cout << "Function cci_param_user_data_type::unregister_all_callbacks Called " << std::endl;

  /* Complex Later */
}

/**
 *  @fn     bool cci_param_user_data_type::unregister_callback(cci::shared_ptr<cci::callb_adpt> callb)
 *  @brief  Function to unregister a particular callback function
 *  @param  callb The function to be unregistered
 *  @return True or false depending on whether it was successfully unregistered
 */
bool cci_param_user_data_type::unregister_callback(cci::shared_ptr<cci::callb_adapt> callb) {
  std::cout << "Function cci_param_user_data_type::unregister_callback Called " << std::endl;

  /* Complex Later */
  return true;  // dummy return value for now
}

/**
 *  @fn     bool cci_param_user_data_type::unregister_callback(cci::callb_adapt* callb)
 *  @brief  Function to unregister a particular callback function
 *  @param  callb The function to be unregistered
 *  @return A true or false depending on whether the function was unregistered or not
 */
bool cci_param_user_data_type::unregister_callback(cci::callb_adapt* callb) {
  std::cout << "Function cci_param_user_data_type::unregister_callback Called " << std::endl;

  /* Complex Later */
  return true;  // dummy return value for now
}

/**
 *  @fn     bool cci_param_user_data_type::has_callback()
 *  @brief  Function to determine if the parameter has callbacks registered or not
 *  @return True or false depending on whether callbacks are registered
 */
bool cci_param_user_data_type::has_callbacks() {
  std::cout << "Function cci_param_user_data_type::has_callbacks Called " << std::endl;
  return callback_flag;
}

/**
 *  @fn     bool cci_param_user_data_type::lock(void* pwd = NULL)
 *  @brief  Lock the paramter
 *  @param  pwd The password to unlock the paramter
 *  @return True or false on whether the lock was successful
 */
bool cci_param_user_data_type::lock(void* pwd) {
  std::cout << "Function cci_param_user_data_type::lock Called " << std::endl;
  /* Check if already */
  l_password = pwd;
  lock_flag = true;
  return lock_flag;
}

/**
 *  @fn     bool cci_param_user_data_type::unlock(void* pwd = NULL)
 *  @brief  Function to unlock the parameter
 *  @param  pwd The password to unlock the parameter
 *  @return True or false depending on whether the parameter was unlocked
 */
bool cci_param_user_data_type::unlock(void* pwd) {
  std::cout << "Function cci_param_user_data_type::unlock Called " << std::endl;

  if (pwd == l_password) {
    lock_flag = false;
    return true;
  }
  return false;
}

/**
 *  @fn     bool cci_param_user_data_type::is_locked()
 *  @brief  Function to determine if the parameter is locked or not
 *  @return True or false depending on whether or not the parameter is locked
 */
bool cci_param_user_data_type::is_locked() const {
  std::cout << "Function cci_param_user_data_type::is_locked Called " << std::endl;
  return lock_flag;
}

bool cci_param_user_data_type::equals(const cci_param_impl_if& rhs) const
{
	const cci_param_user_data_type * other = dynamic_cast<const cci_param_user_data_type*>(&rhs);
	if (other)
	{
		return other->equals(*this);
	}
	return false;
}

void cci_param_user_data_type::init()
{
	cout
		<< "\n\t[PARAM IMPL] : 'init_cci_param' : For user_data_type With cci::mutable_param"
		<< endl;
}

void cci_param_user_data_type::destroy()
{
	cout
		<< "\n\t[PARAM IMPL] : 'destroy_cci_param' : For user_data_type With cci::mutable_param"
		<< endl;
	delete this;
}

/// Creating CCI-parmeter with some default value
namespace cci {

/**
 *  @fn     template<>
 *            cci_param_impl_if*
 *              create_cci_param<user_data_type, mutable_param>(
 *                cci_param<user_data_type, mutable_param> *owner_par,
 *                const std::string &nam,
 *                const user_data_type & val,
 *                bool is_top_level_name,
 *                cci_broker_if* broker_accessor)
 *  @brief  Function to create a cci parameter.
 *  @param  owner_par The mutable owner parameter
 *  @param  nam Name for the parameter
 *  @param  val The value to assign to the parameter
 *  @param  is_top_level_name Whether the name is the top level or not
 *  @param  broker_accessor A pointer to the broker for the parameter.
 *  @return A cci parameter implementation
 */
template<>
cci_param_impl_if
    *create_cci_param<user_data_type, mutable_param>(
        cci_param<user_data_type, mutable_param> *owner_par,
        const std::string &nam,
        const user_data_type & val,
        bool is_top_level_name,
        cci_broker_if* broker_accessor) {
  std::cout
      << "\n\t[PARAM_IMPL] : Creating CCI_PARAM: For user_data_type with cci::mutable_param"
      << std::endl;

  std::cout << "\t[PARAM_IMPL] : Called with Default Value as reference object"
            << val << std::endl;

  cci_param_user_data_type *param_impl = new cci_param_user_data_type(nam, val);

  return param_impl;
}

/**
 *  @fn     template<>
 *            cci_param_impl_if*
 *              create_cci_param<user_data_type, mutable_param>(
 *                cci_param<user_data_type, mutable_param> *owner_par,
 *                const std::string &nam,
 *                const char* pval,
 *                bool is_top_level_name,
 *                cci_broker_if* broker_accessor)
 *  @brief  Function to create a cci parameter using a char* initial value.
 *  @param  owner_par The mutable owner parameter
 *  @param  nam Name for the parameter
 *  @param  pval Pointer to the value to assign to the parameter.
 *  @param  is_top_level_name Whether the name is the top level or not
 *  @param  broker_accessor A pointer to the broker for the parameter.
 *  @return A cci parameter implementation
 */
template<>
cci_param_impl_if
    *create_cci_param<user_data_type, mutable_param>(
        cci_param<user_data_type, mutable_param> *owner_par,
        const std::string &nam,
        const cci_value& pval,
        const bool is_top_level_name,
        cci_broker_if* broker_accessor) {
  std::cout
      << "\n\t[PARAM IMPL] : Creating CCI_PARAM : For user_data_type With cci::mutable_param "
      << std::endl;

  // Get Value from pval string
  cci_param_user_data_type *param_impl = new cci_param_user_data_type(nam,
                                                                      user_data_type(0, 0, 0));

  return param_impl;
}

}  // End of CCI namespace
