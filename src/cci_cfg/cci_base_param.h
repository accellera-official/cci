/*****************************************************************************
Copyright 2006-2015 Accellera Systems Initiative Inc.
All rights reserved.

Copyright 2009-2011 GreenSocs Ltd
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

#ifndef CCI_BASE_PARAM_H_INCLUDED_
#define CCI_BASE_PARAM_H_INCLUDED_

#include <string>
#include <vector>
#include "cci_cfg/cci_config_macros.h"
#include "cci_cfg/cci_datatypes.h"
#include "cci_cfg/cci_value.h"
#include "cci_cfg/cci_originator.h"
#include "cci_cfg/cci_shared_ptr.h"
#include "cci_cfg/cci_callbacks.h"

/**
 * @author Enrico Galli, Intel
 */

CCI_OPEN_NAMESPACE_

// Forward declaration of implementation
class cci_param_impl_if;

// CCI Configuration parameter base class
/**
* Type independent base class all cci parameters derive from.
* This class gives some easier access to type independent functions.
*/
class cci_base_param
{

public:

	/// Destructor.
	virtual ~cci_base_param();

	///@name Set and Get with JSON String Representation
	///@{

	/// Sets the value of this parameter given by a JSON string. @todo Alternative name: function set_json_string
	/**
	* @exception cci_exception_set_param Setting value failed
	* @param json_string the new value, represented as a JSON string.
	*/
	void json_deserialize(const std::string& json_string);

	/// Get the JSON string representation of this parameter's value.
	/**
	* @exception cci_exception_get_param Getting value failed
	* @return  The value of this parameter represented as a JSON string.
	*/
	std::string json_serialize() const;
	
	///@}


	///@name JSON Data Type and access
	///@{

	/// Set the parameter's value to the given one.
	/**
	* @exception cci_exception_set_param Setting value failed
	* @param val This value is either (in the case of a pure basic param) converted into a JSON string and stored in the base param or (in the case of a typed parameter) into the actual data type
	*/
	void set_value(const cci_value& val);
	
	/// Get the parameter's value.
	/**
	* @exception cci_exception_get_param Getting value failed
	* @return This value is either (in the case of a pure basic param) converted from the JSON string or (in the case of a typed parameter) from the actual data type
	*/
	cci_value get_value() const;

	///@}


	///@name Documentation
	///@{

	/// Set parameter documentation.
	/**
	* Set the parameter's documentation describing purpose and 
	* intended use, allowed value range etc. in a human readable way.
	*
	* @param doc Human readable documentation
	*/
	void set_documentation(const std::string& doc);
	
	/// Get the parameter's documentation.
	/**
	* return Documentation
	*/
	std::string get_documentation() const;

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
	bool is_default_value();


	/// Indicates that the parameter received an initial value that has not since been modified.
	/**
	* True if the value was supplied using the broker's
	* json_deserialize_initial_value function and not subsequently changed.
	*
	* Note: false is returned even if the current value matches the initial 
	* value but has undergone intermediate changes.
	*
	* @return fase if no initial value was supplied or the parameter's value has
	*         changed; otherwise, true
	*/
	bool is_initial_value() const;

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
	* json_deserialize(), set_value(), cci_param::set(), cci_param::operator=()
	*/
	const cci_originator* get_latest_write_originator() const;

	///@}

	
	///@name Callback Handling
	///@{

	/// Registers functions as a callback.
	shared_ptr<callb_adapt> register_callback(const callback_type type, void* observer, param_callb_func_ptr function);
	
	/// Function registering a callback object (should not be called by user).
	shared_ptr<callb_adapt> register_callback(const callback_type type, shared_ptr<callb_adapt> callb);
	
	/// Unregisters all callbacks (within this parameter) for the specified observer object (e.g. sc_module). 
	/**
	* observer = NULL should be an error (and not mean unregister all callbacks)
	*                 to avoid accidental unregistration of e.g. callbacks initiated
	*                 by a tool.
	* @param observer Pointer to the observer module that registered parameter callbacks.
	*/
	void unregister_all_callbacks(void* observer);
	
	/// Unregisters the callback and (default) deletes the callback adapter.
	/**
	* Unregisters (only) the given callback.
	*
	* The callback adapter is NOT deleted explicitly because this
	* should be done by the surrounding shared pointer!
	*
	* This may be used by a user module which stored the shared pointer to a
	* specific callback or by the destructor of the param callback adapter.
	*
	* @param callb  Parameter callback adapter
	* @return       If the callback adapter existed in this parameter.
	*/
	bool unregister_callback(shared_ptr<callb_adapt> callb); 
	
	/// @see unregister_callback(shared_ptr<>).
	bool unregister_callback(callb_adapt* callb);
	
	/// Returns if the parameter has registered callbacks.
	bool has_callbacks();

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

	/// Returns a basic type this parameter can be converted to or from (which is not necessarily the actual parameter type).
	/**
	* @return Type
	*/
	basic_param_type get_basic_type() const;

	/// Get the name of this parameter.
	/**
	* @return   Name of the parameter.
	*/
	const std::string& get_name() const;

	///@}

	///@name Accessor methods
	///@{

	/// Indicates that this parameter is an accessor to a parameter implementation.
	/**
	* @return   True if the current parameter is an accessor (vs. parameter creator)
	*/
	bool is_accessor() const;
	
	/// Gets cci_originator of the parameter.
	/**
	* The originator reflects ownership of the parameter proxy, which points
	* to an implementation.  For an accessor, the originator identifies the 
	* entity accessing the parameter.  Otherwise, the originator reflects
	* the parameter's creator.
	*/
	cci_originator get_originator() const;

	/// Creates a parameter accessor holding originator information and pointing to this parameter's implementation.
	/**
	* This shall be used by the broker when granting access to an (existing) parameter.
	*
	* @param  originator the entity requesting access to the parameter
	* @return an accessor to this parameter
	*/
	virtual cci_base_param* create_accessor(const cci_originator& originator) = 0;

	/// Indicates that the accessed parameter get destroyed or no.
	/**
	 *
	 * Note: Only valid in case of an accessor.
	 *
	 * @return false if accessed_param_destroyed no more exist; otherwise, true
	 */
	bool is_accessed_param_destroyed() const;

	/// Update status of the accessed parameter as destroyed.
	void set_accessed_param_destroyed();
	
	///@}


protected:
	/// Constructor to create accessor with given originator.
	cci_base_param(cci_base_param & copy, const cci_originator& originator);

	/// Constructor to create new parameter with given originator.
	cci_base_param(cci_param_impl_if& impl, const cci_originator& originator);


	///@name Type-punned value operations
	///@{

	/// Get a pointer to the stored value.
	/**
	* @return Pointer to type-punned value
	*/
	const void* get() const;


	/// Get a pointer to the default value (passed in via constructor).
	/**
	* @return Pointer to type-punned default value
	*/
	const void* get_default_value() const;
	
	/// Sets the value via type-punned argument.
	/**
	* @param vp pointer to type-punned value
	* @pre Type of vp must be equal to the internal type
	*/
	void set(const void* vp);

	/// Sets the value via type-punned argument.
	/**
	* @param vp pointer to type-punned value
	* @param pwd Password needed to unlock the param, ideally any pointer address known only by the locking entity, default = NULL
	* @pre Type of vp must be equal to the internal type
	*/
	void set(const void* vp, const void* pwd); 

	///@name Type-punned value operations
	///@{

	/// Compare parameter values.
	/**
	* @param rhs reference to another cci_param implementation
	* @return True if both values are equal and of the same data type
	*/
	bool equals(const cci_param_impl_if& rhs) const;

	///@}

	///@name Initialization and Destructions methods
	///@{

	/// Initialize.
	void init();

	/// Free resources attached to parameter.
	void destroy();

	///@}

	/// Add parameter accessor
	/**
	* @param accessor Parameter accessor to associate to accessed parameter
	*/
	void add_param_accessor(cci_base_param* accessor);

	/// Remove parameter accessor
	/**
	* @param accessor Parameter accessor to remove from accessed parameter
	*/
	void remove_param_accessor(cci_base_param* accessor);

private:

	/// Originator of the parameter proxy.
	const cci_originator m_originator;

	/// Actual implementation of the param.
	cci_param_impl_if& m_impl;

	/// Indicates whether this proxy is creator of the impl or an accessor to it.
	const bool m_owns_impl;

	/// Parameter accessor(s)
	typedef std::vector<cci_base_param*> param_accessor_vector;
	param_accessor_vector m_param_accessors;

	/// Accessed parameter
	cci_base_param* m_accessed_param;

	/// Check accessed parameter
	void check_accessed_param() const;
};

CCI_CLOSE_NAMESPACE_
  
#endif //CCI_BASE_PARAM_H_INCLUDED_
