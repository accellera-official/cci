/*****************************************************************************
  Copyright 2006-2015 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2006-2014 OFFIS Institute for Information Technology
  All rights reserved.

  Copyright 2006-2015 Intel Corporation
  All rights reserved.

  Copyright 2016 Ericsson AB
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

#include "cci_param_untyped.h"
#include "cci_callbacks.h"
#include "cci_core/systemc.h"

CCI_OPEN_NAMESPACE_

	cci_param_untyped::cci_param_untyped(bool is_top_level_name,
								   cci::cci_broker_if* broker_handle,
								   const std::string& desc,
								   const cci_originator& originator)
	: m_gs_param_base(NULL), m_is_default_value(0), m_is_invalid_value(!0), // TODO
	  m_status_guard(*this), m_description(desc), m_init_called(false),
	  m_broker_handle(broker_handle),
	  m_latest_write_access_originator_cp("NONE"),
	  m_latest_write_access_originator_valid(false),
	  m_originator(originator)
	{
	}

	cci_param_untyped::~cci_param_untyped()
	{
		m_broker_handle->remove_param(this);
		assert(m_init_called && "If this happens, the construction did not call the base param init function!");
		for (unsigned int i = 0; i < fw_vec.size(); ++i) {
			fw_vec[i]->adapt->caller_param = NULL;
		}
		sc_core::sc_unregister_hierarchical_name(get_name().c_str());
		for (std::vector<cci_callback_handle>::reverse_iterator ii = m_callback_handles.rbegin();
			 ii != m_callback_handles.rend(); ++ii)
		{
			ii->unregister();
		}
	}

	void cci_param_untyped::set_description(const std::string& desc)
	{
		m_description = desc;
	}

	std::string cci_param_untyped::get_description() const
	{
		return m_description;
	}

	void cci_param_untyped::add_metadata(const std::string &name,
										 const cci_value &value,
										 const std::string &desc)
	{
		metadata.push_entry(name, cci_value_list().push_back(value)
				.push_back(desc));
	}

	cci_value_map cci_param_untyped::get_metadata() const
	{
		return metadata;
	}

	bool cci_param_untyped::is_default_value()
	{
		return m_is_default_value;
	}

	bool cci_param_untyped::is_initial_value() const
	{
		assert(m_gs_param_base != NULL && "This must been set immediately after construction!");
		return m_gs_param_base->is_initial_value();
	}

	const cci_originator* cci_param_untyped::get_latest_write_originator() const
	{
		if (!m_latest_write_access_originator_valid) {
			return NULL;
		}
		return &m_latest_write_access_originator_cp;
	}

	void cci_param_untyped::update_latest_write_originator(const cci_originator& originator) const {
		m_latest_write_access_originator_valid = true;
		m_latest_write_access_originator_cp = originator;
	}

	cci_callback_handle cci_param_untyped::validate_write(cci_callback<bool(const cci_value&, const cci_value&, const cci_originator&)> callback)
	{
		// TODO
	}

	cci_callback_handle cci_param_untyped::validate_write(cci_callback<bool(const void*, const void*, const cci_originator&)> callback)
	{
		// TODO
	}

	cci_callback_handle cci_param_untyped::on_write(cci_callback<void(const cci_value&, const cci_value&, const cci_originator&)> callback)
	{
		// TODO
	}

	cci_callback_handle cci_param_untyped::on_write(cci_callback<void(const void*, const void*, const cci_originator&)> callback)
	{
		// TODO
	}

	cci_callback_handle cci_param_untyped::validate_read(cci_callback<bool(const cci_originator&)> callback)
	{
		// TODO
	}

	bool cci_param_untyped::lock(void* pwd)
	{
		assert(m_gs_param_base != NULL && "This must been set immediately after construction!");
		return m_gs_param_base->lock(pwd);
	}

	bool cci_param_untyped::unlock(void* pwd)
	{
		assert(m_gs_param_base != NULL && "This must been set immediately after construction!");
		return m_gs_param_base->unlock(pwd);
	}

	bool cci_param_untyped::is_locked() const
	{
		assert(m_gs_param_base != NULL && "This must been set immediately after construction!");
		return m_gs_param_base->locked();
	}

	const std::string& cci_param_untyped::get_name() const
	{
		assert(m_gs_param_base != NULL && "This must been set immediately after construction!");
		return m_gs_param_base->getName();
	}

	cci_originator cci_param_untyped::get_originator() const
	{
		return m_originator;
	}

	void cci_param_untyped::init()
	{
		assert(m_init_called == false && "init() function called more than once!");
		m_init_called = true;
		m_param_untyped_handle = this->create_param_handle(cci_originator());
		/*m_post_write_callback = register_callback(cci::post_write,
												  &m_status_guard,
												  bind(&status_guard::call,
													   &m_status_guard, _1,
													   _2),
												  *m_param_untyped_handle); // internal callback for status variables */
	}

CCI_CLOSE_NAMESPACE_

