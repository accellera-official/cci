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

#include "cci_cfg/cci_callback_handle.h"
#include "cci_cfg/cci_callback_handle_impl_if.h"

CCI_OPEN_NAMESPACE_

cci_callback_handle::cci_callback_handle(cci_callback_handle_impl_if* impl) : m_impl(impl)
{}

cci_callback_handle::cci_callback_handle(const cci_callback_handle& copy) : m_impl(copy.m_impl->clone())
{}

#ifdef CCI_RVALUE_REFERENCES_SUPPORTED
cci_callback_handle::cci_callback_handle(cci_callback_handle&& other) : m_impl(other.m_impl)
{
	other.m_impl = nullptr;
}
#endif


cci_callback_handle::~cci_callback_handle()
{
	if (m_impl)
		delete m_impl;
}

cci_callback_handle & cci_callback_handle::operator=(const cci_callback_handle& copy)
{
	if (this == &copy)
		return *this;

	delete m_impl;
	m_impl = copy.m_impl->clone();

	return *this;
}

#ifdef CCI_RVALUE_REFERENCES_SUPPORTED

cci_callback_handle & cci_callback_handle::operator=(cci_callback_handle&& other)
{
	if (this == &other)
		return *this;

	delete m_impl;
	m_impl = other.m_impl;
	other.m_impl = nullptr;

	return *this;
}

#endif

void cci_callback_handle::unregister()
{
	if (m_impl)
		m_impl->unregister();
}

CCI_CLOSE_NAMESPACE_
