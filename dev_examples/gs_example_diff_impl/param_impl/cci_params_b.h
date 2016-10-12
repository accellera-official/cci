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

/**
 * @file   cci_params_b.h
 * @author Christian Schroeder, GreenSocs
 * @author Mark Burton, GreenSocs
 */


#ifndef __CCI_PARAMS_B_H__
#define __CCI_PARAMS_B_H__

//
// This extra library ONLY provides the one addtional data type and NOT the cci_value
//

//#define GS_cci_VERBOSE

#ifdef GS_cci_VERBOSE
  #define GS_cci_DUMP(msg) { std::cout<<msg<<std::endl; } 
#else
  #define GS_cci_DUMP(msg) {  } 
#endif


/// namespace macro
#define __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__ different_gs_example_param_implementation 
/// open namespace macro
#define __OPEN_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__ namespace __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__ { 
/// close namespace macro
#define __CLOSE_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__ }

#include "cci_configuration"
#include "greencontrol/config.h"

#ifndef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#define __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#endif
#include "gs_cci_base_param.h"
#ifndef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#define __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#endif
#include "gs_cci_param_t.h"
#ifndef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#define __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#endif
#include "gs_cci_param.h"
//#ifndef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
//#define __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
//#endif
//#include "gs_cci_value.h"
//#undef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__

namespace cci {
  
	template<class T, param_mutable_type TM>
	class other_cci_param : public __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__::gs_cci_param<T, TM>
	{
	public:

		typedef __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__::gs_cci_param<T, TM> base_type;

		other_cci_param(cci_param<typename base_type::value_type, TM> &owner_par, const std::string &nam, const T& val, const bool is_top_level_name, cci_broker_if* broker_accessor)
			: base_type(owner_par, nam, val, is_top_level_name, broker_accessor)
		{
		}

		other_cci_param(cci_param<typename base_type::value_type, TM> &owner_par, const std::string &nam, const cci_value& val, const bool is_top_level_name, cci_broker_if* broker_accessor)
			: base_type(owner_par, nam, val, is_top_level_name, broker_accessor)
		{
		}

		virtual void destroy()
		{
			std::cout << "TYPE B delete param" << std::endl;
			base_type::destroy();
		}
	};

  /// Implementation of parameter factory create function function declared in cci_config.h
  template<typename T, param_mutable_type TM>
  cci_param_impl_if* create_cci_param(cci_param<T, TM> *owner_par, const std::string &nam, const T& val, const bool is_top_level_name, cci_broker_if* broker_accessor, const std::string& desc)
  {
	cci_param_impl_if* impl_par = new other_cci_param<T, TM>(*owner_par, nam, val, is_top_level_name, broker_accessor);
    std::cout << "TYPE B new impl_par" << std::endl;
    return impl_par;
  }


  

  /// Implementation of parameter factory create function function declared in cci_config.h
  template<typename T, param_mutable_type TM>
  cci_param_impl_if* create_cci_param(cci_param<T, TM> *owner_par, const std::string &nam, const cci_value& val, const bool is_top_level_name, cci_broker_if* broker_accessor, const std::string& desc)
  {
	cci_param_impl_if* impl_par = new other_cci_param<T, TM>(*owner_par, nam, val, is_top_level_name, broker_accessor);
    std::cout << "TYPE B new impl_par" << std::endl;
    return impl_par;
  }

} // end namespace


#endif
