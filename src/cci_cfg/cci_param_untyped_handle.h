/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 ****************************************************************************/

#ifndef CCI_CCI_PARAM_UNTYPED_HANDLE_H_INCLUDED_
#define CCI_CCI_PARAM_UNTYPED_HANDLE_H_INCLUDED_

#include <string>
#include "cci_cfg/cci_config_macros.h"
#include "cci_core/cci_core_types.h"
#include "cci_core/cci_value.h"
#include "cci_cfg/cci_originator.h"
#include "cci_cfg/cci_mutable_types.h"
#include "cci_cfg/cci_param_callbacks.h"

/**
 * @author Guillaume Delbergue, Ericsson / GreenSocs
 */

CCI_OPEN_NAMESPACE_

// Forward declaration
class cci_param_if;

// Convenient name
#define cci_param_handle cci_param_untyped_handle

// CCI Configuration parameter base class
/**
* Type independent base class all cci parameters derive from.
* This class gives some easier access to type independent functions.
*/
class cci_param_untyped_handle
{

public:
    /// Constructor to create handle with given originator.
    cci_param_untyped_handle(cci_param_if & orig_param,
                             const cci_originator& originator);

    /// Constructor to create an invalid param handle with given originator.
    explicit cci_param_untyped_handle(const cci_originator& originator);

    /// Constructor to create an invalid param handle with given originator.
    cci_param_untyped_handle(const cci_originator& originator,
                                      const std::string&);

    /// Copy constructor
    cci_param_untyped_handle(const cci_param_untyped_handle& param_handle);

    /// Destructor.
    virtual ~cci_param_untyped_handle();

    ///@name Description
    ///@{

    /// @copydoc cci_param_untyped::get_description
    virtual std::string get_description() const;

    ///@}

    ///@name Metadata
    ///@{

    /// @copydoc cci_param_untyped::get_metadata
    cci_value_map get_metadata() const;

    ///@}

    ///@name CCI value Data Type and access
    ///@{

    /// @copydoc cci_param_typed::set_cci_value
    void set_cci_value(const cci_value& val);

    /// @copydoc cci_param_typed::get_cci_value
    cci_value get_cci_value() const;

    /// @copydoc cci_param_typed::get_mutable_type
    cci_param_mutable_type get_mutable_type() const;

    /// @copydoc cci_param_typed::get_default_cci_value
    cci_value get_default_cci_value() const;

    ///@}

    ///@name Parameter Value Status
    ///@{

    /// @copydoc cci_param_untyped::is_default_value
    virtual bool is_default_value();


    /// @copydoc cci_param_untyped::is_initial_value
    virtual bool is_initial_value() const;

    ///@}


    ///@name Miscellaneous
    ///@{

    /// @copydoc cci_param_untyped::get_latest_write_originator
    const cci_originator& get_latest_write_originator() const;

    ///@}

#define CCI_PARAM_UNTYPED_HANDLE_CALLBACK_DECL_(name)                          \
    /* @copydoc cci_param_untyped::register_##name##_callback(const cci_param_##name##_callback_untyped, cci_untyped_tag) */ \
    cci_callback_untyped_handle register_##name##_callback(                    \
            const cci_param_##name##_callback_untyped &cb,                     \
            cci_untyped_tag = cci_untyped_tag());                              \
                                                                               \
    /* @copydoc cci_param_untyped::register_##name##_callback(cci_param_##name##_callback_untyped::signature, C*, cci_untyped_tag) */ \
    template<typename C>                                                                        \
    cci_callback_untyped_handle register_##name##_callback(                    \
            cci_param_##name##_callback_untyped::signature (C::*cb), C* obj,   \
            cci_untyped_tag = cci_untyped_tag())                               \
    {                                                                          \
        return register_##name##_callback(sc_bind(cb, obj, sc_unnamed::_1));   \
    }                                                                          \
                                                                               \
    /* @copydoc cci_param_untyped::unregister_##name##_callback(const cci_callback_untyped_handle) */ \
    bool unregister_##name##_callback(const cci_callback_untyped_handle &cb);  \
                                                                               \
    /* TODO: doc */                                                            \
    cci_callback_untyped_handle register_##name##_callback(                    \
            const cci_callback_untyped_handle& cb, cci_typed_tag<void>)

    /// @name Pre write callback handling
    /// @{

    CCI_PARAM_UNTYPED_HANDLE_CALLBACK_DECL_(pre_write);

    /// @}

    /// @name Post write callback handling
    /// @{

    CCI_PARAM_UNTYPED_HANDLE_CALLBACK_DECL_(post_write);

    /// @}

    /// @name Pre read callback handling
    /// @{

    CCI_PARAM_UNTYPED_HANDLE_CALLBACK_DECL_(pre_read);

    /// @}

    /// @name Post read callback handling
    /// @{

    CCI_PARAM_UNTYPED_HANDLE_CALLBACK_DECL_(post_read);

    /// @}

#undef CCI_PARAM_UNTYPED_HANDLE_CALLBACK_DECL_

    /// @name CCI callback handling
    /// @{

    /// @copydoc cci_param_untyped::unregister_all_callbacks
    bool unregister_all_callbacks();

    /// @copydoc cci_param_untyped::has_callbacks
    bool has_callbacks() const;

    /// @}

    ///@name Write-access control
    ///@{

    /// @copydoc cci_param_untyped::lock
    bool lock(void* pwd = NULL);

    /// @copydoc cci_param_untyped::unlock
    bool unlock(void* pwd = NULL);

    /// @copydoc cci_param_untyped::is_locked
    bool is_locked() const;

    ///@}

    ///@name Query parameter type and name
    ///@{

    /// @copydoc cci_param_typed::get_data_category
    cci_param_data_category get_data_category() const;

    /// @copydoc cci_param_untyped::get_name
    const std::string& get_name() const;

    ///@}

    /// @copydoc cci_param_untyped::get_originator
    cci_originator get_originator() const;

    ///@}

    ///@name Type-punned value operations
    ///@{

    /// @copydoc cci_param_typed::equals
    bool equals(const cci_param_untyped_handle& rhs) const;

    /// @copydoc cci_param_typed::equals
    bool equals(const cci_param_if& rhs) const;

    ///@}

    /// Indicates if the handled parameter is valid or not
    /**
     * @param check If set to true and the handle is invalid, the handle will
     *        try to revalidate itself using the current broker and the name
     *        of the original parameter before to return the result.
     *
     * @return false if handled parameter is invalid; otherwise, true
     */
    bool is_valid(bool check = false) const;

    /// Invalidate the parameter handle
    /**
     *        remove the parameter handle from the original
     *        parameter before invalidating. Otherwise, just invalidate.
     */
    void invalidate();

    /// Reset the parameter to the intial value
    void reset();

    /// Assigns parameter untyped handle a new value from another legacy
    /// parameter untyped handle
    /**
     * @param param_handle New parameter untyped handle to assign
     * @return reference to this object
     */
    cci_param_untyped_handle& operator=(const cci_param_untyped_handle& param_handle);

protected:
    ///@name Type-punned value operations
    ///@{

    /// @copydoc cci_param_typed::get_raw_value
    const void* get_raw_value() const;


    /// @copydoc cci_param_typed::get_raw_default_value
    const void* get_raw_default_value() const;

    /// @copydoc cci_param_typed::set_raw_value(const void*)
    void set_raw_value(const void* vp);

    /// @copydoc cci_param_typed::set_raw_value(const void*, const void*)
    void set_raw_value(const void* vp, const void* pwd);

    /// @copydoc cci_param_typed::get_type_info
    const std::type_info& get_type_info() const;

    ///@name Initialization and Destructions methods
    ///@{

    /// @copydoc cci_param_untyped::init
    void init();

    /// @copydoc cci_param_typed::destroy
    void destroy();

    ///@}

private:
    /// Originator of the parameter proxy.
    cci_originator m_originator;

    /// Original parameter
    mutable cci_param_if* m_orig_param;

    /// Check handled parameter is valid
    /**
     * In case the handled parameter is no more valid, it will report an error.
     * @param report_error Disable error report
     */
    void check_is_valid(bool report_error = true) const;

    /// Original parameter name
    const char* m_orig_param_name;
};

CCI_CLOSE_NAMESPACE_

#endif //CCI_BASE_PARAM_HANDLE_H_INCLUDED_
