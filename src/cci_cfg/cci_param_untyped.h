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
/**
 * @author Enrico Galli, Intel
 */
#ifndef CCI_CFG_CCI_PARAM_UNTYPED_H_INCLUDED_
#define CCI_CFG_CCI_PARAM_UNTYPED_H_INCLUDED_

#include <string>
#include <vector>

#include "cci_cfg/cci_config_macros.h"
#include "cci_cfg/cci_param_if.h"
#include "cci_cfg/cci_datatypes.h"
#include "cci_cfg/cci_value.h"
#include "cci_cfg/cci_originator.h"
#include <greencontrol/config.h>

//TODO: remove header dependency
#include <greencontrol/config.h>

CCI_OPEN_NAMESPACE_

class cci_broker_if;

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
     * @return Description
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

    /// @name Write callback handling
    /// @{

    /// Register an untyped write callback.
    /**
     * @param cb Untyped write callback
     * @param cci_untyped_tag Untyped tag to avoid compiler ambiguity
     *
     * @return Untyped callback handle
     */
    cci_callback_untyped_handle
    register_write_callback(const cci_param_write_callback_untyped& cb,
                            cci_untyped_tag = cci_untyped_tag());

    /// Register an untyped write callback with a method as callback
    /**
     * @param cb Untyped write callback method
     * @param obj Associated object instance pointer
     * @param cci_untyped_tag Untyped tag to avoid compiler ambiguity
     *
     * @return Untyped callback handle
     */
    template<typename C>
    cci_callback_untyped_handle
    register_write_callback(cci_param_write_callback_untyped::signature
                            (C::*cb), C* obj,
                            cci_untyped_tag = cci_untyped_tag())
    {
        return register_write_callback(sc_bind(cb, obj, sc_unnamed::_1));
    }

    /// @}

    /// @name Validate write callback handling
    /// @{

    /// Register an untyped validate write callback.
    /**
     * @param cb Untyped validate write callback
     * @param cci_untyped_tag Untyped tag to avoid compiler ambiguity
     *
     * @return Untyped callback handle
     */
    cci_callback_untyped_handle
    register_validate_write_callback(
            const cci_param_validate_write_callback_untyped& cb,
            cci_untyped_tag = cci_untyped_tag());

    /// Register an untyped validate write callback with a method as callback
    /**
     * @param cb Untyped validate write callback method
     * @param obj Associated object instance pointer
     * @param cci_untyped_tag Untyped tag to avoid compiler ambiguity
     *
     * @return Untyped callback handle
     */
    template<typename C>
    cci_callback_untyped_handle
    register_validate_write_callback(
            cci_param_validate_write_callback_untyped::signature(C::*cb),
            C* obj, cci_untyped_tag = cci_untyped_tag())
    {
        return register_validate_write_callback(
                sc_bind(cb, obj, sc_unnamed::_1));
    }

    /// @}

    /// @name Read callback handling
    /// @{

    /// Register an untyped read callback.
    /**
     * @param cb Untyped read callback
     * @param cci_untyped_tag Untyped tag to avoid compiler ambiguity
     *
     * @return Untyped callback handle
     */
    cci_callback_untyped_handle
    register_read_callback(const cci_param_read_callback_untyped& cb,
                           cci_untyped_tag = cci_untyped_tag());

    /// Register an untyped read callback with a method as callback
    /**
     * @param cb Untyped read callback method
     * @param obj Associated object instance pointer
     * @param cci_untyped_tag Untyped tag to avoid compiler ambiguity
     *
     * @return Untyped callback handle
     */
    template<typename C>
    cci_callback_untyped_handle
    register_read_callback(cci_param_read_callback_untyped::signature
                            (C::*cb), C* obj,
                            cci_untyped_tag = cci_untyped_tag())
    {
        return register_read_callback(sc_bind(cb, obj, sc_unnamed::_1));
    }

    /// @}

    /// @name Callback handling
    /// @{

    /// Unregister all callbacks
    /**
     * @return True if success, otherwise False
     */
    bool unregister_all_callbacks();

    /// Returns if the parameter has registered callbacks.
    /**
     * @return True if the parameter has registered callbacks, otherwise False
     */
    bool has_callbacks() const;

    /// @}

protected:
    /// @name Write callback handling implementation
    /// @{

    /// Register a write callback handle
    /**
     * @param cb Untyped write callback handle
     * @param cci_originator Originator
     *
     * @return Untyped callback handle
     */
    cci_callback_untyped_handle
    register_write_callback(const cci_callback_untyped_handle &cb,
                            const cci_originator &orig);

    /// Unregister a write callback handle
    /**
     * @param cb Untyped write callback handle
     * @param cci_originator Originator
     *
     * @return True if unregister is a success. Otherwise False.
     */
    bool
    unregister_write_callback(const cci_callback_untyped_handle &cb,
                              const cci_originator &orig);

    /// @}

    /// @name Validate write callback handling implementation
    /// @{

    /// Register a validate write callback handle
    /**
     * @param cb Untyped validate write callback handle
     * @param cci_originator Originator
     *
     * @return Untyped callback handle
     */
    cci_callback_untyped_handle
    register_validate_write_callback(const cci_callback_untyped_handle &cb,
                                     const cci_originator &orig);

    /// Unregister a validate write callback handle
    /**
     * @param cb Untyped validate write callback handle
     * @param cci_originator Originator
     *
     * @return True if unregister is a success. Otherwise False.
     */
    bool
    unregister_validate_write_callback(const cci_callback_untyped_handle &cb,
                                       const cci_originator &orig);

    /// @}

    /// @name Read callback handling implementation
    /// @{

    /// Register a read callback handle
    /**
     * @param cb Untyped read callback handle
     * @param cci_originator Originator
     *
     * @return Untyped callback handle
     */
    cci_callback_untyped_handle
    register_read_callback(const cci_callback_untyped_handle &cb,
                           const cci_originator &orig);

    /// Unregister a read callback handle
    /**
     * @param cb Untyped read callback handle
     * @param cci_originator Originator
     *
     * @return True if unregister is a success. Otherwise False.
     */
    bool
    unregister_read_callback(const cci_callback_untyped_handle &cb,
                             const cci_originator &orig);

    /// @}

    /// @name Callback handling implementation
    /// @{

    /// Unregister all callbacks (write, validate write and read callbacks).
    /**
     * @param cci_originator Originator
     *
     * @return True if unregister is a success. Otherwise False.
     */
    bool unregister_all_callbacks(const cci_originator &orig);

    ///@}

public:

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
    cci_param_untyped(bool is_top_level_name, cci_broker_if* broker_handle,
                   const std::string& desc, const cci_originator& originator);

    ///@name Initialization and Destructions methods
    ///@{

    /// Initialize.
    void init();

    ///@}

    /// Updates the internal member m_latest_write_access_originator_cp
    void update_latest_write_originator(const cci_originator& originator) const;

protected:

    /// Associated gs_param_base
    gs::gs_param_base *m_gs_param_base;

    /// Is default value
    bool m_is_default_value;

    /// Is invalid value
    bool m_is_invalid_value;

    /// Description
    std::string m_description;

    /// Metadata
    cci_value_map metadata;

    /// Initialized
    bool m_init_called;

    /// Broker handle
    cci_broker_if* m_broker_handle;

    /// Stores the originator of the latest successful write access (status within post_write) as an alternative to get originator information within the callback(s)
    mutable cci_originator m_latest_write_access_originator_cp;

    /// Stores if there is a valid m_latest_write_access_originator_cp (latest originator of the latest successful write access)
    mutable bool m_latest_write_access_originator_valid;

    /// Callback object
    template<class T>
    struct callback_obj {
        callback_obj(T cb, const cci_originator& orig):
                callback(cb), originator(orig) {}
        T callback;
        cci_originator originator;
    };

    typedef callback_obj<typename cci_callback_untyped_handle::type>
            write_callback_obj_t;

    /// Write callbacks
    std::vector<write_callback_obj_t> m_write_callbacks;

    typedef callback_obj<typename cci_callback_untyped_handle::type>
            validate_write_callback_obj_t;

    /// Validate write callbacks
    std::vector<validate_write_callback_obj_t> m_validate_write_callbacks;

    typedef callback_obj<typename cci_callback_untyped_handle::type>
            read_callback_obj_t;

    /// Read callbacks
    std::vector<read_callback_obj_t> m_read_callbacks;

    /// Originator of the parameter
    const cci_originator m_originator;
};

CCI_CLOSE_NAMESPACE_
  
#endif // CCI_CFG_CCI_PARAM_UNTYPED_H_INCLUDED_
