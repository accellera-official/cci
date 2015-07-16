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

	const std::string PARAM_ORIGINATOR = "owning_param";

	cci_base_param::cci_base_param(cci_base_param & copy, const cci_originator & originator) : m_originator(originator), m_impl(copy.m_impl)
	{}

	cci_base_param::cci_base_param(cci_param_impl_if & impl, const cci_originator & originator) : m_originator(originator), m_impl(impl)
	{}

	cci_base_param::~cci_base_param()
	{
		if (!is_accessor())
		{
			m_impl.destroy();
		}
	}

	void cci_base_param::json_deserialize(const std::string& json_string)
	{
		cci_originator_lock lock(m_originator);
		m_impl.json_deserialize(json_string);
	}

	std::string cci_base_param::json_serialize() const
	{
		cci_originator_lock lock(m_originator);
		return m_impl.json_serialize();
	}


	void cci_base_param::set_value(const cci_value& val)
	{
		cci_originator_lock lock(m_originator);
		m_impl.set_value(val);
	}

	cci_value cci_base_param::get_value() const
	{
		cci_originator_lock lock(m_originator);
		return m_impl.get_value();
	}

	void cci_base_param::set_documentation(const std::string& doc)
	{
		cci_originator_lock lock(m_originator);
		m_impl.set_documentation(doc);
	}

	std::string cci_base_param::get_documentation() const
	{
		cci_originator_lock lock(m_originator);
		return m_impl.get_documentation();
	}

	bool cci_base_param::is_default_value()
	{
		cci_originator_lock lock(m_originator);
		return m_impl.is_default_value();
	}

	bool cci_base_param::is_invalid_value()
	{
		cci_originator_lock lock(m_originator);
		return m_impl.is_invalid_value();
	}

	void cci_base_param::set_invalid_value()
	{
		cci_originator_lock lock(m_originator);
		m_impl.set_invalid_value();
	}

	bool cci_base_param::is_initial_value() const
	{
		cci_originator_lock lock(m_originator);
		return m_impl.is_initial_value();
	}

	const cci_originator* cci_base_param::get_latest_write_originator() const
	{
		cci_originator_lock lock(m_originator);
		return m_impl.get_latest_write_originator();
	}


	shared_ptr<callb_adapt> cci_base_param::register_callback(const callback_type type, void* observer, param_callb_func_ptr function)
	{
		cci_originator_lock lock(m_originator);
		return m_impl.register_callback(type, observer, function);
	}

	shared_ptr<callb_adapt> cci_base_param::register_callback(const callback_type type, shared_ptr<callb_adapt> callb)
	{
		cci_originator_lock lock(m_originator);
		return m_impl.register_callback(type, callb);
	}

	void cci_base_param::unregister_all_callbacks(void* observer)
	{
		cci_originator_lock lock(m_originator);
		m_impl.unregister_all_callbacks(observer);
	}

	bool cci_base_param::unregister_callback(cci::shared_ptr<callb_adapt> callb)
	{
		cci_originator_lock lock(m_originator);
		return m_impl.unregister_callback(callb);
	}

	bool cci_base_param::unregister_callback(callb_adapt* callb)
	{
		cci_originator_lock lock(m_originator);
		return m_impl.unregister_callback(callb);
	}

	bool cci_base_param::has_callbacks()
	{
		cci_originator_lock lock(m_originator);
		return m_impl.has_callbacks();
	}

	bool cci_base_param::lock(void* pwd)
	{
		cci_originator_lock lock(m_originator);
		return m_impl.lock(pwd);
	}

	bool cci_base_param::unlock(void* pwd)
	{
		cci_originator_lock lock(m_originator);
		return m_impl.unlock(pwd);
	}

	bool cci_base_param::is_locked() const
	{
		cci_originator_lock lock(m_originator);
		return m_impl.is_locked();
	}

	basic_param_type cci_base_param::get_basic_type() const
	{
		cci_originator_lock lock(m_originator);
		return m_impl.get_basic_type();
	}

	const std::string& cci_base_param::get_name() const
	{
		cci_originator_lock lock(m_originator);
		return m_impl.get_name();
	}

	bool cci_base_param::is_accessor() const
	{
		cci_originator_lock lock(m_originator);
		return std::string(m_originator.name()) != PARAM_ORIGINATOR;
	}

	cci_originator cci_base_param::get_originator() const
	{
		return m_originator;
	}

	const void* cci_base_param::get() const
	{
		cci_originator_lock lock(m_originator);
		return m_impl.get();
	}

	const void* cci_base_param::get_default_value() const
	{
		cci_originator_lock lock(m_originator);
		return m_impl.get_default_value();
	}

	void cci_base_param::set(const void* vp)
	{
		cci_originator_lock lock(m_originator);
		m_impl.set(vp);
	}

	void cci_base_param::set(const void* vp, const void* pwd)
	{
		cci_originator_lock lock(m_originator);
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

