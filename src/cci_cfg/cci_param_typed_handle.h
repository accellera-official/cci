/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
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

#ifndef CCI_CFG_CCI_PARAM_TYPED_HANDLE_H_INCLUDED_
#define CCI_CFG_CCI_PARAM_TYPED_HANDLE_H_INCLUDED_

#include "cci_cfg/cci_param_untyped_handle.h"

/**
 * @author Guillaume Delbergue, Ericsson / GreenSocs
 */

CCI_OPEN_NAMESPACE_

// CCI Configuration parameter handle class
/**
* Typed base class all cci parameters derive from.
* This class provided helpers for casting in and out of the base typeless functions
*/
template<typename T>
class cci_param_typed_handle : public cci_param_untyped_handle
{
public:
    /// The parameter's value type.
    typedef T value_type;

    ///Assigns parameter a new value from another parameter handle
    /**
     * @param rhs New value to assign
     * @return reference to this object
     */
    cci_param_typed_handle<value_type>& operator= (const cci_param_typed_handle<value_type> & rhs);

    ///Assigns parameter a new value from another untyped parameter handle
    /**
     * @param rhs New value to assign
     * @return reference to this object
     */
    cci_param_typed_handle<value_type>& operator= (const cci_param_untyped_handle & rhs);

    ///Assigns parameter a new value from another parameter
    /**
     * @param rhs New value to assign
     * @return reference to this object
     */
    cci_param_typed_handle<value_type>& operator= (const cci_param_typed<value_type> & rhs);

    ///Assigns parameter a new value
    /**
     * @param rhs New value to assign
     * @return reference to this object
     */
    cci_param_typed_handle<value_type>& operator= (const value_type & rhs);

    ///Assigns parameter a new value from another legacy parameter
    /**
     * @param rhs New value to assign
     * @return reference to this object
     */
    cci_param_typed_handle<value_type>& operator= (const cci_param_if & rhs);

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

    ///Gets the stored value
    const value_type& get_value() const;

    ///Get the value passed in via constructor
    const value_type & get_default_value();

    /// @name Write callback handling
    /// @{

    // Untyped callbacks

    /// @copydoc cci_param_untyped::register_write_callback(const cci_param_write_callback_untyped, cci_untyped_tag)
    cci_callback_untyped_handle register_write_callback(
            const cci_param_write_callback_untyped &cb,
            cci_untyped_tag);

    /// @copydoc cci_param_untyped::register_write_callback(cci_param_write_callback_untyped::signature, C*, cci_untyped_tag)
    template<typename C>
    cci_callback_untyped_handle register_write_callback(
            cci_param_write_callback_untyped::signature (C::*cb), C* obj,
            cci_untyped_tag);

    // Typed callbacks

    /// Typed write callback type
    typedef typename cci_param_write_callback<value_type>::type
            cci_param_write_callback_typed;

    /// @copydoc cci_param_typed::register_write_callback(const cci_param_write_callback_typed&, cci_typed_tag<value_type>)
    cci_callback_untyped_handle register_write_callback(
            const cci_param_write_callback_typed& cb,
            cci_typed_tag<value_type> = cci_typed_tag<value_type>());

    /// @copydoc cci_param_typed::register_write_callback(typename cci_param_write_callback_typed::signature, C*, cci_typed_tag<value_type>)
    template<typename C>
    cci_callback_untyped_handle register_write_callback(
            typename cci_param_write_callback_typed::signature (C::*cb),
            C* obj, cci_typed_tag<value_type> = cci_typed_tag<value_type>());

    /// @copydoc cci_param_typed::unregister_write_callback
    bool unregister_write_callback(const cci_param_write_callback_typed &cb);

    /// @}

    /// @name Validate write callback handling
    /// @{

    // Untyped callbacks

    /// @copydoc cci_param_typed::register_validate_write_callback(const cci_param_validate_write_callback_untyped, cci_untyped_tag)
    cci_callback_untyped_handle register_validate_write_callback(
            const cci_param_validate_write_callback_untyped &cb,
            cci_untyped_tag);

    /// @copydoc cci_param_typed::register_validate_write_callback(cci_param_validate_write_callback_untyped::signature, C*, cci_untyped_tag)
    template<typename C>
    cci_callback_untyped_handle register_validate_write_callback(
            cci_param_validate_write_callback_untyped::signature (C::*cb), C* obj,
            cci_untyped_tag);

    // Typed callbacks

    /// Typed validate write callback type
    typedef typename cci_param_validate_write_callback<value_type>::type
            cci_param_validate_write_callback_typed;

    /// @copydoc cci_param_typed::register_validate_write_callback(const cci_param_validate_write_callback_typed&, cci_typed_tag<value_type>)
    cci_callback_untyped_handle register_validate_write_callback(
            const cci_param_validate_write_callback_typed& cb,
            cci_typed_tag<value_type> = cci_typed_tag<value_type>());

    /// @copydoc cci_param_typed::register_validate_write_callback(typename cci_param_validate_write_callback_typed::signature, C*, cci_typed_tag<value_type>)
    template<typename C>
    cci_callback_untyped_handle register_validate_write_callback(
            typename cci_param_validate_write_callback_typed::signature (C::*cb),
            C* obj, cci_typed_tag<value_type> = cci_typed_tag<value_type>());

    /// @copydoc cci_param_typed::unregister_validate_write_callback
    bool unregister_validate_write_callback(
            const cci_param_validate_write_callback_typed &cb);

    /// @}

    /// @name Read callback handling
    /// @{

    // Untyped callbacks

    /// @copydoc cci_param_typed::register_read_callback(const cci_param_read_callback_untyped, cci_untyped_tag)
    cci_callback_untyped_handle register_read_callback(
            const cci_param_read_callback_untyped &cb,
            cci_untyped_tag);

    /// @copydoc cci_param_typed::register_read_callback(cci_param_read_callback_untyped::signature, C*, cci_untyped_tag)
    template<typename C>
    cci_callback_untyped_handle register_read_callback(
            cci_param_read_callback_untyped::signature (C::*cb), C* obj,
            cci_untyped_tag);

    // Typed callbacks

    /// Typed read callback type
    typedef typename cci_param_read_callback<value_type>::type
            cci_param_read_callback_typed;

    /// @copydoc cci_param_typed::register_read_callback(const cci_param_read_callback_typed&, cci_typed_tag<value_type>)
    cci_callback_untyped_handle register_read_callback(
            const cci_param_read_callback_typed &cb,
            cci_typed_tag<value_type> = cci_typed_tag<value_type>());

    /// @copydoc cci_param_typed::register_read_callback(typename cci_param_read_callback_typed::signature, C*, cci_typed_tag<value_type>)
    template<typename C>
    cci_callback_untyped_handle register_read_callback(
            typename cci_param_read_callback_typed::signature (C::*cb), C* obj,
            cci_typed_tag<value_type> = cci_typed_tag<value_type>());

    /// @copydoc cci_param_untyped::unregister_read_callback
    bool unregister_read_callback(
            const cci_param_read_callback_typed &cb);

    /// @}

    /// Constructor to create new parameter handle with given originator and
    /// original parameter
    cci_param_typed_handle(cci_param_if& orig_param, const cci_originator& originator);

    /// Constructor to create a typed parameter handle
    cci_param_typed_handle(cci_param_untyped_handle untyped);
};

template <typename T>
cci_param_typed_handle<typename cci_param_typed_handle<T>::value_type>& cci_param_typed_handle<T>::operator=(const cci_param_typed_handle<T>& rhs)
{
    set(rhs.get_value());
    return *this;
}

template <typename T>
cci_param_typed_handle<typename cci_param_typed_handle<T>::value_type>& cci_param_typed_handle<T>::operator=(const cci_param_untyped_handle& rhs)
{
    set_cci_value(rhs.get_cci_value());
    return *this;
}

template <typename T>
cci_param_typed_handle<typename cci_param_typed_handle<T>::value_type>& cci_param_typed_handle<T>::operator=(const cci_param_typed<T>& rhs)
{
    set(rhs.get_value());
    return *this;
}

template <typename T>
cci_param_typed_handle<typename cci_param_typed_handle<T>::value_type>& cci_param_typed_handle<T>::operator=(const cci_param_if& rhs)
{
    set_cci_value(rhs.get_cci_value());
    return *this;
}

template <typename T>
cci_param_typed_handle<typename cci_param_typed_handle<T>::value_type>& cci_param_typed_handle<T>::operator=(const value_type& rhs)
{
    set(rhs);
    return *this;
}

template <typename T>
cci_param_typed_handle<T>::operator const T&() const
{
    return get_value();
}

template <typename T>
void cci_param_typed_handle<T>::set(const value_type& value)
{
    cci_param_untyped_handle::set_raw_value(&value);
}

template <typename T>
void cci_param_typed_handle<T>::set(const value_type& value, const void *pwd)
{
    cci_param_untyped_handle::set_raw_value(&value, pwd);
}

template <typename T>
const T& cci_param_typed_handle<T>::get_value() const
{
    return *static_cast<const value_type *>(cci_param_untyped_handle::get_raw_value());
}

template <typename T>
const typename cci_param_typed_handle<T>::value_type& cci_param_typed_handle<T>::get_default_value()
{
    return *static_cast<const value_type *>(cci_param_untyped_handle::get_default_value_raw());
}

template <typename T>
cci_callback_untyped_handle
cci_param_typed_handle<T>::register_write_callback(
        const cci_param_write_callback_untyped &cb, cci_untyped_tag)
{
    return cci_param_untyped_handle::register_write_callback(cb);
}

template<typename T>
template<typename C>
cci_callback_untyped_handle
cci_param_typed_handle<T>::register_write_callback(
        cci_param_write_callback_untyped::signature (C::*cb),
        C* obj, cci_untyped_tag)
{
    return cci_param_untyped_handle::register_write_callback(cb, obj);
}

template <typename T>
cci_callback_untyped_handle
cci_param_typed_handle<T>::register_write_callback(
        const cci_param_write_callback_typed& cb,
        cci_typed_tag<T>)
{
    return cci_param_untyped_handle::register_write_callback(
            cb, cci_typed_tag<void>());
}

template<typename T>
template<typename C>
cci_callback_untyped_handle
cci_param_typed_handle<T>::register_write_callback(
        typename cci_param_write_callback_typed::signature (C::*cb)
        , C* obj, cci_typed_tag<T>)
{
    return register_write_callback(sc_bind(cb, obj, sc_unnamed::_1));
}

template <typename T>
bool
cci_param_typed_handle<T>::unregister_write_callback(
        const cci_param_write_callback_typed &cb)
{
    return cci_param_untyped_handle::unregister_write_callback(cb);
}

template <typename T>
cci_callback_untyped_handle
cci_param_typed_handle<T>::register_validate_write_callback(
        const cci_param_validate_write_callback_untyped &cb, cci_untyped_tag)
{
    return cci_param_untyped_handle::register_validate_write_callback(cb);
}

template<typename T>
template<typename C>
cci_callback_untyped_handle
cci_param_typed_handle<T>::register_validate_write_callback(
        cci_param_validate_write_callback_untyped::signature (C::*cb),
        C* obj, cci_untyped_tag)
{
    return cci_param_untyped_handle::register_validate_write_callback(cb, obj);
}

template <typename T>
cci_callback_untyped_handle
cci_param_typed_handle<T>::register_validate_write_callback(
        const cci_param_validate_write_callback_typed& cb,
        cci_typed_tag<T>)
{
    return cci_param_untyped_handle::register_validate_write_callback(
            cb, cci_typed_tag<void>());
}

template<typename T>
template<typename C>
cci_callback_untyped_handle
cci_param_typed_handle<T>::register_validate_write_callback(
        typename cci_param_validate_write_callback_typed::signature (C::*cb)
        , C* obj, cci_typed_tag<T>)
{
    return register_validate_write_callback(sc_bind(cb, obj, sc_unnamed::_1));
}

template <typename T>
bool
cci_param_typed_handle<T>::unregister_validate_write_callback(
        const cci_param_validate_write_callback_typed &cb)
{
    return cci_param_untyped_handle::unregister_validate_write_callback(cb);
}

template <typename T>
cci_callback_untyped_handle
cci_param_typed_handle<T>::register_read_callback(
        const cci_param_read_callback_untyped &cb, cci_untyped_tag)
{
    return cci_param_untyped_handle::register_read_callback(cb);
}

template<typename T>
template<typename C>
cci_callback_untyped_handle
cci_param_typed_handle<T>::register_read_callback(
        cci_param_read_callback_untyped::signature (C::*cb),
        C* obj, cci_untyped_tag)
{
    return cci_param_untyped_handle::register_read_callback(cb, obj);
}

template <typename T>
cci_callback_untyped_handle
cci_param_typed_handle<T>::register_read_callback(
        const cci_param_read_callback_typed& cb,
        cci_typed_tag<T>)
{
    return cci_param_untyped_handle::register_read_callback(
            cb, cci_typed_tag<void>());
}

template<typename T>
template<typename C>
cci_callback_untyped_handle
cci_param_typed_handle<T>::register_read_callback(
        typename cci_param_read_callback_typed::signature (C::*cb)
        , C* obj, cci_typed_tag<T>)
{
    return register_read_callback(sc_bind(cb, obj, sc_unnamed::_1));
}

template <typename T>
bool
cci_param_typed_handle<T>::unregister_read_callback(
        const cci_param_read_callback_typed &cb)
{
    return cci_param_untyped_handle::unregister_read_callback(cb);
}

template <typename T>
cci_param_typed_handle<T>::cci_param_typed_handle(cci_param_if& orig_param,
                                                  const cci_originator& originator)
 : cci_param_untyped_handle(orig_param, originator)
{ }

template <typename T>
cci_param_typed_handle<T>::cci_param_typed_handle(cci_param_untyped_handle untyped)
 : cci_param_untyped_handle(untyped)
{
    if(is_valid() && typeid(T) != get_type_info()) {
        invalidate();
    }
}

CCI_CLOSE_NAMESPACE_

#endif //CCI_CFG_CCI_BASE_PARAM_HANDLE_H_INCLUDED_
