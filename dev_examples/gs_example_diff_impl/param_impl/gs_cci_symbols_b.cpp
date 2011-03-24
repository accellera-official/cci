// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#include "cci_params_b.h"
#include "test_datatype.h"

//
// This makes sure the symbols for supported parameter types are available in the library!
//

namespace cci { namespace cnf {

#define CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, MUTABILITY_TYPE) \
  template  cci::cnf::cci_param_impl_if<PTYPE, MUTABILITY_TYPE>*  create_cci_param (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const char* nam,                   const bool is_top_level_name, cci_cnf_broker_if* broker_accessor); \
  template  cci::cnf::cci_param_impl_if<PTYPE, MUTABILITY_TYPE>*  create_cci_param (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const char* nam, const PTYPE& val, const bool is_top_level_name, cci_cnf_broker_if* broker_accessor); \
  template  cci::cnf::cci_param_impl_if<PTYPE, MUTABILITY_TYPE>*  create_cci_param (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const char* nam, const char* val,  const bool is_top_level_name, cci_cnf_broker_if* broker_accessor); \
  template                                                  void  init_cci_param   (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par); \
  template                                                  void  destroy_cci_param(cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE>* param    );

#define CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(PTYPE) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::mutable_parameter) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::immutable_parameter) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::elaboration_time_parameter) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::other_parameter)

  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(test_datatype)

} } // end namespace


std::istream& operator >>(std::istream &is,test_datatype &obj)
{
  is>>obj.strVal;
  return is;
}
std::ostream& operator <<(std::ostream &os,const test_datatype &obj)
{
  os<<obj.strVal;
  return os;
}

