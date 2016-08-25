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
#include "cci_report_handler.h"

CCI_OPEN_NAMESPACE_

/// Originator class which is used to track owners, handles and value providers of parameters.
/**
 *
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
  cci_originator() 
  : m_originator_obj(sc_core::sc_get_curr_simcontext()->hierarchy_curr()) {
	/**
     * @todo Scrutinize how much of this code belongs in the standard (vs. implementation)
	 * @todo Need standard way to get current module; 
	 *       hierarchy_curr() is not standard.
	 * @todo Check boundary conditions:
	 *       - prior to initialization of hierarchy_curr
	 */
	if (!m_originator_obj) {
	  // If unable to determine current module from module stack, try via current process
	  sc_core::sc_process_handle h = sc_core::sc_get_current_process_handle();
	  while (true) { // loop walking up the process chain
		  sc_core::sc_object *p_parent = h.get_parent_object();
		  if (!p_parent) // process is invalid
			  break;
		  if (sc_core::sc_module *p_parent_module = dynamic_cast<sc_core::sc_module *>(p_parent)) {
			  m_originator_obj = p_parent_module;
			  break;
		  }
		  if (sc_core::sc_process_b *p_parent_process = dynamic_cast<sc_core::sc_process_b *>(p_parent))
			  // Parent is a process, evaluate its parent.
			  h = sc_core::sc_process_handle(p_parent_process);
	  }
	}
	if (m_originator_obj) {
		m_originator_str = m_originator_obj->name();
	}
	else {
		// Caller needs to catch this exception & add identifying parameter info.
        CCI_REPORT_ERROR("cci_originator/cci_originator", "Unable to determine parameter's owner.");
	}
  }

  /// Constructor with another originator whose content will be copied
  cci_originator(const cci_originator& originator)
  : m_originator_obj(originator.m_originator_obj)
  , m_originator_str(originator.m_originator_str) {
  }

  /// Constructor with an sc_object originator
  cci_originator(const sc_core::sc_object& originator)
  : m_originator_obj(&originator)
  , m_originator_str() {
  }

  /// Convenience constructor with an sc_module originator
  cci_originator(const sc_core::sc_module& originator)
  : m_originator_obj(static_cast<const sc_core::sc_object*>(&originator))
  , m_originator_str() {
  }

  /// Constructor with an originator string name
  cci_originator(const std::string& originator_name)
  : m_originator_obj(NULL)
  , m_originator_str(originator_name) {
  }

  /// Constructor with an originator (char *) name
  /**
   * This form (in addition to std::string) is necessary to avoid ambiguity
   * between the sc_object, sc_module and std::string overloads for literal
   * string constant arguments.
   */
  cci_originator(const char *originator_name)
  : m_originator_obj(NULL)
  , m_originator_str(originator_name) {
  }

  /// Returns a pointer to the current originator
  /**
   * Might return NULL if there is no current originator or the current originator
   * is only given by name (use get_originator_str() instead).
   *
   * @return Originator object pointer or NULL
   */
  const sc_core::sc_object* get() const {
    return m_originator_obj;
  }

  /// Returns the name of the current originator
  /**
   * Might return empty if there is no current originator.
   * Automatically uses the originator object name if the originator is given by object pointer.
   *
   * @return Originator name or NULL
   */
  const char* name() const {
    if (m_originator_obj) return m_originator_obj->name();
    return m_originator_str.c_str();
  }

  //Assignment operator overload
  cci_originator& operator=( const cci_originator &originator )
  {
   this->m_originator_obj = originator.m_originator_obj;
   this->m_originator_str = originator.m_originator_str;
   return(*this);
  }

protected:

  /// Pointer to the current originator object (priority compared to name m_originator_str)
  const sc_core::sc_object* m_originator_obj;

  /// Name of the current originator (no relevance if m_originator_obj not NULL)
  std::string m_originator_str;

};

CCI_CLOSE_NAMESPACE_


#endif

