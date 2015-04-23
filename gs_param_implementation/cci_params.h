// LICENSETEXT
//
//   Copyright (C) 2009-2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//    Mark Burton, mark@greensocs.com
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#ifndef __CCI_PARAMS_H__
#define __CCI_PARAMS_H__


//#define GS_CCI_CNF_VERBOSE

#ifdef GS_CCI_CNF_VERBOSE
  #define GS_CCI_CNF_DUMP(msg) { std::cout<<msg<<std::endl; } 
#else
  #define GS_CCI_CNF_DUMP(msg) {  } 
#endif


/// namespace macro
#define __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__ gs_example_param_implementation 
/// open namespace macro
#define __OPEN_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__ namespace __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__ { 
/// close namespace macro
#define __CLOSE_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__ }

#include "cci"
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
#undef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__

namespace cci { namespace cnf {

  /// Implementation of parameter factory create function function declared in cci_config.h
  template<typename T, param_mutable_type TM>
  cci_param_impl_if* create_cci_param(cci_param<T, TM> *owner_par, const std::string &nam, const T& val, const bool is_top_level_name, cci_cnf_broker_if* broker_accessor) {
    cci_param_impl_if* impl_par = new __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__::gs_cci_param<T, TM>(*owner_par, nam, val, is_top_level_name, broker_accessor);
    //std::cout << (std::hex) << "new impl_par=" << impl_par << std::endl;
    return impl_par;
  }


  /// Implementation of parameter factory create function function declared in cci_config.h
  template<typename T, param_mutable_type TM>
  cci_param_impl_if* create_cci_param(cci_param<T, TM> *owner_par, const std::string &nam, const cci_value& val, const bool is_top_level_name, cci_cnf_broker_if* broker_accessor) {
    cci_param_impl_if * impl_par = new __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__::gs_cci_param<T, TM>(*owner_par, nam, val, is_top_level_name, broker_accessor);
    //std::cout << (std::hex) << "new impl_par=" << impl_par << std::endl;
    return impl_par;
  }

} } // end namespace


#endif
