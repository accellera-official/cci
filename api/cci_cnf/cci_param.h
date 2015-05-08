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

#ifndef CCI_CNF_CCI_PARAM_H_INCLUDED_
#define CCI_CNF_CCI_PARAM_H_INCLUDED_

#include "cci_cnf/cci_param_b.h"
#include "cci_cnf/cci_factory.h"
#include "cci_cnf/cci_broker_manager.h"

/**
 * @author Enrico Galli, Intel
 */

CCI_OPEN_CONFIG_NAMESPACE_

/// Parameter class, internally forwarding calls to the implementation
/**
 * The implementation is in file cci_param.hpp, which has to be included 
 * in the headers as well because the class is a template class.
 *
 * Documentation for the undocumented functions can be found in
 * cci_param_impl_if
 */
template<typename T, param_mutable_type TM = mutable_param>
class cci_param 
: public cci_param_b<T>
{

public:
	/// The parameter's value type.
	typedef T value_type;

	using cci_param_b<T>::operator =;
  
	/// Constructor with (local/hierarchical) name and initial value.
	cci_param(const std::string& name, const value_type& value);
	
	/// Constructor with (local/hierarchical) name and initial value.
	cci_param(const std::string& name, const cci_value& value);

	/// Constructor with (local/hierarchical) name and initial value and top-level name.
	cci_param(const std::string& name, const value_type& value, cci_top_level_name);

	/// Constructor with (local/hierarchical) name and initial value and top-level name.
	cci_param(const std::string& name, const cci_value& value, cci_top_level_name);

	/// Constructor with (local/hierarchical) name and initial value and private broker.
	cci_param(const std::string& name, const value_type& value, cci_cnf_broker_if& private_broker);

	/// Constructor with (local/hierarchical) name and initial value and private broker.
	cci_param(const std::string& name, const cci_value& value, cci_cnf_broker_if& private_broker);

	/// Constructor with (local/hierarchical) name and initial value and top-level name and private broker.
	cci_param(const std::string& name, const value_type& value, cci_top_level_name, cci_cnf_broker_if& private_broker);

	/// Constructor with (local/hierarchical) name and initial value and top-level name and private broker.
	cci_param(const std::string& name, const cci_value& value, cci_top_level_name, cci_cnf_broker_if& private_broker);

	///Creates a parameter accessor object holding the originator information and pointing to the same parameter
	/**
	* This shall be used by the broker when returning a not yet created parameter accessor.
	*
	* @param originator  Originator object the returned parameter accessor shall represent
	* @return  A newed copy pointing to the same implementation parameter.
	*          Memory management has to be done by the caller!
	*/
	virtual cci_param<value_type, TM>* create_accessor(const cci_originator& originator);

private:
	/// Constructor to create accessors with a giver originator
	cci_param(cci_param<value_type, TM> & copy, const cci_originator& originator);
};

#define SIGNATURE_HELPER3(a, b, c) a, b, c
#define SIGNATURE_HELPER2(a, b) a, b
#define SIGNATURE_HELPER1(a) a
#define SIGNATURE_APPEND(signature, count, value) (SIGNATURE_HELPER##count signature, value)

#define CCI_PARAM_CONSTRUCTOR_IMPL_A(signature, top, broker)  \
template <typename T, param_mutable_type TM>                  \
cci_param<T, TM>::cci_param signature :                       \
	cci_param_b<T>(                                           \
				*create_cci_param(this, name, value,          \
					                  top, & broker),         \
				PARAM_ORIGINATOR)                             \
{                                                             \
	broker.add_param(this);                                   \
	this->init();                                             \
}

#define CCI_PARAM_CONSTRUCTOR_IMPL_B(signature, count, top) \
	CCI_PARAM_CONSTRUCTOR_IMPL_A(signature, top, cci_broker_manager::get_current_broker(PARAM_ORIGINATOR)) \
	CCI_PARAM_CONSTRUCTOR_IMPL_A(SIGNATURE_APPEND(signature, count, cci_cnf_broker_if& private_broker), top, private_broker)

#define CCI_PARAM_CONSTRUCTOR_IMPL_C(signature) \
	CCI_PARAM_CONSTRUCTOR_IMPL_B(signature, 2, false) \
	CCI_PARAM_CONSTRUCTOR_IMPL_B(SIGNATURE_APPEND(signature, 2, cci_top_level_name), 3, true)

#define CCI_PARAM_CONSTRUCTOR_IMPL(signature) \
	CCI_PARAM_CONSTRUCTOR_IMPL_C(SIGNATURE_APPEND(signature, 1, const value_type& value)) \
	CCI_PARAM_CONSTRUCTOR_IMPL_C(SIGNATURE_APPEND(signature, 1, const cci_value& value))

	CCI_PARAM_CONSTRUCTOR_IMPL((const std::string& name))

#undef CCI_PARAM_CONSTRUCTOR_IMPL
#undef CCI_PARAM_CONSTRUCTOR_IMPL_B
#undef CCI_PARAM_CONSTRUCTOR_IMPL_A
#undef SIGNATURE_APPEND
#undef SIGNATURE_HELPER1
#undef SIGNATURE_HELPER2
#undef SIGNATURE_HELPER3

	template <typename T, param_mutable_type TM>
	cci_param<T, TM>::cci_param(cci_param<value_type, TM>& copy, const cci_originator& originator) : cci_param_b<T>(copy, originator)
	{}


	template <typename T, param_mutable_type TM>
	cci_param<typename cci_param<T, TM>::value_type, TM>* cci_param<T, TM>::create_accessor(const cci_originator& originator)
	{
		cci_originator_lock lock(this->get_originator());
		return new cci_param(*this, originator);
	}

	CCI_CLOSE_CONFIG_NAMESPACE_

#endif
