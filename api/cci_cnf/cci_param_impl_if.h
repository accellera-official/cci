/*****************************************************************************
  Copyright 2006-2015 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2006-2014 OFFIS Institute for Information Technology
  All rights reserved.

  Copyright 2006-2015 Intel Corporation
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


#ifndef CCI_CNF_CCI_PARAM_IMPL_IF_H_INCLUDED_
#define CCI_CNF_CCI_PARAM_IMPL_IF_H_INCLUDED_

#include "cci_cnf/cci_shared_ptr.h"
#include "cci_cnf/cci_callbacks.h"
#include "cci_cnf/cci_value.h"

/**
 * @author Enrico Galli, Intel
 */

CCI_OPEN_CONFIG_NAMESPACE_


class cci_base_param;
class cci_originator;

/// Interfaces for cci_param implementation
/**
* Features:
* - Name of the parameter,
* - stores value untyped as string representation
* - allows generic typed and list access
* - JSON (de)serialize functions
* - Callback handling
*/
class cci_param_impl_if
{
public:

	/// Destructor.
	/**
	*
	*/
	virtual ~cci_param_impl_if() { }


	///@name Set and Get with JSON String Representation
	///@{

	/// @copydoc cci_base_param::json_deserialize
	virtual void json_deserialize(const std::string& json_string) = 0;

	/// @copydoc cci_base_param::json_serialize
	virtual std::string json_serialize() const = 0;

	///@}

	///@name JSON Data Type and access
	///@{

	/// @copydoc cci_base_param::set_value
	virtual void set_value(const cci_value& val) = 0;

	/// @copydoc cci_base_param::get_value
	virtual cci_value get_value() const = 0;

	///@}

	///@name Documentation
	///@{

	/// @copydoc cci_base_param::set_documentation
	virtual void set_documentation(const std::string& doc) = 0;

	/// @copydoc cci_base_param::get_documentation
	virtual std::string get_documentation() const = 0;


	///@}

	///@name Parameter Value Status
	///@{


	/// @copydoc cci_base_param::is_default_value
	virtual bool is_default_value() = 0;

	/// @copydoc cci_base_param::is_invalid_value
	virtual bool is_invalid_value() = 0;

	/// @copydoc cci_base_param::set_invalid_value
	virtual void set_invalid_value() = 0;

	/// @copydoc cci_base_param::is_initial_value
	virtual bool is_initial_value() const = 0;


	///@}

	///@name Miscellaneous
	///@{

	/// @copydoc cci_base_param::get_latest_write_originator
	virtual const cci_originator* get_latest_write_originator() const = 0;

	///@}

	///@name Callback Handling
	///@{

	/// @copydoc cci_base_param::register_callback(const callback_type, void*, param_callb_func_ptr)
	virtual shared_ptr<callb_adapt> register_callback(const callback_type type, void* observer, param_callb_func_ptr function) = 0;

	/// @copydoc cci_base_param::register_callback(const callback_type, cci::shared_ptr<callb_adapt>)
	virtual shared_ptr<callb_adapt> register_callback(const callback_type type, shared_ptr<callb_adapt> callb) = 0;

	/// @copydoc cci_base_param::unregister_all_callbacks
	virtual void unregister_all_callbacks(void* observer) = 0;

	/// @copydoc cci_base_param::unregister_callback(cci::shared_ptr<callb_adapt>)
	virtual bool unregister_callback(shared_ptr<callb_adapt> callb) = 0;
	
	/// @copydoc cci_base_param::unregister_callback(callb_adapt*)
	virtual bool unregister_callback(callb_adapt* callb) = 0;

	/// @copydoc cci_base_param::json_deserialize
	virtual bool has_callbacks() = 0;

	///@}

	///@name Write-access control
	///@{

	/// @copydoc cci_base_param::lock
	virtual bool lock(void* pwd = NULL) = 0;

	/// @copydoc cci_base_param::unlock
	virtual bool unlock(void* pwd = NULL) = 0;

	/// @copydoc cci_base_param::is_locked
	virtual bool is_locked() const = 0;

	///@}

	///@name Query parameter type and name
	///@{

	/// @copydoc cci_base_param::get_basic_type
	virtual basic_param_type get_basic_type() const = 0;

	/// @copydoc cci_base_param::get_name
	virtual const std::string& get_name() const = 0;

	///@}

	///@name Type-punned value operations
	///@{

	/// @copydoc cci_base_param::get
	virtual const void* get() const = 0;

	/// @copydoc cci_base_param::get_default_value
	virtual const void* get_default_value() const = 0;

	/// @copydoc cci_base_param::set(const void*)
	virtual void set(const void* vp) = 0;

	/// @copydoc cci_base_param::set(const void*, const void*)
	virtual void set(const void* vp, const void* pwd) = 0;

	/// Computer if the stored values are equal
	/**
	* @param rhs reference to another cci_param implementation
	* @return True if both values are equal
	*/
	virtual bool equals(const cci_param_impl_if& rhs) const = 0;

	///@}


	///@name Initialization and Destructions methods
	///@{

	/// @copydoc cci_base_param::init
	virtual void init() = 0;

	/// @copydoc cci_base_param::destroy
	virtual void destroy() = 0;

	///@}

};

CCI_CLOSE_CONFIG_NAMESPACE_

#endif
