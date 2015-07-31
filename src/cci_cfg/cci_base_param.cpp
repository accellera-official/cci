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

/**
 * @author Enrico Galli, Intel
 */

#include "cci_base_param.h"
#include "cci_param_impl_if.h"


CCI_OPEN_NAMESPACE_

	cci_base_param::cci_base_param(cci_base_param & copy, const cci_originator & originator) 
	: m_originator(originator), m_impl(copy.m_impl), m_owns_impl(false)
	{}

	cci_base_param::cci_base_param(cci_param_impl_if & impl, const cci_originator & originator) 
	: m_originator(originator), m_impl(impl), m_owns_impl(true)
	{}

	cci_base_param::~cci_base_param()
	{
		if (m_owns_impl)
		{
			m_impl.destroy();
		}
	}

	void cci_base_param::json_deserialize(const std::string& json_string)
	{
		// Make originator available to the implementation.
		cci_originator::set_global_originator(&m_originator);
		m_impl.json_deserialize(json_string);
	}

	std::string cci_base_param::json_serialize() const
	{
		return m_impl.json_serialize();
	}


	void cci_base_param::set_value(const cci_value& val)
	{
		m_impl.set_value(val);
	}

	cci_value cci_base_param::get_value() const
	{
		return m_impl.get_value();
	}

	void cci_base_param::set_documentation(const std::string& doc)
	{
		m_impl.set_documentation(doc);
	}

	std::string cci_base_param::get_documentation() const
	{
		return m_impl.get_documentation();
	}

	bool cci_base_param::is_default_value()
	{
		return m_impl.is_default_value();
	}

	bool cci_base_param::is_initial_value() const
	{
		return m_impl.is_initial_value();
	}

	const cci_originator* cci_base_param::get_latest_write_originator() const
	{
		return m_impl.get_latest_write_originator();
	}


	shared_ptr<callb_adapt> cci_base_param::register_callback(const callback_type type, void* observer, param_callb_func_ptr function)
	{
		return m_impl.register_callback(type, observer, function);
	}

	shared_ptr<callb_adapt> cci_base_param::register_callback(const callback_type type, shared_ptr<callb_adapt> callb)
	{
		return m_impl.register_callback(type, callb);
	}

	void cci_base_param::unregister_all_callbacks(void* observer)
	{
		m_impl.unregister_all_callbacks(observer);
	}

	bool cci_base_param::unregister_callback(cci::shared_ptr<callb_adapt> callb)
	{
		return m_impl.unregister_callback(callb);
	}

	bool cci_base_param::unregister_callback(callb_adapt* callb)
	{
		return m_impl.unregister_callback(callb);
	}

	bool cci_base_param::has_callbacks()
	{
		return m_impl.has_callbacks();
	}

	bool cci_base_param::lock(void* pwd)
	{
		return m_impl.lock(pwd);
	}

	bool cci_base_param::unlock(void* pwd)
	{
		return m_impl.unlock(pwd);
	}

	bool cci_base_param::is_locked() const
	{
		return m_impl.is_locked();
	}

	basic_param_type cci_base_param::get_basic_type() const
	{
		return m_impl.get_basic_type();
	}

	const std::string& cci_base_param::get_name() const
	{
		return m_impl.get_name();
	}

	bool cci_base_param::is_accessor() const
	{
		return !m_owns_impl;
	}

	cci_originator cci_base_param::get_originator() const
	{
		return m_originator;
	}

	const void* cci_base_param::get() const
	{
		return m_impl.get();
	}

	const void* cci_base_param::get_default_value() const
	{
		return m_impl.get_default_value();
	}

	void cci_base_param::set(const void* vp)
	{
		// Make originator available to the implementation.
		cci_originator::set_global_originator(&m_originator);
		m_impl.set(vp);
	}

	void cci_base_param::set(const void* vp, const void* pwd)
	{
		// Make originator available to the implementation.
		cci_originator::set_global_originator(&m_originator);
		m_impl.set(vp, pwd);
	}

	bool cci_base_param::equals(const cci_param_impl_if& rhs) const
	{
		return m_impl.equals(rhs);
	}

	void cci_base_param::init()
	{
		m_impl.init();
	}

	void cci_base_param::destroy()
	{
		m_impl.destroy();
	}

CCI_CLOSE_NAMESPACE_

