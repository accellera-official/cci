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
	{
	}

	cci_base_param::cci_base_param(cci_param_impl_if & impl, const cci_originator & originator) 
	: m_originator(originator), m_impl(impl), m_owns_impl(true)
	{
	}

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
		m_impl.request_exclusive();
		m_impl.json_deserialize(json_string);
		m_impl.release_exclusive();
	}

	std::string cci_base_param::json_serialize() const
	{
		m_impl.request_exclusive();
		std::string json = m_impl.json_serialize();
		m_impl.release_exclusive();
		return json;
	}


	void cci_base_param::set_value(const cci_value& val)
	{
		m_impl.request_exclusive();
		m_impl.set_value(val);
		m_impl.release_exclusive();
	}

	cci_value cci_base_param::get_value() const
	{
		m_impl.request_exclusive();
		cci_value value = m_impl.get_value();
		m_impl.release_exclusive();
		return value;
	}

	void cci_base_param::set_documentation(const std::string& doc)
	{
		m_impl.request_exclusive();
		m_impl.set_documentation(doc);
		m_impl.release_exclusive();
	}

	std::string cci_base_param::get_documentation() const
	{
		m_impl.request_exclusive();
		std::string documentation = m_impl.get_documentation();
		m_impl.release_exclusive();
		return documentation;
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
		m_impl.request_exclusive();
		shared_ptr<callb_adapt> callback = m_impl.register_callback(type, observer, function);
		m_impl.release_exclusive();
		return callback;
	}

	shared_ptr<callb_adapt> cci_base_param::register_callback(const callback_type type, shared_ptr<callb_adapt> callb)
	{
		m_impl.request_exclusive();
		shared_ptr<callb_adapt> callback = m_impl.register_callback(type, callb);
		m_impl.release_exclusive();
		return callback;
	}

	void cci_base_param::unregister_all_callbacks(void* observer)
	{
		m_impl.request_exclusive();
		m_impl.unregister_all_callbacks(observer);
		m_impl.release_exclusive();
	}

	bool cci_base_param::unregister_callback(cci::shared_ptr<callb_adapt> callb)
	{
		m_impl.request_exclusive();
		bool callback = m_impl.unregister_callback(callb);
		m_impl.release_exclusive();
		return callback;
	}

	bool cci_base_param::unregister_callback(callb_adapt* callb)
	{
		m_impl.request_exclusive();
		bool callback = m_impl.unregister_callback(callb);
		m_impl.release_exclusive();
		return callback;
	}

	bool cci_base_param::has_callbacks()
	{
		m_impl.request_exclusive();
		bool callback =  m_impl.has_callbacks();
		m_impl.release_exclusive();
		return callback;
	}

	bool cci_base_param::lock(void* pwd)
	{
		m_impl.request_exclusive();
		bool lock = m_impl.lock(pwd);
		m_impl.release_exclusive();
		return lock;
	}

	bool cci_base_param::unlock(void* pwd)
	{
		m_impl.request_exclusive();
		bool unlock = m_impl.unlock(pwd);
		m_impl.release_exclusive();
		return unlock;
	}

	bool cci_base_param::is_locked() const
	{
		m_impl.request_exclusive();
		bool lock = m_impl.is_locked();
		m_impl.release_exclusive();
		return lock;
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
		m_impl.request_exclusive();
		void* value = (void*)m_impl.get();
		m_impl.release_exclusive();
		return value;
	}

	const void* cci_base_param::get_default_value() const
	{
		return m_impl.get_default_value();
	}

	void cci_base_param::set(const void* vp)
	{
		// Make originator available to the implementation.
		cci_originator::set_global_originator(&m_originator);
		m_impl.request_exclusive();
		m_impl.set(vp);
		m_impl.release_exclusive();
	}

	void cci_base_param::set(const void* vp, const void* pwd)
	{
		// Make originator available to the implementation.
		cci_originator::set_global_originator(&m_originator);
		m_impl.request_exclusive();
		m_impl.set(vp, pwd);
		m_impl.release_exclusive();
	}

	bool cci_base_param::equals(const cci_param_impl_if& rhs) const
	{
		rhs.request_exclusive();
		m_impl.request_exclusive();
		bool equals = m_impl.equals(rhs);
		m_impl.release_exclusive();
		rhs.release_exclusive();
		return equals;
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

