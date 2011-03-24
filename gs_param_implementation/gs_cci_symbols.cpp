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
// PART 1
//
// This makes sure the symbols for standard parameter types are available in this library!
//
// Note: Only standard parameter types shall be provided here! Additional provided types shall be
//       provided in separate libraries / object files to allow exchangeablility of different
//       parameter vendor libraries.
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

  // TODO: add (ONLY!) standard parameter types here!!
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

} } // end namespace


  
