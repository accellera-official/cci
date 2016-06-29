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

#ifndef CCI_CALLABACK_H_INCLUDED_
#define CCI_CALLABACK_H_INCLUDED_

#include "cci_cfg/cci_config_macros.h"


CCI_OPEN_NAMESPACE_

/**
 Wrapper class for callback functions
 Class uses partial-template specialization to support better callback signatures
 @tparam T Type of callback function to be wrapped
 */
template<class T>
struct cci_callback;

#define CCI_CALLBACK_BLOCK                                                     \
	/**
	 Constructor - Creates new callback wrapper from callback
	 @tparam C Callback type
	 @param callback Reference to callback to be wrapped
	 */                                                                        \
	template<class C>			                                               \
	cci_callback(const C& callback) : m_impl(new callback_impl<C>(callback))   \
	{}                                                                         \
	/**
	 Copy Constructor - Creates copy of callback wrapper
	 @param copy Callback wrapper to copy
	 */                                                                        \
	cci_callback(const cci_callback& copy)                                     \
		: m_impl(copy.m_impl ? copy.m_impl->clone() : 0)                       \
	{}                                                                         \
	/**
	 Destructor
	 */                                                                        \
	~cci_callback()                                                            \
	{                                                                          \
		if (m_impl)                                                            \
		{                                                                      \
			delete m_impl;                                                     \
			m_impl = 0;                                                        \
		}                                                                      \
	}                                                                          \
	/**
	 Assignment operator - Creates a new wrapper from an existing wrapper
	 @param copy Wrapper to copy
	 @returns Reference to current wrapper
	 */                                                                        \
	cci_callback& operator=(const cci_callback& copy)                          \
	{                                                                          \
		if (this == &copy)                                                     \
			return *this;                                                      \
		                                                                       \
		delete m_impl;                                                         \
		m_impl = 0;                                                            \
		if (copy.m_impl)                                                       \
			m_impl = copy.m_impl->clone();                                     \
		                                                                       \
		return *this;                                                          \
	}


//Only add move semantics if supported by compiler
#ifdef CCI_RVALUE_REFERENCES_SUPPORTED
#define CCI_CALLBACK_MOVE_BLOCK                                   \
	/**
	 Move constructor - Moves wrapper implementation from other into current wrapper
	 @param other Callback wrapper to move implementation from
	 */                                                           \
	cci_callback(cci_callback && other) : m_impl(other.m_impl)    \
	{                                                             \
			other.m_impl = 0;                                     \
	}                                                             \
	                                                              \
	/**
	 Move assignment - Moves wrapper implementation from other into current wrapper
	 @param other Callback wrapper to move implementation from
	 @returns Reference to current wrapper
	 */                                                           \
	cci_callback & operator=(cci_callback && other)               \
	{                                                             \
		if (this == &other)                                       \
			return &this;                                         \
		                                                          \
		if (m_impl)                                               \
			delete m_impl;                                        \
		                                                          \
		m_impl = other.m_impl;                                    \
		                                                          \
	}

#else
#define CCI_CALLBACK_MOVE_BLOCK
#endif

#define  CCI_CALLBACK_IMPL_IF_BLOCK          \
	/**
	Creates a new copy of the wrapper
	@returns Pointer to clone of this object
	*/                                       \
	virtual callback_impl_if* clone() = 0; \
	virtual ~callback_impl_if(){}

#define  CCI_CALLBACK_IMPL_BLOCK                              \
	/**
	Constructor - Creates a callback wrapper implementation from give callback
	@param callback Callback to wrap
	*/                                                        \
	callback_impl(const C& callback) : m_callback(callback)   \
	{}                                                        \
	                                                          \
	/**
	Creates a new copy of the wrapper
	@returns Pointer to clone of this object
	*/                                                        \
	virtual callback_impl_if * clone()                        \
	{                                                         \
		return new callback_impl(m_callback);                 \
	}


/**
 Wrapper class for callback take 1 argument
 @tparam TRet Type of callback return value
 @tparam TFirst Type of first callback argument
 */
template<class TRet, class TFirst>
class cci_callback < TRet(TFirst) >
{
public:
	CCI_CALLBACK_BLOCK
	CCI_CALLBACK_MOVE_BLOCK

	/**
	 Operator() - Invokes wrapped callback 
	 @param first_value First argument to pass to wrapped callback
	 @return Value returned by wrapped callback
	 */
	TRet operator()(TFirst first_value)
	{
		return (*m_impl)(first_value);
	}

private:

	/**
	 *	Base class of all wrapper implementation for a given function prototype
	 */
	struct callback_impl_if
	{
		CCI_CALLBACK_IMPL_IF_BLOCK

		/**
		Operator() - Invokes wrapped callback
		@param first_value First argument to pass to wrapped callback
		@return Value returned by wrapped callback
		*/
		virtual TRet operator()(TFirst first_value) = 0;
	};

	/**
	 Implementation of callback wrapper
	 @tparam C Type of callback
	 */
	template<class C>
	struct callback_impl : callback_impl_if
	{
		CCI_CALLBACK_IMPL_BLOCK

		/**
		Operator() - Invokes wrapped callback
		@param first_value First argument to pass to wrapped callback
		@param second_value Second argument to pass to wrapped callback
		@return Value returned by wrapped callback
		*/
		virtual TRet operator()(TFirst first_value)
		{
			return m_callback(first_value);
		}

		///Stored callback
		C m_callback;
	};

	///Pointer to callback wrapper implementation
	callback_impl_if * m_impl;
};


/**
 Wrapper class for callback take 2 arguments
 @tparam TRet Type of callback return value
 @tparam TFirst Type of first callback argument
 @tparam TSecond Type of second callback argument
 */
template<class TRet, class TFirst, class TSecond>
class cci_callback < TRet(TFirst, TSecond) >
{
public:
	CCI_CALLBACK_BLOCK
	CCI_CALLBACK_MOVE_BLOCK

	/**
	 Operator() - Invokes wrapped callback 
	 @param first_value First argument to pass to wrapped callback
	 @param second_value Second argument to pass to wrapped callback
	 @return Value returned by wrapped callback
	 */
	TRet operator()(TFirst first_value,TSecond second_value)
	{
		return (*m_impl)(first_value, second_value);
	}

private:

	/**
	 *	Base class of all wrapper implementation for a given function prototype
	 */
	struct callback_impl_if
	{
		CCI_CALLBACK_IMPL_IF_BLOCK

		/**
		Operator() - Invokes wrapped callback
		@param first_value First argument to pass to wrapped callback
		@param second_value Second argument to pass to wrapped callback
		@return Value returned by wrapped callback
		*/
		virtual TRet operator()(TFirst first_value,TSecond second_value) = 0;
	};

	/**
	 Implementation of callback wrapper
	 @tparam C Type of callback
	 */
	template<class C>
	struct callback_impl : callback_impl_if
	{
		CCI_CALLBACK_IMPL_BLOCK

		/**
		Operator() - Invokes wrapped callback
		@param first_value First argument to pass to wrapped callback
		@param second_value Second argument to pass to wrapped callback
		@return Value returned by wrapped callback
		*/
		virtual TRet operator()(TFirst first_value,TSecond second_value)
		{
			return m_callback(first_value, second_value);
		}

		///Stored callback
		C m_callback;
	};

	///Pointer to callback wrapper implementation
	callback_impl_if * m_impl;
};

/**
Wrapper class for callback take 3 arguments
@tparam TRet Type of callback return value
@tparam TFirst Type of first callback argument
@tparam TSecond Type of second callback argument
@tparam TThird Type of third callback argument
*/
template<class TRet, class TFirst, class TSecond, class TThird>
class cci_callback < TRet(TFirst, TSecond, TThird) >
{
public:

	CCI_CALLBACK_BLOCK
	CCI_CALLBACK_MOVE_BLOCK

	/**
	Operator() - Invokes wrapped callback
	@param first_value First argument to pass to wrapped callback
	@param second_value Second argument to pass to wrapped callback
	@param third_value Third argument to pass to wrapped callback
	@return Value returned by wrapped callback
	*/
	TRet operator()(TFirst first_value,TSecond second_value, TThird third_value)
	{
		return (*m_impl)(first_value, second_value, third_value);
	}

private:

	/**
	*	Base class of all wrapper implementation for a given function prototype
	*/
	struct callback_impl_if
	{
		CCI_CALLBACK_IMPL_IF_BLOCK

		/**
		Operator() - Invokes wrapped callback
		@param first_value First argument to pass to wrapped callback
		@param second_value Second argument to pass to wrapped callback
		@param third_value Third argument to pass to wrapped callback
		@return Value returned by wrapped callback
		*/
		virtual TRet operator()(TFirst first_value,TSecond second_value, TThird third_value) = 0;
	};

	/**
	Implementation of callback wrapper
	@tparam C Type of callback
	*/
	template<class C>
	struct callback_impl : callback_impl_if
	{
		CCI_CALLBACK_IMPL_BLOCK

		/**
		Operator() - Invokes wrapped callback
		@param first_value First argument to pass to wrapped callback
		@param second_value Second argument to pass to wrapped callback
		@param third_value Third argument to pass to wrapped callback
		@return Value returned by wrapped callback
		*/
		virtual TRet operator()(TFirst first_value,TSecond second_value, TThird third_value)
		{
			return m_callback(first_value, second_value, third_value);
		}

		///Stored callback
		C m_callback;
	};

	///Pointer to callback wrapper implementation
	callback_impl_if * m_impl;
};

#undef CCI_CALLBACK_IMPL_BLOCK
#undef CCI_CALLBACK_IMPL_IF_BLOCK
#undef CCI_CALLBACK_MOVE_BLOCK
#undef CCI_CALLBACK_BLOCK

CCI_CLOSE_NAMESPACE_

#endif //CCI_CALLABACK_H_INCLUDED_
