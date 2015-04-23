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

#ifndef CCI_CNF_CCI_PARAM_B_H_INCLUDED_
#define CCI_CNF_CCI_PARAM_B_H_INCLUDED_

#include "cci_cnf/cci_base_param.h"

/**
 * @author Enrico Galli, Intel
 */
CCI_OPEN_CONFIG_NAMESPACE_

template<class T>
class cci_param_b : public cci_base_param
{
public:
	typedef T value_type;

	///Assigns parameter a new value from another parameter
	/**
	* @param rhs New value to assign
	* @return reference to this object
	*/
	cci_param_b<value_type>& operator= (const cci_param_b<T> & rhs);

	///Assigns parameter a new value
	/**
	 * @param rhs New value to assign
	 * @return reference to this object
	*/
	cci_param_b<value_type>& operator= (const value_type & rhs);

	///Conversion operator to be able use cci_param as a regular object
	operator const value_type& () const;
	
	///Sets the stored value to a new value 
	/**
	* @param value new value to assign
	*/
	void set(const value_type& value);
	///Sets the stored value to a new value 
	/**
	 * @param value new value to assign
	 * @param pwd Password needed to unlock the param, ideally any pointer address known only by the locking entity, default = NULL
	*/
	void set(const value_type & value, const void * lock_pwd);

	///Gets the stored value
	const value_type & get() const;

	///Get the value passed in via constructor
	const value_type & get_default_value();

	/// Copy myself; The copy is a parameter accessor object holding the originator information and pointing to the same parameter
	/**
	* This shall be used by the broker when returning a not yet created parameter accessor.
	*
	* @param originator  Originator object the returned parameter accessor shall represent
	* @return  A newed copy pointing to the same implementation parameter.
	*          Memory management has to be done by the caller!
	*/
	virtual cci_param_b<value_type>* create_accessor(const cci_originator& originator) = 0;

protected:
	cci_param_b(cci_param_b<value_type> & copy, const cci_originator & originator);
	cci_param_b(cci_param_impl_if &impl, const cci_originator & originator);
};

template <class T>
cci_param_b<typename cci_param_b<T>::value_type>& cci_param_b<T>::operator=(const cci_param_b<T>& rhs)
{
	set(rhs.get());
	return *this;
}

	template <class T>
cci_param_b<typename cci_param_b<T>::value_type>& cci_param_b<T>::operator=(const value_type& rhs)
{
	set(rhs);
	return *this;
}

template <class T>
cci_param_b<T>::operator const T&() const
{
	return get();
}

template <class T>
void cci_param_b<T>::set(const value_type& value)
{
	cci_base_param::set(&value);
}

template <class T>
void cci_param_b<T>::set(const value_type& value, const void *pwd)
{
	cci_base_param::set(&value, pwd);
}

template <class T>
const typename cci_param_b<T>::value_type& cci_param_b<T>::get() const
{
	return *static_cast<const value_type *>(cci_base_param::get());
}

template <class T>
const typename cci_param_b<T>::value_type& cci_param_b<T>::get_default_value()
{
	return *static_cast<const value_type *>(cci_base_param::get_default_value());
}

template <class T>
cci_param_b<T>::cci_param_b(cci_param_b<value_type>& copy, const cci_originator& originator) : cci_base_param(copy, originator)
{ }

template <class T>
cci_param_b<T>::cci_param_b(cci_param_impl_if& impl, const cci_originator& originator) : cci_base_param(impl, originator)
{ }

CCI_CLOSE_CONFIG_NAMESPACE_

#endif //CCI_CNF_CCI_PARAM_B_H_INCLUDED_
