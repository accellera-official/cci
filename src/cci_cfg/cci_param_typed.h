/*****************************************************************************
  Copyright 2006-2015 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2006-2014 OFFIS Institute for Information Technology
  All rights reserved.

  Copyright 2006-2015 Intel Corporation
  All rights reserved.

  Copyright 2016 Ericsson
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

#ifndef CCI_CCI_PARAM_TYPED_H_INCLUDED_
#define CCI_CCI_PARAM_TYPED_H_INCLUDED_

#include "cci_cfg/cci_param_untyped.h"
#include "cci_cfg/cci_report_handler.h"

/**
 * @author Enrico Galli, Intel
 */
CCI_OPEN_NAMESPACE_

// Convenient name
#define cci_param cci_param_typed

/// Parameter class, internally forwarding calls to the implementation
/**
 * The implementation is in file cci_param_typed.hpp, which has to be included
 * in the headers as well because the class is a template class.
 *
 * Documentation for the undocumented functions can be found in
 * cci_param_if
 */
template<typename T, param_mutable_type TM = mutable_param>
class cci_param_typed : public cci_param_untyped
{
public:
	/// The parameter's value type.
	typedef T value_type;

	///Assigns parameter a new value from another parameter
	/**
	* @param rhs New value to assign
	* @return reference to this object
	*/
	cci_param_typed<value_type, TM>& operator= (const cci_param_typed<T, TM> & rhs);

	///Assigns parameter a new value from another legacy parameter
	/**
	* @param rhs New value to assign
	* @return reference to this object
	*/
	cci_param_typed<value_type, TM>& operator= (const cci_param_if & rhs);

	///Assigns parameter a new value from another parameter handle
	/**
	* @param rhs New value to assign
	* @return reference to this object
	*/
	cci_param_typed<value_type, TM>& operator= (const cci_param_typed_handle<T> & rhs);

	///Assigns parameter a new value from another untyped parameter handle
	/**
	* @param rhs New value to assign
	* @return reference to this object
	*/
	cci_param_typed<value_type, TM>& operator= (const cci_param_untyped_handle & rhs);

	///Assigns parameter a new value
	/**
	 * @param rhs New value to assign
	 * @return reference to this object
	*/
	cci_param_typed<value_type, TM>& operator= (const value_type & rhs);

	///Conversion operator to be able use cci_param_typed as a regular object
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
	void set(const value_type & value, const void * pwd);

	///Get the value passed in via constructor
	const value_type & get_default_value();

	/// Get a pointer to the stored value.
	/**
	 * @return Pointer to type-punned value
	 */
	const void* get_raw_value() const;

    ///Conversion operator to be able use cci_param_typed as a regular object
    const value_type& get_value() const;

	/// Get a pointer to the default value (passed in via constructor).
	/**
	 * @return Pointer to type-punned default value
	 */
	const void* get_default_value_raw() const;

    /// Sets the value via type-punned argument.
    /**
     * @param vp pointer to type-punned value
     * @pre Type of vp must be equal to the internal type
     */
    void set_raw_value(const void* vp);

	/// Sets the value via type-punned argument.
	/**
	 * @param vp pointer to type-punned value
	 * @param originator reference to the originator
	 * @pre Type of vp must be equal to the internal type
	 */
	void set_raw_value(const void* vp, const cci_originator& originator);

    /// Sets the value via type-punned argument.
    /**
     * @param vp pointer to type-punned value
     * @param pwd Password needed to unlock the param, ideally any pointer address known only by the locking entity, default = NULL
     * @pre Type of vp must be equal to the internal type
     */
    void set_raw_value(const void* vp, const void* pwd);

	/// Sets the value via type-punned argument.
	/**
	 * @param vp pointer to type-punned value
	 * @param pwd Password needed to unlock the param, ideally any pointer address known only by the locking entity, default = NULL
	 * @param originator reference to the originator
	 * @pre Type of vp must be equal to the internal type
	 */
	void set_raw_value(const void* vp, const void* pwd, const cci_originator& originator);

	///@name Type-punned value operations
	///@{

	/// Compare parameter handle values.
	/**
	 * @param rhs reference to another cci_param_untyped_handle implementation
	 * @return True if both values are equal and of the same data type
	 */
	bool equals(const cci_param_untyped_handle& rhs) const;

	/// Compare parameter values.
	/**
	 * @param rhs reference to another cci_param_if implementation
	 * @return True if both values are equal and of the same data type
	 */
	bool equals(const cci_param_if& rhs) const;

	/// Returns a basic type this parameter can be converted to or from (which is not necessarily the actual parameter type).
	/**
	 * @return Type
	 */
	basic_param_type get_basic_type() const;

	///@name CCI value Data Type and access
	///@{

    /// Set the parameter's value to the given one.
    /**
     * @exception cci_exception_set_param Setting value failed
     * @param val This value is either (in the case of a pure basic param) converted into a JSON string and stored in the base param or (in the case of a typed parameter) into the actual data type
     */
    void set_cci_value(const cci_value& val);

	/// Set the parameter's value to the given one.
	/**
	 * @exception cci_exception_set_param Setting value failed
	 * @param val This value is either (in the case of a pure basic param) converted into a JSON string and stored in the base param or (in the case of a typed parameter) into the actual data type
	 * @param originator reference to the originator
	 */
	void set_cci_value(const cci_value& val, const cci_originator& originator);

	/// Get the parameter's value.
	/**
	 * @exception cci_exception_get_param Getting value failed
	 * @return This value is either (in the case of a pure basic param) converted from the JSON string or (in the case of a typed parameter) from the actual data type
	 */
	cci::cci_value get_cci_value() const;

	///@}

    /// Free resources attached to parameter.
    void destroy();

    ///Creates a parameter handle object holding the originator information and pointing to the same parameter
    /**
     * This shall be used by the broker when returning a not yet created parameter handle.
     *
     * @param originator  Originator object the returned parameter handle shall represent
     * @return  A newed copy pointing to the same implementation parameter.
     *          Memory management has to be done by the caller!
     */
    cci_param_untyped_handle* create_param_handle(const cci_originator& originator);

    /// Constructor with (local/hierarchical) name and initial value and description.
    cci_param_typed(const std::string& name, const value_type& value, const std::string& desc = "");

    /// Constructor with (local/hierarchical) name and initial value and description.
    cci_param_typed(const std::string& name, const cci_value& value, const std::string& desc = "");

    /// Constructor with (local/hierarchical) name and initial value and top-level name and description.
    cci_param_typed(const std::string& name, const value_type& value, cci_top_level_name, const std::string& desc = "");

    /// Constructor with (local/hierarchical) name and initial value and top-level name and description.
    cci_param_typed(const std::string& name, const cci_value& value, cci_top_level_name, const std::string& desc = "");

    /// Constructor with (local/hierarchical) name and initial value and private broker and description.
    cci_param_typed(const std::string& name, const value_type& value, cci_broker_if& private_broker, const std::string& desc = "");

    /// Constructor with (local/hierarchical) name and initial value and private broker and description.
    cci_param_typed(const std::string& name, const cci_value& value, cci_broker_if& private_broker, const std::string& desc = "");

    /// Constructor with (local/hierarchical) name and initial value and top-level name and private broker and description.
    cci_param_typed(const std::string& name, const value_type& value, cci_top_level_name, cci_broker_if& private_broker, const std::string& desc = "");

    /// Constructor with (local/hierarchical) name and initial value and top-level name and private broker and description.
    cci_param_typed(const std::string& name, const cci_value& value, cci_top_level_name, cci_broker_if& private_broker, const std::string& desc = "");

protected:
	///Constructor to create new parameter with given originator and cci_value
	cci_param_typed(const std::string& name, const cci_value &value,
				bool is_top_level_name, cci::cci_broker_if* broker_handle,
				const std::string& desc,
				const cci_originator & originator);
	///Constructor to create new parameter with given originator and T value
	cci_param_typed(const std::string& name, const value_type &value,
				bool is_top_level_name, cci::cci_broker_if* broker_handle,
				const std::string& desc,
				const cci_originator & originator);

	/// GS Parameter internal
	gs::gs_param<T>* m_gs_param;

private:
    /// Constructor to create handles with a giver originator
    cci_param_typed(cci_param_typed<value_type, TM> & copy, const cci_originator& originator);

    //Used verify kernel phase for elaboration time parameters
    /**
     * Before setValue is called for elaboration parameter updates,
     * this function is called to determine whether the value updates
     * shall be rejected or not
     */
    bool isScElabPhase() {
        if (sc_core::sc_get_status() & (sc_core::SC_BEFORE_END_OF_ELABORATION |
                                        sc_core::SC_ELABORATION |
                                        sc_core::SC_END_OF_ELABORATION)) {
            return true;
        } else {
            return false;
        }
    }

    /// Check whether value update is allowed
    //@todo Should these checks be moved into the parameter proxy (cci_param_typed)?
    bool set_cci_value_allowed()
    {
        if (TM == cci::elaboration_time_param) {
            if(!isScElabPhase()) {
                cci::cci_report_handler::set_param_failed("Attempt to set elaboration parameter during simulation.");
                return false;
            }
        } else if (TM == cci::immutable_param) {
            cci::cci_report_handler::set_param_failed("Parameter is immutable.");
            return false;
        }
        return true;
    }
};

template <typename T, param_mutable_type TM>
cci_param_typed<typename cci_param_typed<T, TM>::value_type, TM>& cci_param_typed<T, TM>::operator=(const cci_param_typed<T, TM>& rhs)
{
	set(rhs.get_value());
	return *this;
}

template <typename T, param_mutable_type TM>
cci_param_typed<typename cci_param_typed<T, TM>::value_type, TM>& cci_param_typed<T, TM>::operator=(const cci_param_if& rhs)
{
    set_cci_value(rhs.get_cci_value());
    return *this;
}

template <typename T, param_mutable_type TM>
cci_param_typed<typename cci_param_typed<T, TM>::value_type, TM>& cci_param_typed<T, TM>::operator=(const cci_param_typed_handle<T>& rhs)
{
    set(rhs.get_value());
    return *this;
}

template <typename T, param_mutable_type TM>
cci_param_typed<typename cci_param_typed<T, TM>::value_type, TM>& cci_param_typed<T, TM>::operator=(const cci_param_untyped_handle& rhs)
{
    set_cci_value(rhs.get_cci_value());
    return *this;
}

template <typename T, param_mutable_type TM>
cci_param_typed<typename cci_param_typed<T, TM>::value_type, TM>& cci_param_typed<T, TM>::operator=(const value_type& rhs)
{
	set(rhs);
	return *this;
}

template <typename T, param_mutable_type TM>
cci_param_typed<T, TM>::operator const value_type&() const
{
	return m_gs_param->getValue();
}

template <typename T, param_mutable_type TM>
const T& cci_param_typed<T, TM>::get_value() const
{
    return m_gs_param->getValue();
}

template <typename T, param_mutable_type TM>
void cci_param_typed<T, TM>::set_raw_value(const void* value)
{
	set_raw_value(value, get_originator());
}

template <typename T, param_mutable_type TM>
void cci_param_typed<T, TM>::set_raw_value(const void* value, const cci_originator& originator)
{
	value_type val = *static_cast<const value_type*>(value);
	if (m_gs_param->locked())
	{
		cci::cci_report_handler::set_param_failed("Parameter locked.");
		return;
	}
    if (set_cci_value_allowed()) {
        if (!m_gs_param->setValue(val))
            cci::cci_report_handler::set_param_failed(
                    "Value rejected by callback.");
        else
            update_latest_write_originator(originator);
    }
}

template <typename T, param_mutable_type TM>
void cci_param_typed<T, TM>::set_raw_value(const void* value, const void *pwd)
{
    set_raw_value(value, pwd, get_originator());
}

template <typename T, param_mutable_type TM>
void cci_param_typed<T, TM>::set_raw_value(const void* value, const void *pwd,
                         const cci_originator& originator)
{
	if (!m_gs_param->check_pwd(pwd)) {
		cci::cci_report_handler::set_param_failed("Wrong key.");
		return;
	}
    if (set_cci_value_allowed()) {
        if (!m_gs_param->setValue(*static_cast<const value_type *>(value), pwd))
            cci::cci_report_handler::set_param_failed("Bad value.");
        else
            update_latest_write_originator(originator);
    }
}

template <typename T, param_mutable_type TM>
void cci_param_typed<T, TM>::set(const value_type& value)
{
    set_raw_value(&value);
}

template <typename T, param_mutable_type TM>
void cci_param_typed<T, TM>::set(const value_type& value, const void *pwd)
{
	set_raw_value(&value, pwd);
}

template <typename T, param_mutable_type TM>
const void* cci_param_typed<T, TM>::get_raw_value() const
{
	return &m_gs_param->getValue();
}

template <typename T, param_mutable_type TM>
bool cci_param_typed<T, TM>::equals(const cci::cci_param_untyped_handle& rhs) const
{
    return rhs.equals(*this);
}

template <typename T, param_mutable_type TM>
bool cci_param_typed<T, TM>::equals(const cci::cci_param_if& rhs) const
{
	const cci_param_typed<T, TM>* other = dynamic_cast<const cci_param_typed<T, TM>*>(&rhs);
	if (other)
	{
		return other->m_gs_param->getValue() == this->m_gs_param->getValue();
	}
	return false;
}

template <typename T, param_mutable_type TM>
cci::basic_param_type cci_param_typed<T, TM>::get_basic_type() const
{
	return get_cci_value().basic_type();
}

template <typename T, param_mutable_type TM>
const void* cci_param_typed<T, TM>::get_default_value_raw() const {
	if (!this->m_gs_param->has_default_value())
		cci::cci_report_handler::get_param_failed("Param has no default value.");
	return &this->m_gs_param->get_default_value();
}

template <typename T, param_mutable_type TM>
const typename cci_param_typed<T, TM>::value_type& cci_param_typed<T, TM>::get_default_value()
{
	return *static_cast<const value_type *>(get_default_value());
}

template <typename T, param_mutable_type TM>
void cci_param_typed<T, TM>::set_cci_value(const cci_value& val)
{
    set_cci_value(val, get_originator());
}

template <typename T, param_mutable_type TM>
void cci_param_typed<T, TM>::set_cci_value(const cci_value& val,
                               const cci_originator& originator)
{
	value_type v = val.get<value_type>();
	set_raw_value(&v, originator);
}

template <typename T, param_mutable_type TM>
cci::cci_value cci_param_typed<T, TM>::get_cci_value() const {
	return cci::cci_value(this->m_gs_param->getValue());
}

template <typename T, param_mutable_type TM>
cci_param_untyped_handle* cci_param_typed<T, TM>::create_param_handle(const cci_originator& originator)
{
    return new cci_param_typed_handle<T>(*this, originator);
}

template <typename T, param_mutable_type TM>
void cci_param_typed<T, TM>::destroy()
{
    delete this;
}

/// Constructors

#define CCI_PARAM_CONSTRUCTOR_CCI_VALUE_IMPL(signature, top, broker)           \
template <typename T, param_mutable_type TM>                                   \
cci_param_typed<T, TM>::cci_param_typed signature                              \
: cci_param_untyped(top, &broker, desc, cci_originator()),                     \
  m_gs_param(new gs::gs_param<T>(name, "", NULL, top, true))                   \
{                                                                              \
    m_gs_param->setString(value.to_json());                     			   \
    cci_param_untyped::m_gs_param_base = m_gs_param;                           \
    broker.add_param(this);                                          		   \
    this->init();                                                              \
}

#define CCI_PARAM_CONSTRUCTOR_IMPL(signature, top, broker)                     \
template <typename T, param_mutable_type TM>                                   \
cci_param_typed<T, TM>::cci_param_typed signature                              \
: cci_param_untyped(top, &broker, desc, cci_originator()),                     \
  m_gs_param(new gs::gs_param<T>(name, value, top))                            \
{                                                                              \
    std::string init_value =                                                   \
        broker.get_initial_cci_value(get_name()).to_json();                    \
    if(!init_value.empty()) {                                                  \
        m_gs_param.setString(init_value);                                      \
    }                                                                          \
    cci_param_untyped::m_gs_param_base = m_gs_param;                           \
    broker.add_param(this);                                                    \
    this->init();                                                              \
}

/// Constructor with (local/hierarchical) name and initial value and description.
CCI_PARAM_CONSTRUCTOR_IMPL((const std::string& name, const value_type& value,
                            const std::string& desc), false,
                            cci_broker_manager::get_current_broker(cci_originator()))

/// Constructor with (local/hierarchical) name and initial value and description.
CCI_PARAM_CONSTRUCTOR_CCI_VALUE_IMPL((const std::string& name, const cci_value& value,
                                      const std::string& desc), false,
                                      cci_broker_manager::get_current_broker(cci_originator()))

/// Constructor with (local/hierarchical) name and initial value and
/// top-level name and description.
CCI_PARAM_CONSTRUCTOR_IMPL((const std::string& name, const value_type& value,
                            cci_top_level_name, const std::string& desc),
                            true, cci_broker_manager::get_current_broker(cci_originator()))

/// Constructor with (local/hierarchical) name and initial value and
/// top-level name and description.
CCI_PARAM_CONSTRUCTOR_CCI_VALUE_IMPL((const std::string& name, const cci_value& value,
                                      cci_top_level_name, const std::string& desc),
                                      true, cci_broker_manager::get_current_broker(cci_originator()))

/// Constructor with (local/hierarchical) name and initial value and
/// private broker and description.
CCI_PARAM_CONSTRUCTOR_IMPL((const std::string& name, const value_type& value,
                            cci_broker_if& private_broker, const std::string& desc),
                            false, private_broker)

/// Constructor with (local/hierarchical) name and initial value and
/// private broker and description.
CCI_PARAM_CONSTRUCTOR_CCI_VALUE_IMPL((const std::string& name, const cci_value& value,
                                      cci_broker_if& private_broker, const std::string& desc),
                                      false, private_broker)

/// Constructor with (local/hierarchical) name and initial value and
/// top-level name and private broker and description.
CCI_PARAM_CONSTRUCTOR_IMPL((const std::string& name, const value_type& value,
                            cci_top_level_name, cci_broker_if& private_broker,
                            const std::string& desc), true, private_broker)

/// Constructor with (local/hierarchical) name and initial value and
/// top-level name and private broker and description.
CCI_PARAM_CONSTRUCTOR_CCI_VALUE_IMPL((const std::string& name, const cci_value& value,
                                      cci_top_level_name, cci_broker_if& private_broker,
                                      const std::string& desc), true, private_broker)

#undef CCI_PARAM_CONSTRUCTOR_IMPL
#undef CCI_PARAM_CONSTRUCTOR_CCI_VALUE_IMPL

template <typename T, param_mutable_type TM>
cci_param_typed<T, TM>::cci_param_typed(cci_param_typed<value_type, TM>& copy, const cci_originator& originator)
: cci_param_untyped(copy, originator),
  m_gs_param(copy.m_gs_param)
{
	cci_param_untyped::m_gs_param_base = m_gs_param;
}

CCI_CLOSE_NAMESPACE_

#endif //CCI_CNF_CCI_PARAM_H_INCLUDED_
