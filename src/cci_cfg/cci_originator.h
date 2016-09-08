/*****************************************************************************
  Copyright 2006-2015 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2009-2011 GreenSocs Ltd
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

#ifndef CCI_CCI_ORIGINATOR_H_INCLUDED_
#define CCI_CCI_ORIGINATOR_H_INCLUDED_

#include "cci_core/systemc.h"
#include "cci_cfg/cci_config_macros.h"

CCI_OPEN_NAMESPACE_

/// Originator class which is used to track owners, handles and value providers of parameters.
/**
 * Static setter function is used by the parameter (proxy) to identify originator
 * changing parameter's value.
 *
 * Static getter function is used by the parameter implementation to record
 * originator's identity.
 */
class cci_originator
{
public:
    /// Default Constructor assumes current module is the originator
    inline cci_originator()
            : m_originator_obj(current_originator_object()),
              m_originator_str(NULL) {}

    /// Constructor with an originator name
    /**
     * Constructor to associate explicitly a string name to an originator.
     * The provided name will be used as the default name in case the
     * originator is not build in the SystemC hierarchy.
     *
     * @param originator_name string name of the originator
     */
    cci_originator(const std::string& originator_name);

    /// Constructor with an originator (char *) name
    /**
     * This form (in addition to @see cci_originator(const std::string&))
     * is necessary to avoid ambiguity between the sc_object,
     * sc_module and std::string overloads for literal string constant
     * arguments.
     *
     * @param originator_name string name of the originator
     */
    cci_originator(const char *originator_name);

    /// Constructor with another originator whose content will be copied
    /**
     * @param originator originator whose content will be copied
     */
    cci_originator(const cci_originator& originator);

    /// Constructor with an sc_object originator
    inline cci_originator(const sc_core::sc_object& originator)
            : m_originator_obj(&originator), m_originator_str(NULL) {}

    /// Convenience constructor with an sc_module originator
    inline cci_originator(const sc_core::sc_module& originator):
        m_originator_obj(static_cast<const sc_core::sc_object *>(&originator)),
        m_originator_str(NULL) {}

    /// Destructor
    ~cci_originator();

    /// Returns a pointer to the current originator
    /**
     * Might return NULL if there is no current originator or the current originator
     * is only given by name (use get_originator_str() instead).
     *
     * @return Originator object pointer or NULL
     */
    const sc_core::sc_object* get_object() const;

    /// Returns the name of the current originator
    /**
     * Might return empty if there is no current originator.
     * Automatically uses the originator object name if the originator is given by object pointer.
     *
     * @return Originator name or NULL
     */
    const char* name() const;

    /// Returns the string name of the current originator
    /**
     * Might return empty if no explicit string name was provided by the user
     * during originator construction
     *
     * @return Originator user provided name or NULL
     */
    const char* string_name() const;

    //Assignment operator overload
    cci_originator& operator=( cci_originator originator );

protected:

    /// Return the current originator object pointer
    sc_core::sc_object* current_originator_object();

    /// Pointer to the current originator object (priority compared to name m_originator_str)
    const sc_core::sc_object* m_originator_obj;

    /// Name of the current originator
    std::string* m_originator_str;

};

CCI_CLOSE_NAMESPACE_


#endif

