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

#include "cci_params.h"

//
// This makes sure the symbols for supported parameter types are available in the library!
//

namespace cci { namespace cnf {

#define CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, MUTABILITY_TYPE) \
  template  cci::cnf::cci_param_impl_if<PTYPE, MUTABILITY_TYPE>*  create_cci_param (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const char* nam,                   const bool force_top_level_name); \
  template  cci::cnf::cci_param_impl_if<PTYPE, MUTABILITY_TYPE>*  create_cci_param (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const char* nam, const PTYPE& val, const bool force_top_level_name); \
  template  cci::cnf::cci_param_impl_if<PTYPE, MUTABILITY_TYPE>*  create_cci_param (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const char* nam, const char* val,  const bool force_top_level_name); \
  template                                                  void  init_cci_param   (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par); \
  template                                                  void  destroy_cci_param(cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE>* param    );

#define CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(PTYPE) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::mutable_parameter) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::immutable_parameter) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::elaboration_time_parameter) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::other_parameter)

  // TODO: add all supported parameter types here!!
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(int)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(unsigned int)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(bool)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(double)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(float)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(std::string)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(long long)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(unsigned char)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(signed char)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(char)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(unsigned short)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(std::vector<std::string>)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(sc_dt::sc_int_base)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(sc_dt::sc_uint_base)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(sc_dt::sc_signed)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(sc_dt::sc_unsigned)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(sc_dt::sc_bit)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(sc_dt::sc_logic)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(sc_core::sc_time)
  //CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(sc_core::sc_event)

  /*CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(sc_core::sc_attribute<T>)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(sc_dt::sc_int<w>)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(sc_dt::sc_uint<w>)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(sc_dt::sc_bigint<w>)
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(sc_dt::sc_biguint<w>)*/

  
} } // end namespace
