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

namespace cci {

#define CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, MUTABILITY_TYPE) \
  template  cci_param_impl_if*  create_cci_param (cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const std::string &nam, const PTYPE& val, const bool is_top_level_name, cci_broker_if* broker_accessor); \
  template  cci_param_impl_if*  create_cci_param (cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const std::string &nam, const cci_value& val,  const bool is_top_level_name, cci_broker_if* broker_accessor);

#define CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(PTYPE) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, mutable_param) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, immutable_param) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, elaboration_time_param) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, other_param)

  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(test_datatype)

} // end namespace


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

// add support for cci_value and JSON (de)serialization
namespace cci {
template<>
struct cci_value_traits< test_datatype >
{
  typedef test_datatype type;
  static bool pack( cci_value::reference dst, type const & src )
  {
    dst.set_string( src.strVal );
    return true;
  }
  static bool unpack( type & dst, cci_value::const_reference src )
  {
    if( !src.is_string() )
      return false;

    dst.strVal = src.get_string();
    return true;
  }
};
} /* namespace cci */
