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

#ifndef CCI_CCI_PARAM_IF_H_INCLUDED_
#define CCI_CCI_PARAM_IF_H_INCLUDED_

#include <typeinfo>

#include "cci_cfg/cci_param_callbacks.h"
#include "cci_cfg/cci_mutable_types.h"

/**
 * @author Enrico Galli, Intel
 * @author Philipp A. Hartmann, Intel
 */

CCI_OPEN_NAMESPACE_

class cci_param_untyped_handle;
class cci_originator;

/// Interfaces for cci_param_typed implementation
/**
* Features:
* - Name of the parameter,
* - stores value untyped as string representation
* - allows generic typed and list access
* - CCI value functions
* - Callback handling
*/

class cci_param_if : public cci_param_callback_if
{

    friend class cci_param_untyped_handle;

public:

    /// Destructor.
    virtual ~cci_param_if() {}

    ///@name CCI value Data Type and access
    ///@{

    /// @copydoc cci_param_untyped_handle::set_cci_value
    virtual void set_cci_value(const cci_value &val) = 0;

    /// @copydoc cci_param_typed::get_cci_value
    virtual cci_value get_cci_value() const = 0;

    /// @copydoc cci_param_typed::get_mutable_type
    virtual cci_param_mutable_type get_mutable_type() const = 0;

    ///@}


    ///@name Description
    ///@{

    /// @copydoc cci_param_untyped::get_description
    virtual std::string get_description() const = 0;

    /// @copydoc cci_param_untyped::set_description
    virtual void set_description(const std::string &desc) = 0;

    ///@}

    ///@name Metadata
    ///@{

    /// @copydoc cci_param_untyped::add_metadata
    virtual void add_metadata(const std::string &name, const cci_value &value,
                              const std::string &desc = "") = 0;

    /// @copydoc cci_param_untyped::get_metadata
    virtual cci_value_map get_metadata() const = 0;

    ///@}


    ///@}

    ///@name Parameter Value Status
    ///@{

    /// @copydoc cci_param_untyped::is_default_value
    virtual bool is_default_value() = 0;

    /// @copydoc cci_param_untyped::is_initial_value
    virtual bool is_initial_value() const = 0;

    ///@}

    ///@name Miscellaneous
    ///@{

    /// @copydoc cci_param_untyped::get_latest_write_originator
    virtual const cci_originator *get_latest_write_originator() const = 0;

    ///@}

    ///@name Write-access control
    ///@{

    /// @copydoc cci_param_untyped::lock
    virtual bool lock(void *pwd = NULL) = 0;

    /// @copydoc cci_param_untyped::unlock
    virtual bool unlock(void *pwd = NULL) = 0;

    /// @copydoc cci_param_untyped::is_locked
    virtual bool is_locked() const = 0;

    ///@}

    ///@name Query parameter type and name
    ///@{

    /// @copydoc cci_param_typed::get_basic_type
    virtual cci_data_type get_basic_type() const = 0;

    /// @copydoc cci_param_untyped::get_name
    virtual const std::string &get_name() const = 0;

    ///@}

    ///@name Type-punned value operations
    ///@{

    /// Computer if the stored values are equal
    /**
    * @param rhs reference to another cci_param_if implementation
    * @return True if both values are equal
    */
    virtual bool equals(const cci_param_if &rhs) const = 0;

    /// Computer if the stored values are equal
    /**
    * @param rhs reference to another cci_param_untyped_handle implementation
    * @return True if both values are equal
    */
    virtual bool equals(const cci_param_untyped_handle &rhs) const = 0;

    ///@}

    /// @copydoc cci_param_typed::get_type_info
    virtual const std::type_info& get_type_info() const = 0;

    ///@name Initialization and Destructions methods
    ///@{

    /// Initialize.
    virtual void init() = 0;

    /// @copydoc cci_param_typed::destroy
    virtual void destroy() = 0;

    ///@}

    /// @copydoc cci_param_typed::create_param_handle
    virtual cci_param_untyped_handle
    create_param_handle(const cci_originator &originator) = 0;

private:
    /// @copydoc cci_param_typed::set_cci_value(const cci_value&, const cci_originator&)
    virtual void set_cci_value(const cci_value &val,
                               const cci_originator &originator) = 0;

    /// @copydoc cci_param_typed::set_raw_value(const void*, const cci_originator&)
    virtual void set_raw_value(const void *vp, const cci_originator &originator) = 0;

    /// @copydoc cci_param_typed::set_raw_value(const void*, const void*, const cci_originator&)
    virtual void set_raw_value(const void *vp, const void *pwd,
                               const cci_originator &originator) = 0;

    /// @copydoc cci_param_typed::get_raw_value
    virtual const void *get_raw_value() const = 0;

    /// @copydoc cci_param_typed::get_default_value_raw
    virtual const void *get_default_value_raw() const = 0;

    /// @copydoc cci_param_untyped::add_param_handle
    virtual void add_param_handle(cci_param_untyped_handle* param_handle) = 0;

    /// @copydoc cci_param_untyped::remove_param_handle
    virtual void remove_param_handle(cci_param_untyped_handle* param_handle) = 0;
};

CCI_CLOSE_NAMESPACE_

#endif
