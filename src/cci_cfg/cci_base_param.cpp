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
	: m_originator(originator), m_impl(copy.m_impl), m_owns_impl(false), m_accessed_param(&copy)
	{}

	cci_base_param::cci_base_param(cci_param_impl_if & impl, const cci_originator & originator) 
	: m_originator(originator), m_impl(impl), m_owns_impl(true), m_accessed_param(nullptr)
	{}

	cci_base_param::~cci_base_param()
	{
		if (m_owns_impl)
		{
			for (param_accessor_vector::iterator it = m_param_accessors.begin();
				 it != m_param_accessors.end(); it++) {
					(*it)->set_accessed_param_destroyed();
			}
			m_impl.destroy();
		}
		else if(m_accessed_param)
		{
			m_accessed_param->remove_param_accessor(this);
		}
	}

	void cci_base_param::json_deserialize(const std::string& json_string)
	{
		check_accessed_param();
		// Make originator available to the implementation.
		cci_originator::set_global_originator(&m_originator);
		m_impl.json_deserialize(json_string);
	}

	std::string cci_base_param::json_serialize() const
	{
		check_accessed_param();
		return m_impl.json_serialize();
	}


	void cci_base_param::set_value(const cci_value& val)
	{
		check_accessed_param();
		m_impl.set_value(val);
	}

	cci_value cci_base_param::get_value() const
	{
		check_accessed_param();
		return m_impl.get_value();
	}

	void cci_base_param::set_documentation(const std::string& doc)
	{
		check_accessed_param();
		m_impl.set_documentation(doc);
	}

	std::string cci_base_param::get_documentation() const
	{
		check_accessed_param();
		return m_impl.get_documentation();
	}

	bool cci_base_param::is_default_value()
	{
		check_accessed_param();
		return m_impl.is_default_value();
	}

	bool cci_base_param::is_initial_value() const
	{
		check_accessed_param();
		return m_impl.is_initial_value();
	}

	const cci_originator* cci_base_param::get_latest_write_originator() const
	{
		check_accessed_param();
		return m_impl.get_latest_write_originator();
	}


	shared_ptr<callb_adapt> cci_base_param::register_callback(const callback_type type, void* observer, param_callb_func_ptr function)
	{
		check_accessed_param();
		return m_impl.register_callback(type, observer, function);
	}

	shared_ptr<callb_adapt> cci_base_param::register_callback(const callback_type type, shared_ptr<callb_adapt> callb)
	{
		check_accessed_param();
		return m_impl.register_callback(type, callb);
	}

	void cci_base_param::unregister_all_callbacks(void* observer)
	{
		check_accessed_param();
		m_impl.unregister_all_callbacks(observer);
	}

	bool cci_base_param::unregister_callback(cci::shared_ptr<callb_adapt> callb)
	{
		check_accessed_param();
		return m_impl.unregister_callback(callb);
	}

	bool cci_base_param::unregister_callback(callb_adapt* callb)
	{
		check_accessed_param();
		return m_impl.unregister_callback(callb);
	}

	bool cci_base_param::has_callbacks()
	{
		check_accessed_param();
		return m_impl.has_callbacks();
	}

	bool cci_base_param::lock(void* pwd)
	{
		check_accessed_param();
		return m_impl.lock(pwd);
	}

	bool cci_base_param::unlock(void* pwd)
	{
		check_accessed_param();
		return m_impl.unlock(pwd);
	}

	bool cci_base_param::is_locked() const
	{
		check_accessed_param();
		return m_impl.is_locked();
	}

	basic_param_type cci_base_param::get_basic_type() const
	{
		check_accessed_param();
		return m_impl.get_basic_type();
	}

	const std::string& cci_base_param::get_name() const
	{
		check_accessed_param();
		return m_impl.get_name();
	}

	bool cci_base_param::is_accessor() const
	{
		return !m_owns_impl;
	}

	bool cci_base_param::is_accessed_param_destroyed() const
	{
		return m_accessed_param ? false : true;
	}

	void cci_base_param::set_accessed_param_destroyed()
	{
		m_accessed_param = nullptr;
	}

	void cci_base_param::add_param_accessor(cci_base_param* accessor)
	{
		if (m_owns_impl) {
			m_param_accessors.push_back(accessor);
		}
	}

	void cci_base_param::remove_param_accessor(cci_base_param* accessor)
	{
		if (m_owns_impl) {
			m_param_accessors.erase(
				std::remove(m_param_accessors.begin(),
						m_param_accessors.end(), accessor),
				m_param_accessors.end());
		}
	}

	cci_originator cci_base_param::get_originator() const
	{
		return m_originator;
	}

	const void* cci_base_param::get() const
	{
		check_accessed_param();
		return m_impl.get();
	}

	const void* cci_base_param::get_default_value() const
	{
		check_accessed_param();
		return m_impl.get_default_value();
	}

	void cci_base_param::set(const void* vp)
	{
		check_accessed_param();
		// Make originator available to the implementation.
		cci_originator::set_global_originator(&m_originator);
		m_impl.set(vp);
	}

	void cci_base_param::set(const void* vp, const void* pwd)
	{
		check_accessed_param();
		// Make originator available to the implementation.
		cci_originator::set_global_originator(&m_originator);
		m_impl.set(vp, pwd);
	}

	bool cci_base_param::equals(const cci_param_impl_if& rhs) const
	{
		check_accessed_param();
		return m_impl.equals(rhs);
	}

	void cci_base_param::init()
	{
		check_accessed_param();
		m_impl.init();
	}

	void cci_base_param::destroy()
	{
		check_accessed_param();
		m_impl.destroy();
	}

	void cci_base_param::check_accessed_param() const
	{
		if(is_accessor() && is_accessed_param_destroyed()) {
			CCI_REPORT_ERROR("CCI_DESTROYED_PARAM",
							 "The accessed parameter has been destroyed.");
		}
	}

CCI_CLOSE_NAMESPACE_

