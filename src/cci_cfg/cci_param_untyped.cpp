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
        for (std::vector<cci_param_untyped_handle*>::iterator it = m_param_handles.begin();
             it != m_param_handles.end(); it++) {
            (*it)->invalidate();
        }
        delete m_gs_param_base;
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

	shared_ptr<callb_adapt> cci_param_untyped::register_callback(const callback_type type,
															  	 void* observer,
															  	 param_callb_func_ptr function,
																 cci_param_untyped_handle& param)
	{
		return register_callback(type,
		    cci::make_shared<cci::callb_adapt>(observer,
											   function,
											   &param),
								 param);
	}

	shared_ptr<callb_adapt> cci_param_untyped::register_callback(const callback_type type,
															  	 shared_ptr<callb_adapt> callb,
																 cci_param_untyped_handle& param)
	{
		gs::cnf::callback_type cb = gs::cnf::no_callback;
		switch(type) {
			case cci::pre_read:
				cb = gs::cnf::pre_read;
				break;
			case cci::reject_write:
				cb = gs::cnf::reject_write;
				break;
			case cci::pre_write:
				cb = gs::cnf::pre_write;
				break;
			case cci::create_param:
				cb = gs::cnf::create_param;
				break;
			case cci::post_write:
				cb = gs::cnf::post_write;
				break;
			case cci::destroy_param:
				cb = gs::cnf::destroy_param;
				break;
		}
		if (cb != gs::cnf::no_callback) {
			internal_callback_forwarder *fw =
					new internal_callback_forwarder(callb, cb, param);
			fw_vec.push_back(fw);
			assert(m_gs_param_base != NULL && "This must been set immediately after construction!");
			fw->calling_gs_adapter =
					m_gs_param_base->registerParamCallback(
							cci::shared_ptr< ::gs::cnf::ParamCallbAdapt_b>(
									new ::gs::cnf::ParamTypedCallbAdapt<internal_callback_forwarder>
											(fw,
											 &internal_callback_forwarder::call,
											 callb->get_observer(),
											 const_cast<gs::gs_param_base*>(static_cast<gs::gs_param_base*>(m_gs_param_base)))
							), fw->get_type() );
		}
		return callb;
	}

	shared_ptr<callb_adapt> cci_param_untyped::register_callback(const callback_type type,
																 void* observer,
																 param_callb_func_ptr function)
	{
		return register_callback(type,
								 cci::make_shared<cci::callb_adapt>(observer,
																	function,
																	m_param_untyped_handle),
								 *m_param_untyped_handle);
	}

	shared_ptr<callb_adapt> cci_param_untyped::register_callback(const callback_type type,
																 shared_ptr<callb_adapt> callb)
	{
		return register_callback(type, callb, *m_param_untyped_handle);
	}

	void cci_param_untyped::unregister_all_callbacks(void* observer)
	{
		bool succ = true;
		internal_callback_forwarder* fw;
		while (succ) {
			succ = false;
			for (unsigned int i = 0; i < fw_vec.size(); ++i) {
				if (fw_vec[i]->adapt->get_observer() == observer) {
					fw = fw_vec[i];
					fw_vec.erase(fw_vec.begin()+i);
					delete fw;
					succ = true;
					break;
				}
			}
		}
	}

	bool cci_param_untyped::unregister_callback(cci::shared_ptr<callb_adapt> callb)
	{
		return unregister_callback(callb.get());
	}

	bool cci_param_untyped::unregister_callback(callb_adapt* callb)
	{
		internal_callback_forwarder* fw;
		for (unsigned int i = 0; i < fw_vec.size(); ++i) {
			if (fw_vec[i]->adapt == callb) {
				fw = fw_vec[i];
				fw_vec.erase(fw_vec.begin()+i);
				delete fw;
				return true;
			}
		}
		return false;
	}

	bool cci_param_untyped::has_callbacks()
	{
		return (fw_vec.size() > 0);
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
		m_post_write_callback = register_callback(cci::post_write,
												  &m_status_guard,
												  bind(&status_guard::call,
													   &m_status_guard, _1,
													   _2),
												  *m_param_untyped_handle); // internal callback for status variables
	}

    void cci_param_untyped::add_param_handle(cci_param_untyped_handle* param_handle)
    {
        m_param_handles.push_back(param_handle);
    }

    void cci_param_untyped::remove_param_handle(cci_param_untyped_handle* param_handle)
    {
        m_param_handles.erase(std::remove(m_param_handles.begin(), m_param_handles.end(), param_handle), m_param_handles.end());
    }

CCI_CLOSE_NAMESPACE_

