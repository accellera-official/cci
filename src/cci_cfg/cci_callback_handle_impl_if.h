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

#ifndef CCI_CALLABACK_HANDLE_IMPL_IF_H_INCLUDED_
#define CCI_CALLABACK_HANDLE_IMPL_IF_H_INCLUDED_

#include "cci_cfg/cci_config_macros.h"

CCI_OPEN_NAMESPACE_

///Abstract base class of callback registration handle implementation
class cci_callback_handle_impl_if
{
public:
	///Virtual destructor
	virtual ~cci_callback_handle_impl_if() {}

	///Creates of copy of current object
	virtual cci_callback_handle_impl_if * clone() const = 0;

	///Unregister callback from cci_param. Multiple calls to this method will be ignored.
	virtual void unregister() = 0;
};

CCI_CLOSE_NAMESPACE_

#endif //CCI_CALLABACK_HANDLE_IMPL_IF_H_INCLUDED_

