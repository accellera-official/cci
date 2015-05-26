/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2006-2016 Intel Corporation
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

#ifndef CCI_CALLABACK_HANDLE_H_INCLUDED_
#define CCI_CALLABACK_HANDLE_H_INCLUDED_

#include "cci_cfg/cci_config_macros.h"

CCI_OPEN_NAMESPACE_

class cci_callback_handle_impl_if;

///Handle to callback registration
class cci_callback_handle
{
public:
	///Constructor - creates new handle from implementation.
	/**
	* @param impl Pointer to callback registration. This pointer will get deleted when cci_callback_handle gets destructed.
	*/
	explicit cci_callback_handle(cci_callback_handle_impl_if* impl);
	///Copy Constructor - creates new handle from existing handle
	/**
	* @param copy Handle to be copied
	*/
	cci_callback_handle(const cci_callback_handle& copy);
#ifdef CCI_RVALUE_REFERENCES_SUPPORTED
	///Move Constructor - Moves implementation from another handle to current handle
	/**
	* @param other Handle to move implementation from
	*/
	cci_callback_handle(cci_callback_handle&& other);
#endif
	///Destructor - deletes pointer to implementation
	~cci_callback_handle();

	///Assignment operator - creates new handle from existing handle
	/**
	* @param copy Handle to be copied
	* @return Reference to this handle
	*/
	cci_callback_handle& operator=(const cci_callback_handle& copy);

#ifdef CCI_RVALUE_REFERENCES_SUPPORTED
	/// Move assignment operator - Moves implementation from another handle to current handle and deletes the current implementation
	/**
	* @param other Handle to move implementation from
	* @return Reference to this handle
	*/
	cci_callback_handle& operator=(cci_callback_handle&& other);
#endif

	///Unregister callback from cci_param. Multiple calls to this method will be ignored.
	void unregister();

private:

	///Pointer to implementation
	cci_callback_handle_impl_if* m_impl;
};

CCI_CLOSE_NAMESPACE_

#endif // CCI_CALLABACK_HANDLE_H_INCLUDED_
