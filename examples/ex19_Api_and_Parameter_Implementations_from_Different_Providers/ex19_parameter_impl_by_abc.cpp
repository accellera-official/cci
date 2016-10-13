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
 *  @file      parameter_impl_by_abc.cpp
 *  @brief     This class demonstrates the parameter implementation by a
 *             different provider
 *  @date      14th December, 2011
 *  @author    Girish Verma, CircuitSutra Technologies     <girish@circuitsutra.com>
 *             P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 */

#include "ex19_parameter_impl_by_abc.h"
using cci::cci_report_handler;

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
      description("")
{
  nam = _name;
  /*Register created parameter into global broker*/
  m_broker_handle = &cci::cci_broker_manager::get_current_broker(cci::cci_originator());
  m_broker_handle->add_param(this);

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
 *  @fn     user_data_type& get_value()
 *  @brief  Function to get the value of the parameter
 *  @return user's parameter value
 */
const user_data_type& cci_param_user_data_type::get_value() const
{
  return value;
}

/**
 *  @fn     const void* get_raw_value() const
 *  @brief  Function to get the user defined type for the parameter
 *  @return The pointer to user_data_type of the parameter.
 */
const void* cci_param_user_data_type::get_raw_value() const
{
  return NULL;
}

/**
*  @fn     void set_raw_value(const void *vp, const cci::cci_originator &originator)
*  @brief  Function to set the value of the parameter along with the lock password.
*  @param  val The value to assign
*  @param  originator reference to the originator
*  @return void
*/
void cci_param_user_data_type::set_raw_value(const void *vp, const cci::cci_originator &originator) {
	std::cout << "Function cci_param_user_data_type::set_raw_value Called " << std::endl;
	if (l_password == NULL) {
		value = *static_cast<const user_data_type*>(vp);
	}
}

/**
 *  @fn     set_raw_value(const void *vp, const void *pwd,const cci::cci_originator &originator)
 *  @brief  Function to set the value of the parameter along with the lock password.
 *  @param  val The value to assign
 *  @param  pwd  The password for the parameter
 *  @param  originator reference to the originator
 *  @return void
 */
void cci_param_user_data_type::set_raw_value(const void *vp, const void *pwd,
    const cci::cci_originator &originator) {
  std::cout << "Function cci_param_user_data_type::set_raw_value Called " << std::endl;
  if (l_password == pwd) {
    value = *static_cast<const user_data_type*>(vp);
  }
}

/**
 *  @fn     const user_data_type& cci_param_user_data_type::get_default_value()
 *  @brief  Retrieve the default value of the parameter
 *  @return The user data type for the value of the parameter
 */
const void* cci_param_user_data_type::get_default_value_raw() const {
  std::cout << "Function cci_param_user_data_type::get_default_value Called " << std::endl;

  return &default_value;
}

// Virtual function in cci_base_param_impl_if

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
void cci_param_user_data_type::set_cci_value(const cci::cci_value& val) {
  std::cout << "Function cci_param_user_data_type::set_cci_value Called " << std::endl;
  c_value = val;
}

void cci_param_user_data_type::set_cci_value(const cci::cci_value& val, const cci::cci_originator& originator) {
  std::cout << "Function cci_param_user_data_type::set_cci_value Called " << std::endl;
  c_value = val;
}

/**
 *  @fn     cci::cci_value cci_param_user_data_type::get_cci_value() const
 *  @brief  Function to retrieve the value of the parameter
 *  @return The cci_value of the parameter
 */
cci::cci_value cci_param_user_data_type::get_cci_value() const {
  std::cout << "Function cci_param_user_data_type::get_cci_value Called " << std::endl;
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

void cci_param_user_data_type::add_metadata(const std::string &, const cci::cci_value &,
	const std::string &) {
	// Not required for the purposes of this example.
	CCI_REPORT_ERROR("cci_param_user_data_type/add_metadata", "NOT IMPLEMENTED.");
}

cci::cci_value_map cci_param_user_data_type::get_metadata() const {
	// Not required for the purposes of this example.
	CCI_REPORT_ERROR("cci_param_user_data_type/add_metadata", "NOT IMPLEMENTED.");
	return cci::cci_value_map();
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

const std::type_info& cci_param_user_data_type::get_type_info() const {
  std::cout << "Function cci_param_user_data_type::get_type_info Called " << std::endl;
  return typeid(user_data_type);
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
 *  @fn     bool is_handle()
 *  @brief  check if this is an handle
 *  @return True or false on whether it is an handle
 */
bool cci_param_user_data_type::is_handle() const {
  return false;
}

/**
 *  @fn     bool cci_param_user_data_type::has_callback()
 *  @brief  Function to determine if the parameter has callbacks registered or not
 *  @return True or false depending on whether callbacks are registered
 */
bool cci_param_user_data_type::has_callbacks() const {
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

bool cci_param_user_data_type::equals(const cci::cci_param_if& rhs) const
{
  return rhs.equals(*this);
}

bool cci_param_user_data_type::equals(const cci::cci_param_untyped_handle &rhs) const
{
  const cci_param_user_data_type * other = dynamic_cast<const cci_param_user_data_type*>(&rhs);
  if (other)
  {
    return other->get_value()==this->get_value();
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

void cci_param_user_data_type::add_param_handle(
        cci::cci_param_untyped_handle* param_handle)
{
  // Not implemented
}

void cci_param_user_data_type::remove_param_handle(
        cci::cci_param_untyped_handle* param_handle)
{
  // Not implemented
}

cci::cci_callback_untyped_handle
cci_param_user_data_type::register_pre_write_callback
(const cci::cci_callback_untyped_handle &cb,const cci::cci_originator &orig)
{
  return cci::cci_callback_untyped_handle();
}

bool cci_param_user_data_type::unregister_pre_write_callback
(const cci::cci_callback_untyped_handle &cb,const cci::cci_originator &orig)
{
  return false;
}

cci::cci_callback_untyped_handle
cci_param_user_data_type::register_post_write_callback
(const cci::cci_callback_untyped_handle &cb,const cci::cci_originator &orig)
{
  return cci::cci_callback_untyped_handle();
}

bool cci_param_user_data_type::unregister_post_write_callback
(const cci::cci_callback_untyped_handle &cb,const cci::cci_originator &orig)
{
  return false;
}

cci::cci_callback_untyped_handle
cci_param_user_data_type::register_pre_read_callback
(const cci::cci_callback_untyped_handle &cb,const cci::cci_originator &orig)
{
  return cci::cci_callback_untyped_handle();
}

bool cci_param_user_data_type::unregister_pre_read_callback
(const cci::cci_callback_untyped_handle &cb,const cci::cci_originator &orig)
{
  return false;
}

cci::cci_callback_untyped_handle
cci_param_user_data_type::register_post_read_callback
(const cci::cci_callback_untyped_handle &cb,const cci::cci_originator &orig)
{
  return cci::cci_callback_untyped_handle();
}

bool cci_param_user_data_type::unregister_post_read_callback
(const cci::cci_callback_untyped_handle &cb,const cci::cci_originator &orig)
{
  return false;
}

bool cci_param_user_data_type::unregister_all_callbacks
(const cci::cci_originator &orig)
{
 return false;
}
