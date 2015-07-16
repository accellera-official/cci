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

#ifndef CCI_CNF_BASE_PARAM_H_INCLUDED_
#define CCI_CNF_BASE_PARAM_H_INCLUDED_

#include <string>
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

//Forward declaration of implementation
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

	/// Set the parameter's value to the given one
	/**
	* @exception cci_exception_set_param Setting value failed
	* @param val This value is either (in the case of a pure basic param) converted into a JSON string and stored in the base param or (in the case of a typed parameter) into the actual data type
	*/
	void set_value(const cci_value& val);
	
	/// Get the parameter's value
	/**
	* @exception cci_exception_get_param Getting value failed
	* @return This value is either (in the case of a pure basic param) converted from the JSON string or (in the case of a typed parameter) from the actual data type
	*/
	cci_value get_value() const;

	///@}


	///@name Documentation
	///@{

	/// Set parameter meta data/documentation
	/**
	* Adds parameter meta data which should describe the
	* intended use, allowed value range etc. in a human readable way.
	*
	* @param doc Human readable documentation
	*/
	void set_documentation(const std::string& doc);
	
	/// Get the parameter's meta data/documentation
	/**
	* return Documentation
	*/
	std::string get_documentation() const;

	///@}

	///@name Parameter Value Status
	///@{

	/// Returns if the current value is the default one being set by the constructor and never been modified
	/**
	* True if the value had never been set but only by the constructor.
	*
	* Note that this returns false even if the current value is
	* actually the same as the default one. This returns also false
	* if there had not been set a constructor default value.
	*
	* @return If the parameter's current value is the default one
	*/
	bool is_default_value();
	
	/// Returns if the current value is invalid
	/**
	* Returns true if
	*  - the value has manually been set to be invalid (using function set_invalid_value() ) @see set_invalid_value()
	*
	* @return  If the parameter's current value is invalid
	*/
	bool is_invalid_value(); 

	/// Marks the value to be invalid. (Does not impact the actual value.)
	void set_invalid_value();


	/// Returns if the current value is an initial value being set by the database and not been modified
	/**
	* True if the value has been set using the cnf_broker's
	* json_deserialize_initial_value function and not been modified since then.
	*
	* Note: this will return false if a value that has been set as non-initial
	*       value but is equals the initial value.
	*
	* @return If the parameter's current value is an initial value being set
	*         using the broker function json_deserialize_initial_value
	*/
	bool is_initial_value() const;

	///@}


	///@name Miscellaneous
	///@{

	/// Returns the originator of the latest successful write access 
	/**
	* This is an alternative to get originator information within the callback(s). <br>
	* This returns the originator of the latest post_write. <br>
	* This returns NULL as long as no write had been made.
	*
	* The originator is updated on the calls of the following functions:
	* json_deserialize(), set_value(), set_invalid_value(), cci_param::set(), cci_param::operator=()
	*/
	const cci_originator* get_latest_write_originator() const;

	///@}

	
	///@name Callback Handling
	///@{

	/// Registers functions as a callback
	shared_ptr<callb_adapt> register_callback(const callback_type type, void* observer, param_callb_func_ptr function);
	
	/// Function registering a callback object (should not be called by user)
	shared_ptr<callb_adapt> register_callback(const callback_type type, shared_ptr<callb_adapt> callb);
	
	/// Unregisters all callbacks (within this parameter) for the specified observer object (e.g. sc_module). 
	/**
	* observer = NULL should be an error (and not mean unregister all callbacks)
	*                 to avoid accidental unregistration of e.g. callbacks initiated
	*                 by the/a tool.
	* @param observer   Pointer to the observer module who did register parameter callbacks.
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
	
	/// @see unregister_callback(shared_ptr<>) 
	bool unregister_callback(callb_adapt* callb);
	
	/// Returns if the parameter has registered callbacks
	bool has_callbacks();

	///@}

	///@name Write-access control
	///@{

	/// Locking this parameter, optionally with a password
	/**
	* Makes a parameter read-only.
	*
	* Returns false
	* - if this parameter was already locked with a different password than pwd. Then it is still locked but not with this given/without password.
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

	/// Unlocking this parameter, optionally with a password if needed
	/**
	* @param pwd Password to unlock the param (if needed), default = NULL.
	* @return If the parameter is unlocked now.
	*/
	bool unlock(void* pwd = NULL);
	
	/// If this parameter is locked
	/**
	* @return If this parameter is locked
	*/
	bool is_locked() const;

	///@}

	///@name Query parameter type and name
	///@{

	/// Returns a basic type this parameter can be converted to or from (which is not necessarily the actual parameter type)
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

	///Checks if the current functions 
	/**
	* @return   True if the current parameter is an accessor
	*/
	bool is_accessor() const;
	
	///Gets cci_originator of parameter
	cci_originator get_originator() const;

	///Creates a parameter accessor object holding the originator information and pointing to the same parameter
	/**
	* This shall be used by the broker when returning a not yet created parameter accessor.
	*
	* @param originator  Originator object the returned parameter accessor shall represent
	* @return  A newed copy pointing to the same implementation parameter.
	*          Memory management has to be done by the caller!
	*/
	virtual cci_base_param* create_accessor(const cci_originator& originator) = 0;
	
	///@}


protected:
	///Constructor to create accessor with given originator
	cci_base_param(cci_base_param & copy, const cci_originator& originator);

	///Constructor to create new parameter with given originator
	cci_base_param(cci_param_impl_if& impl, const cci_originator& originator);


	///@name Type-punned value operations
	///@{

	/// Get a pointer to the stored value
	/**
	* @return Pointer to type-punned value stored
	*/
	const void* get() const;


	/// Get a pointer to the default value (passed in via constructor)
	/**
	* @return Pointer to type-punned default value
	*/
	const void* get_default_value() const;
	
	/// Sets the stored value to the value passed in via vp
	/**
	* @param vp pointer to type-punned value
	* @pre Type of vp must be equal to the internal type
	*/
	void set(const void* vp);

	/// Sets the stored value to the value passed in via vp
	/**
	* @param vp pointer to type-punned value
	* @param pwd Password needed to unlock the param, ideally any pointer address known only by the locking entity, default = NULL
	* @pre Type of vp must be equal to the internal type
	*/
	void set(const void* vp, const void* pwd); 

	///@name Type-punned value operations
	///@{

	/// Computer if the stored values are equal
	/**
	* @param rhs reference to another cci_param implementation
	* @return True if both values are equal and of the same type
	*/
	bool equals(const cci_param_impl_if& rhs) const;

	///@}

	///@name Initialization and Destructions methods
	///@{

	/// Initialize 
	void init();

	/// Free resources attached to parameter
	void destroy();

	///@}

private:

	const cci_originator m_originator;
	cci_param_impl_if& m_impl;

};

CCI_CLOSE_NAMESPACE_
  
#endif //CCI_CNF_BASE_PARAM_H_INCLUDED_
