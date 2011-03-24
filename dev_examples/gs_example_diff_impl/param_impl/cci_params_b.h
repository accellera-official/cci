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


#ifndef __CCI_PARAMS_B_H__
#define __CCI_PARAMS_B_H__

//
// This extra library ONLY provides the one addtional data type and NOT the cci_value
//

//#define GS_CCI_CNF_VERBOSE

#ifdef GS_CCI_CNF_VERBOSE
  #define GS_CCI_CNF_DUMP(msg) { std::cout<<msg<<std::endl; } 
#else
  #define GS_CCI_CNF_DUMP(msg) {  } 
#endif


/// namespace macro
#define __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__ different_gs_example_param_implementation 
/// open namespace macro
#define __OPEN_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__ namespace __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__ { 
/// close namespace macro
#define __CLOSE_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__ }

#include "cci.h"
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

namespace cci { namespace cnf {
  
  /// Implementation of parameter factory create function function declared in cci_config.h
  template<typename T, cci::cnf::param_mutable_type TM>
  cci::cnf::cci_param_impl_if<T, TM>* create_cci_param(cci::cnf::cci_param<T, TM> *owner_par, const char* nam, const bool is_top_level_name, cci_cnf_broker_if* broker_accessor) {
    cci::cnf::cci_param_impl_if<T, TM>* impl_par = new __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__::gs_cci_param<T, TM>(*owner_par, nam, is_top_level_name, broker_accessor);
    std::cout << (std::hex) << "TYPE B new impl_par" << std::endl;
    return impl_par;
  }

  
  /// Implementation of parameter factory create function function declared in cci_config.h
  template<typename T, cci::cnf::param_mutable_type TM>
  cci::cnf::cci_param_impl_if<T, TM>* create_cci_param(cci::cnf::cci_param<T, TM> *owner_par, const char* nam, const T& val, const bool is_top_level_name, cci_cnf_broker_if* broker_accessor) {
    cci::cnf::cci_param_impl_if<T, TM>* impl_par = new __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__::gs_cci_param<T, TM>(*owner_par, nam, val, is_top_level_name, broker_accessor);
    std::cout << (std::hex) << "TYPE B new impl_par" << std::endl;
    return impl_par;
  }


  /// Implementation of parameter factory create function function declared in cci_config.h
  template<typename T, cci::cnf::param_mutable_type TM>
  cci::cnf::cci_param_impl_if<T, TM>* create_cci_param(cci::cnf::cci_param<T, TM> *owner_par, const char* nam, const char* val, const bool is_top_level_name, cci_cnf_broker_if* broker_accessor) {
    cci::cnf::cci_param_impl_if<T, TM>* impl_par = new __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__::gs_cci_param<T, TM>(*owner_par, nam, val, is_top_level_name, broker_accessor);
    std::cout << (std::hex) << "TYPE B new impl_par" << std::endl;
    return impl_par;
  }

  /// Implementation of parameter factory init function function declared in cci_config.h
  template<typename T, cci::cnf::param_mutable_type TM>
  void init_cci_param(cci::cnf::cci_param<T, TM> *owner_par) {
    assert(owner_par != NULL && "The given parameter must not be NULL.");
    __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__::gs_cci_param<T, TM>* p = dynamic_cast<__NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__::gs_cci_param<T, TM>*> (owner_par->get_pImpl());
    assert(p != NULL && "The given parameter should be created by the create_cci_param function thus being of the implementation type.");
    p->init();
  }
  

  /// Implementation of parameter factory destroy function function declared in cci_config.h
  template<class T, cci::cnf::param_mutable_type TM>
  void destroy_cci_param(cci::cnf::cci_param<T, TM>* param) {
    std::cout << (std::hex) << "TYPE B delete param" << std::endl;
    assert(param != NULL && "The given parameter must not be NULL.");
    __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__::gs_cci_param<T, TM>* p = dynamic_cast<__NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__::gs_cci_param<T, TM>*> (param->get_pImpl());
    assert(p != NULL && "The given parameter should be created by the create_cci_param function thus being of the implementation type.");
    delete p; p = NULL;
  }
  

} } // end namespace


#endif
