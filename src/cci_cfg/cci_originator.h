/*****************************************************************************
Copyright 2006-2015 Accellera Systems Initiative Inc.
All rights reserved.

Copyright 2009-2011 GreenSocs Ltd
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

#ifndef CCI_CCI_ORIGINATOR_H_INCLUDED_
#define CCI_CCI_ORIGINATOR_H_INCLUDED_

#include "cci_core/systemc.h"

CCI_OPEN_NAMESPACE_

/// Originator information when functions being called by the main object (which only the owner shall use)
extern const std::string PARAM_ORIGINATOR;

/// Originator class which is used to handle the originator information independently from its data type (object pointer or string)
/**
 * Can be used to set and handle originator information in a uniform way
 * independently from its data type.
 *
 * Use the appropriate setter (setting the object is preferred!).
 * Use the appropriate getter:
 * - Use get object to make use of value the sc_object brings but check for NULL needs to be done
 * - Use get string to get the originator name, in the case an object had been set, its name will be returned.
 *   No need to differentiate the between the underlying data type.
 *
 * Static setter function shall be used by the CCI implementation to update the originator.
 *
 * Static getter function can be used by the end user to get the current originator.
 *
 * Guidelines for CCI implementations:
 * - Use the call stack for stacking originator information.
 *   Use the returned old originator to back it up and re-apply afterwards.
 * - It shall be unique/unambiguously to identify an originator using its name!
 *
 */
class cci_originator {

public:

  /// (Static) global setter function for updating the current Golbal Originator Pointer
  /**
   * @param current_originator  Pointer to the current Golbal Originator Pointer, might be NULL if there is no originator information
   * @return The old originator (NULL if there was none before)
   */
  static const cci_originator* set_global_originator(const cci_originator* current_originator) {
    const cci_originator* old_originator = handle_current_originator();
    handle_current_originator(current_originator, true);
    return old_originator;
  }

  /// (Static) global getter function for getting the current Golbal Originator Pointer
  /**
   * @param The current Global Originator Pointer (might be NULL if there is none)
   */
  static const cci_originator* get_global_originator() {
    return handle_current_originator();
  }

protected:
  /// Internal function storing the originator
  /**
   * Can be used to get and set the originator:
   * @param new_originator Pointer to the new originator (optional), only applied if param set_new_originator is true.
   * @param set_new_originator If the param new_originator shall be stored as new originator (guarded by this bool to allow setting of NULL)
   */
  static const cci_originator* handle_current_originator(const cci_originator* new_originator = NULL, bool set_new_originator = false) {
    static const cci_originator* global_current_originator = NULL;
    if (set_new_originator) global_current_originator = new_originator;
    return global_current_originator;
  }

public:

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

  /// Set the originator using its object (preferred!)
  /**
   * @param originator  Originator object
   */
  //void set_originator_obj(sc_core::sc_object& originator) {
  //  m_originator_obj = &originator;
  //}

  /// Set the originator using its name (only use if no sc_object available!)
  /**
   * @param originator  Originator name
   */
  //void set_originator_name(const std::string& originator) {
  //  m_originator_obj = NULL;
  //  m_originator_str = originator;
  //}

  //void delete_originator() {
  //  m_originator_obj = NULL;
  //  m_originator_str = "";
  //}

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

/// Global originator RAII helper class
/**
* Helper class to handle setting the global originator
* 
*/
class cci_originator_lock
{
public:
	///Sets global originator to originator parameter
	explicit cci_originator_lock(const cci_originator& originator) :
		originator_backup(cci_originator::set_global_originator(&originator))
	{}

	///Reverts global originator back to original version
	~cci_originator_lock()
	{
		cci_originator::set_global_originator(originator_backup);
	}

private:

	///Disable coping of lock
	cci_originator_lock( const cci_originator_lock& );

	const cci_originator * originator_backup;
};

CCI_CLOSE_NAMESPACE_


#endif

