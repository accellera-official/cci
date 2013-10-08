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

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "cci_params_b.h"
#include "test_datatype.h"

//
// This makes sure the symbols for supported parameter types are available in the library!
//

namespace cci { namespace cnf {

#define CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, MUTABILITY_TYPE) \
  template  cci::cnf::cci_param_impl_if<PTYPE, MUTABILITY_TYPE>*  create_cci_param (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const std::string &nam,                   const bool is_top_level_name, cci_cnf_broker_if* broker_accessor); \
  template  cci::cnf::cci_param_impl_if<PTYPE, MUTABILITY_TYPE>*  create_cci_param (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const std::string &nam, const PTYPE& val, const bool is_top_level_name, cci_cnf_broker_if* broker_accessor); \
  template  cci::cnf::cci_param_impl_if<PTYPE, MUTABILITY_TYPE>*  create_cci_param (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const std::string &nam, const char* val,  const bool is_top_level_name, cci_cnf_broker_if* broker_accessor); \
  template                                                  void  init_cci_param   (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par); \
  template                                                  void  destroy_cci_param(cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE>* param    );

#define CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(PTYPE) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::mutable_param) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::immutable_param) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::elaboration_time_param) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::other_param)

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
bool operator==(test_datatype const& lhs, test_datatype const& rhs) {
  return lhs.strVal.compare(rhs.strVal);
}

__OPEN_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__

/// Helper class template specialization: to make cci_values work
template<cci::cnf::param_mutable_type TM>
struct cci_value_helper<test_datatype, TM> {
  typedef test_datatype my_type;
  static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
    test_datatype d;
    switch (val.type() ) {
      case cci::cnf::param_type_string:
        d.strVal = val.get_string();
        param.set(d);
        break;
      default:
        cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with not supported value type.");
    }
  }
  
  static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
    test_datatype d;
    std::string s = param.get().strVal;
    cci::cnf::cci_value val(s);
    return val;    
  }
};

__CLOSE_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__
