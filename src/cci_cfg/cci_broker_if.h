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

#ifndef CCI_CCI_BROKER_IF_H_INCLUDED_
#define CCI_CCI_BROKER_IF_H_INCLUDED_

#include "cci_cfg/cci_broker_callbacks.h"
#include "cci_cfg/cci_param_untyped_handle.h"
#include "cci_cfg/cci_value.h"
#include "cci_cfg/cci_param_filter_iterator.h"

CCI_OPEN_NAMESPACE_

// forward declaration
class cci_param_if;

// forward declaration
template<class T>
class cci_param_typed_handle;

// forward declaration for friend class
class cci_broker_manager;

/// CCI configuration broker interface.
/**
 * This can be used by a tool to access the database or parameter objects, set initial values etc.
 * or can be used by the model itself to get access to configuration objects etc.
 *
 * This always returns not the owner's parameter objects but parameter handle wrappers.
 */
class cci_broker_if : public cci_broker_callback_if {
protected:
    friend class cci_broker_manager;

    /// Creates or returns existing responsible handle for this broker for the given originator
    /**
     * This is called by the broker manager when the static search functions shall return a broker handle.
     * It is possible (but "senseless") to call this as a user. Returned instances shall be deleted together
     * with the original broker.
     *
     * It is ok to nest calls with the same originator.
     *
     * @param originator   Originator for which the broker handle shall be returned
     * @return Broker handle
     */
    virtual cci_broker_if &
    create_broker_handle(const cci_originator &originator) = 0;

    /// If this is a handle, returns the originator this broker handle is responsible for, otherwise returns NULL
    /**
     * @return Originator pointer in the case this is a handle; NULL if this is a raw broker
     */
    virtual const cci_originator *get_originator() const = 0;

public:

    /// Destructor
    virtual ~cci_broker_if() {};

    /// Name of this broker
    /**
     * Shall be a system-wide unique broker name.
     * Broker handles shall return their underlying broker's name.
     * Accessors can be distinguished using their originator information.
     *
     * @return broker name
     */
    virtual const std::string &name() const = 0;

    // //////////////////////////////////////////////////////////////////// //
    // ///////////   Access Parameters and Values   /////////////////////// //

    /// Set a parameter's init value.
    /**
     * The init value has priority to the default value being set by the owner!
     * If called for an explicit parameter this is a usual set, including not
     * setting the is_initial_value state to true!
     *
     * @exception        cci::cci_report::set_param_failed Setting parameter object failed
     * @param parname    Full hierarchical parameter name.
     * @param cci_value  cci::cci_value representation of the init value the parameter has to be set to.
     */
    virtual void set_initial_cci_value(const std::string &parname,
                                       const cci::cci_value &cci_value) = 0;

    /// Get a parameter's init value.
    /**
     *
     * @param parname    Full hierarchical parameter name.
     * @return           CCI value of the parameter's initial value. Empty value is returned when parameter is not existing or its initial value is not existing
     */
    virtual const cci::cci_value
    get_initial_cci_value(const std::string &parname) = 0;

    /// Returns the originator of the latest write access for the given parameter, independently if it is an implicit or explicit parameter, otherwise returns NULL
    /**
     * For explicit parameters it is recommended to use the parameter object's get_latest_write_originator() function.
     * Returns NULL if there is no parameter with the given name
     * or there is no originator.
     * If there is an explicit parameter and its originator is not NULL
     * this returns the explicit parameter's originator information!
     *
     * @param parname  Name of an implicit or explicit parameter.
     * @return Originator pointer; NULL if there was not yet any write
     */
    virtual cci_originator
    get_latest_write_originator(const std::string &parname) const = 0;

    /// Lock a parameter's init value.
    /**
     * Lock so that this parameter's init value cannot be overwritten by
     * any subsequent setInitValue call. This allows to emulate a hierarchical
     * precendence since a top-level module can prevent the childs from setting
     * init values by locking the init value before creating the subsystem.
     *
     * Throws (and does not lock) if no initial value is existing
     * that can be locked or if an initial value is already locked or if the
     * parameter is already existing as object (explicit parameter).
     *
     * @exception     cci::cci_report::set_param_failed Locking parameter object failed
     * @param parname Hierarchical parameter name.
     */
    virtual void lock_initial_value(const std::string &parname) = 0;

    /// Get a parameter's value (CCI value representation). Independent of the implicit or explicit status.
    /**
     * This accesses the parameter's NVP and works
     * for implicit and explicit parameters.
     *
     * See cci_broker_if::get_cci_value_keep_unused to do the same without impacting the used status.
     *
     * @param parname  Full hierarchical name of the parameter whose value should be returned.
     * @return  CCI value of the parameter
     */
    virtual const cci::cci_value get_cci_value(const std::string &parname) = 0;

    /// Get a parameter's value (like cci_broker_if::get_cci_value), but not impacting the used status
    /**
     * This is to be used only by tools, e.g. functional coverage tools.
     *
     * @see get_cci_value
     * @param parname  Full hierarchical name of the parameter whose value should be returned.
     * @return  CCI value of the parameter
     */
    virtual const cci::cci_value
    get_cci_value_keep_unused(const std::string &parname) const = 0;

    /// Get a parameter handle pointer. (TODO: maybe drop this because of Many-to-one Mapping, this returns only one (which one?))
    /**
     * This returns not the owner's parameter object but a handle.
     *
     * @param   parname   Full hierarchical parameter name.
     * @return  Pointer to the parameter handle object (NULL if not existing).
     *          @todo return a vector/iterator over param objects, because there might be more than one
     */
    virtual cci_param_untyped_handle
    get_param_handle(const std::string &parname) = 0;

    /// Return a list of all parameters (implicit and explicit) for the given scope (matching the pattern)
    /**
     * TODO:
     * e.g.
     * - parameters of the specified module, pattern = module_name, e.g. "mymod.mysubmod";
     * - pattern = module_name+'.*' to include the parameters of the childs, e.g. "mymod.mysubmod.*"
     * - pattern = '*'.param_name to get all parameters with the name, e.g. "*.cache_size"
     * - pattern = '*'.hierarchical.param_name to get all parameters with the name, e.g. "*.mysubmod.cache_size"
     *
     * @param pattern Specifies the parameters to be returned.
     * @return Vector with full hierarchical parameter names.
     */
    virtual const std::vector <std::string>
    get_param_list(const std::string &pattern) = 0;

    /// Checks whether a parameter exists (implicit or explicit).
    /**
     * @param parname  Full hierarchical parameter name.
     * @return Whether the parameter < parname > exists in the registry.
     */
    virtual bool param_exists(const std::string &parname) = 0;

    /// Returns if the parameter has ever been used.
    /**
     * A parameter has been used if there is/was either a parameter object
     * or handle mapped to the NVP, or the NVP's value has ever been read
     * (from the broker).
     *
     * @param parname  Full hierarchical parameter name.
     * @return If the parameter is or has been used.
     */
    virtual bool is_used(const std::string &parname) = 0;

    // //////////////////////////////////////////////////////////////////// //
    // ////////////////   Get Parameter List   //////////////////////////// //

    /// Returns a list of all parameters existing (implicit and explicit) in the registry.
    /**
     * @return Vector with full hierarchical parameter names.
     */
    virtual const std::vector <std::string> get_param_list() = 0;

    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Registry Functions   ///////////////////////////// //

    /// Adds a parameter to the registry.
    /**
     * Note: addPar (and all related methods) must not call any of the
     *       pure virtual functions in cci_param_untyped_handle because this method
     *       may be called by the cci_param_untyped_handle constructor.
     *
     * Note: This function shall never been called for any parameter handle
     *       objects but only for "real" parameter objects.
     *
     * @exception cci::cci_report::add_param_failed Adding parameter object failed
     * @param par Parameter (including name and value).
     */
    virtual void add_param(cci_param_if *par) = 0;

    /// Removes a parameter from the registry. May only be called by the parameter destructor, must not be called by anone else.
    /**
     * It should be ensured this is not being called from elsewhere than the parameter destructor (e.g. by user).
     *
     * Note: This function shall never been called for any parameter handle
     *       objects but only for "real" parameter objects.
     *
     * @exception cci::cci_report::remove_param_failed Remove parameter object failed
     * @param par Parameter pointer.
     */
    virtual void remove_param(cci_param_if *par) = 0;

public:
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////    Optional functions   //////////////////////////// //
    // TODO: Optional Config broker functions to be discussed

    /// Return a list of all (explicit) parameter handles in the given scope (matching the pattern)
    /**
     * pattern @see get_param_list
     * + (in the case the Many-to-one Mapping should work):
     *   pattern = full_param_name to get all param objects/handles (PH/PO) being mapped to the NVP
     * @todo use iterator instead of vector?
     *
     * This returns not the owner's parameter objects but handles.
     *
     * @param pattern Specifies the parameters to be returned.
     * @return Vector with parameter handles.
     */
    virtual const std::vector <cci_param_untyped_handle>
    get_param_handles(const std::string &pattern = "") = 0;

    /// Search parameters with a user-defined predicate callback
    /**
     * @param pred Callback to filter parameters
     * @return cci_iterable Iterable parameters
     */
    virtual cci_param_filter_iterator
    get_param_handles(cci_param_predicate& pred) = 0;

    /// Convenience function to get a typed parameter handle.
    /**
     * @param   parname   Full hierarchical parameter name.
     * @return  Parameter handle (invalid if not existing or the type is not correct)
     */
    template<class T>
    cci_param_typed_handle<T> get_cci_param_handle(const std::string &parname) {
        return cci_param_typed_handle<T>(get_param_handle(parname));
    }

    ///If this broker is a private broker (or handle)
    /**
     * @return If this broker is a private broker
     */
    virtual bool is_private_broker() const = 0;

};

/// Creates or returns the one non-private global broker provided by
/// the broker implementation. Called by the header function
/// @see cci_broker_manager::get_broker, NEVER call this as a user!
/**
 * This returns the raw global broker, not a handle, thus this
 * shall not be returned directly to the user!
 *
 * @return The one non-private global broker (not wrapped with a handle)
 */
cci_broker_if &cci_get_global_broker();

CCI_CLOSE_NAMESPACE_

#endif
