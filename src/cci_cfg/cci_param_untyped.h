/*****************************************************************************
  Copyright 2006-2015 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2009-2011 GreenSocs Ltd
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

#ifndef CCI_CCI_PARAM_UNTYPED_H_INCLUDED_
#define CCI_CCI_PARAM_UNTYPED_H_INCLUDED_

#include <string>
#include <vector>

#include "cci_cfg/cci_config_macros.h"
#include "cci_cfg/cci_param_if.h"
#include "cci_cfg/cci_datatypes.h"
#include "cci_cfg/cci_value.h"
#include "cci_cfg/cci_originator.h"
#include "cci_cfg/cci_shared_ptr.h"
#include "cci_cfg/cci_callback.h"
#include <greencontrol/config.h>

/**
 * @author Enrico Galli, Intel
 */

CCI_OPEN_NAMESPACE_

// CCI Configuration parameter base class
/**
* Type independent base class all cci parameters derive from.
* This class gives some easier access to type independent functions.
*/
class cci_param_untyped : public cci_param_if
{

public:

	/// Destructor.
	virtual ~cci_param_untyped();

	///@name Description
	///@{

	/// Set parameter description.
	/**
	 * Set the parameter's description describing purpose and
	 * intended use, allowed value range etc. in a human readable way.
	 *
	 * @param desc Human readable description
	 */
    virtual void set_description(const std::string& desc);
	
	/// Get the parameter's description.
	/**
	 * return Description
	 */
	virtual std::string get_description() const;

	///@}

	///@name Metadata
	///@{

	/// Add metadata
	/**
	 * Add metadata to the parameter identified by its name, value
	 * and a human readable description.
	 *
	 * @param name Name of the metadata
	 * @param value Value of the metadata
	 * @param desc Human readable description
	 */
	void add_metadata(const std::string &name, const cci::cci_value &value,
					  const std::string &desc = "");

	/// Return the metadata value
	/**
	 * Return value of the metadata by its given name.
	 *
	 * @return name Name of the metadata
	 * @return Metadata value
	 */
	cci_value_map get_metadata() const;

	///@}

	///@name Parameter Value Status
	///@{

	/// Indicates whether the value provided at parameter construction persists.
	/**
	 * True if the value was supplied as a constructor argument and not
	 * subsequently changed.
	 *
	 * Note: false is returned even if the current value matches the constructor
	 * supplied default but has undergone intermediate changes.
	 *
	 * @return false if the parameter received an initial value or its value has
	 *         changed; otherwise, true
	 */
	virtual bool is_default_value();


	/// Indicates that the parameter received an initial value that has not since been modified.
	/**
	 * True if the value was supplied using the broker's
	 * set_initial_cci_value function and not subsequently changed.
	 *
	 * Note: false is returned even if the current value matches the initial
	 * value but has undergone intermediate changes.
	 *
	 * @return fase if no initial value was supplied or the parameter's value has
	 *         changed; otherwise, true
	 */
    virtual bool is_initial_value() const;

	///@}


	///@name Miscellaneous
	///@{

	/// Returns the originator of the parameter's current value. 
	/**
	 * This initially reflects originator of the parameter's starting value,
	 * e.g. the owning module or startup configuration file.  It is
	 * subsequently updated to reflect the originator of any value changes.
	 *
	 * The originator is updated on successful calls to the following functions:
	 * set_cci_value(), cci_param_typed::set(), cci_param_typed::operator=()
	 */
	const cci_originator* get_latest_write_originator() const;

	///@}

	
	///@name Callback Handling
	///@{

	cci_callback_handle validate_write(cci_callback<bool(const cci_value&, const cci_value&, const cci_originator&)> callback);

	cci_callback_handle on_write(cci_callback<void(const cci_value&, const cci_value&, const cci_originator&)> callback);

	cci_callback_handle validate_read(cci_callback<bool(const cci_originator&)> callback);

	cci_callback_handle validate_write(cci_callback<bool(const void*, const void*, const cci_originator&)> callback);

	cci_callback_handle on_write(cci_callback<void(const void*, const void*, const cci_originator&)> callback);

	///@}

	///@name Write-access control
	///@{

	/// Locking this parameter, optionally with a password.
	/**
	 * Makes a parameter read-only.
	 *
	 * Returns false
	 * - if this parameter was already locked with a different password (this call has no effect)
	 *
	 * Returns true
	 * - if the parameter was not locked (and is locked now) or
	 * - if the parameter was locked without a password. Then it is locked now with the given password
	 * - if the parameter was locked with the given password pwd. Then it is still locked now with the given password.
	 *
 	 * @param pwd Password needed to unlock the param, ideally any pointer address known only by the locking entity, default = NULL.
	 * @return If the lock was successful.
	 */
	bool lock(void* pwd = NULL);

	/// Unlocking this parameter, optionally with a password if needed.
	/**
	 * @param pwd Password to unlock the param (if needed), default = NULL.
	 * @return If the parameter is unlocked now.
	 */
	bool unlock(void* pwd = NULL);
	
	/// If this parameter is locked.
	/**
	* @return If this parameter is locked
	*/
	bool is_locked() const;

	///@}

	///@name Query parameter type and name
	///@{

	/// Get the name of this parameter.
	/**
	 * @return   Name of the parameter.
	 */
	const std::string& get_name() const;

	///@}

	///@name Accessor methods
	///@{
	
	/// Gets cci_originator of the parameter.
	/**
	 * The originator reflects ownership of the parameter proxy, which points
	 * to an implementation.  For a handle, the originator identifies the
	 * entity accessing the parameter.  Otherwise, the originator reflects
	 * the parameter's creator.
	 */
	cci_originator get_originator() const;
	
	///@}


protected:
	/// Constructor to create new parameter with given originator.
	cci_param_untyped(bool is_top_level_name, cci::cci_broker_if* broker_handle,
				   const std::string& desc, const cci_originator& originator);

	///@name Initialization and Destructions methods
	///@{

	/// Initialize.
	void init();

	///@}

	/// Updates the internal member m_latest_write_access_originator_cp
	void update_latest_write_originator(const cci_originator& originator) const;

	/// Callback forwarder class
	/**
     * This is instantiated and registered at the base param to forward a
     * callback to the cci world when called by the gs_base_param.
     * @see also see similar gs_cci_cnf_broker::internal_callback_forwarder
     */
	class internal_callback_forwarder {
	public:
		internal_callback_forwarder(cci::shared_ptr<cci::callb_adapt> _adapt,
									const gs::cnf::callback_type _type,
									cci_param_untyped_handle& _par)
				: adapt(_adapt.get())
				, type(_type)
				, param(_par)
				, calling_gs_adapter() {
		}
		~internal_callback_forwarder() {
			//cout << "Destructing callback forwarder for param "<<param->get_name() << endl;
			if (calling_gs_adapter) {
				calling_gs_adapter->unregister_at_parameter();
			}
		}
		// This gets called by the base gs_param
		gs::cnf::callback_return_type call(gs::gs_param_base& par,
										   gs::cnf::callback_type cbtype) {
			gs::cnf::callback_return_type returned_gs_message =
					gs::cnf::return_nothing;
			cci::callback_return_type returned_cci_message =
					cci::return_nothing;
			switch(cbtype) {
				case gs::cnf::destroy_param:
					assert(par.is_destructing());
					// TODO This cannot work because the cast the the already destructed cci_param_untyped fails.
					// TODO adapt->call(*cci_p, cci::destroy_param);
					break;
				case gs::cnf::pre_read: // = cci::pre_read
					returned_cci_message = adapt->call(param, cci::pre_read);
					break;
				case gs::cnf::reject_write: // = cci::reject_write
					returned_cci_message = adapt->call(param,
													   cci::reject_write);
					break;
				case gs::cnf::pre_write: // = cci::pre_write
					returned_cci_message = adapt->call(param, cci::pre_write);
					break;
				case gs::cnf::post_write: // = cci::post_write
					returned_cci_message = adapt->call(param, cci::post_write);
					break;
				default:
					returned_cci_message = cci::return_other_error;
					assert(false);
			}
			switch (returned_cci_message) {
				case cci::return_value_change_rejected:
					returned_gs_message = gs::cnf::return_value_change_rejected;
					break;
				case cci::return_other_error:
					returned_gs_message = gs::cnf::return_other_error;
					break;
				default:
					returned_gs_message = gs::cnf::return_nothing;
			}
			return returned_gs_message;
		}
		const gs::cnf::callback_type get_type() {
			return type;
		}

		cci::callb_adapt* adapt;
		gs::cnf::callback_type type;
		cci_param_untyped_handle& param;
		cci::shared_ptr< gs::cnf::ParamCallbAdapt_b> calling_gs_adapter;
	};

	/// Internal guard which is called on a value change to update
	/// the value status variable(s)
	struct status_guard {
		status_guard(cci_param_untyped &_owner)
		{
			owner = dynamic_cast<cci_param_untyped*>(&_owner);
		}
		// This gets called by the base gs_param
		cci::callback_return_type call(cci::cci_param_untyped_handle& changed_param,
									   const cci::callback_type& cb_reason)
		{
			owner->m_is_default_value = false;
			owner->m_is_invalid_value = false;
			return cci::return_nothing;
		}
		cci_param_untyped* owner;
	};

protected:
	/// Callback forwarder
	std::vector<internal_callback_forwarder*> fw_vec;

	/// Associated gs_param_base
	gs::gs_param_base *m_gs_param_base;

	/// Is default value
	bool m_is_default_value;

	/// Is invalid value
	bool m_is_invalid_value;

	/// Status guard
	status_guard m_status_guard;

	/// Description
	std::string m_description;

	/// Metadata
	cci_value_map metadata;

	/// Initialized
	bool m_init_called;

	/// Post write callback
	cci::shared_ptr<cci::callb_adapt> m_post_write_callback;

	/// Broker handle
	cci::cci_broker_if* m_broker_handle;

	/// Stores the originator of the latest successful write access (status within post_write) as an alternative to get originator information within the callback(s)
	mutable cci::cci_originator m_latest_write_access_originator_cp;

	/// Stores if there is a valid m_latest_write_access_originator_cp (latest originator of the latest successful write access)
	mutable bool m_latest_write_access_originator_valid;

	cci_param_untyped_handle* m_param_untyped_handle;

private:
	/// Originator of the parameter
	const cci_originator m_originator;

	std::vector<cci_callback_handle> m_callback_handles;
};

CCI_CLOSE_NAMESPACE_
  
#endif //CCI_BASE_PARAM_H_INCLUDED_
