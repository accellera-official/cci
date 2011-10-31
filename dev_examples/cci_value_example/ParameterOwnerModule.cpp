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


#include "ParameterOwnerModule.h"
using namespace std;

void ParameterOwnerModule::main_action() {

  // demonstrate value setting using cci_value
  /*
   int
   unsigned int
   bool
   double
   float
   std::string
   long long
   unsigned char
   signed char
   char
   unsigned short
  */   
  
  cci::cnf::cci_value int_v(-10);
  DEMO_DUMP(name(), "Set and get parameter Owner.int_param - value = " << int_v.get_int());
  int_param.set_value(int_v);
  int inte = int_param.get_value().get_int();
  DEMO_DUMP(name(), "  got " << inte << endl);
  
  cci::cnf::cci_value uint_v(10);
  DEMO_DUMP(name(), "Set and get parameter Owner.uint_param - value = " << uint_v.get_int());
  uint_param.set_value(uint_v);
  unsigned int uint = uint_param.get_value().get_int64();
  DEMO_DUMP(name(), "  got " << uint << endl);

  cci::cnf::cci_value bool_v(true);
  DEMO_DUMP(name(), "Set and get parameter Owner.bool_param - value = " << bool_v.get_bool());
  bool_param.set_value(bool_v);
  bool boole = bool_param.get_value().get_bool();
  DEMO_DUMP(name(), "  got " << boole << endl);

  cci::cnf::cci_value dbl_v(10.123);
  DEMO_DUMP(name(), "Set and get parameter Owner.double_param - value = " << dbl_v.get_real());
  double_param.set_value(dbl_v);
  double dbl = double_param.get_value().get_real();
  DEMO_DUMP(name(), "  got " << dbl << endl);
  
  cci::cnf::cci_value flt_v(98.765);
  DEMO_DUMP(name(), "Set and get parameter Owner.float_param - value = " << flt_v.get_real());
  float_param.set_value(flt_v);
  float flt = float_param.get_value().get_real();
  DEMO_DUMP(name(), "  got " << flt << endl);
  
  cci::cnf::cci_value str_v("This is a string");
  DEMO_DUMP(name(), "Set and get parameter Owner.str_param - value = " << str_v.get_string());
  str_param.set_value(str_v);
  std::string stri = str_param.get_value().get_string();
  DEMO_DUMP(name(), "  got \"" << stri << "\"" << endl);
  
  cci::cnf::cci_value ll_v((sc_dt::int64)1000000000000000001ull);
  DEMO_DUMP(name(), "Set and get parameter Owner.ll_param - value = " << ll_v.get_int64());
  ll_param.set_value(ll_v);
  long long ll = ll_param.get_value().get_int64();
  DEMO_DUMP(name(), "  got " << ll << endl);
  
  cci::cnf::cci_value uc_v(70);
  DEMO_DUMP(name(), "Set and get parameter Owner.uchar_param - value = " << (unsigned char) uc_v.get_int());
  uchar_param.set_value(uc_v);
  unsigned char uc = uchar_param.get_value().get_int();
  DEMO_DUMP(name(), "  got " << uc << endl);
  
  cci::cnf::cci_value sc_v(71);
  DEMO_DUMP(name(), "Set and get parameter Owner.schar_param - value = " << (signed char) sc_v.get_int());
  schar_param.set_value(sc_v);
  signed char sc = schar_param.get_value().get_int();
  DEMO_DUMP(name(), "  got " << sc << endl);
  
  cci::cnf::cci_value c_v(72);
  DEMO_DUMP(name(), "Set and get parameter Owner.char_param - value = " << (char) c_v.get_int());
  char_param.set_value(c_v);
  char c = char_param.get_value().get_int();
  DEMO_DUMP(name(), "  got " << c << endl);
  
  cci::cnf::cci_value us_v(244);
  DEMO_DUMP(name(), "Set and get parameter Owner.ushort_param - value = " << (unsigned short) us_v.get_int());
  ushort_param.set_value(us_v);
  unsigned short us = ushort_param.get_value().get_int();
  DEMO_DUMP(name(), "  got " << us << endl);
  
  //TODO weitere Datentypen testen
  
  
  
  cout << endl;
  cout << "-------------------------" << endl;
  
  // demonstrate setting a parameter using a cci value OF WRONG TYPE
  DEMO_DUMP(name(), "Set unsigned integer parameter Owner.uint_param to string value=\"Hello\" using WRONG cci_value type!");
  cci::cnf::cci_value valStr("Hello");
  try {
    uint_param.set_value(valStr);
  } catch(sc_core::sc_report e) {
    switch ( cci::cnf::cci_report_handler::get_param_failure(e) ) {
      case cci::cnf::CCI_VALUE_FAILURE: 
        std::cout << std::endl << name() << ": Caught " << e.what() << std::endl;
        break;
      default:
        // If other error, throw it again
        throw e;
    }
  }
  cout << endl;

  // demonstrate setting a parameter (that does not support cci_values) using a cci value
#ifdef ENABLE_CCI_VALUE_SUPPORT_FOR_TEST_DATATYPE
  DEMO_DUMP(name(), "Set user data type parameter that does not support cci_values using a cci_value (disabled ENABLE_CCI_VALUE_SUPPORT_FOR_TEST_DATATYPE)!");
#else
  DEMO_DUMP(name(), "Set user data type parameter that does support cci_values using a cci_value (enabled ENABLE_CCI_VALUE_SUPPORT_FOR_TEST_DATATYPE)!");
#endif
  try {
    some_param.set_value(valStr);
  } catch(sc_core::sc_report e) {
    switch ( cci::cnf::cci_report_handler::get_param_failure(e) ) {
      case cci::cnf::CCI_VALUE_FAILURE: 
        std::cout << std::endl << name() << ": Caught " << e.what() << std::endl;
        break;
      default:
        // If other error, throw it again
        throw e;
    }
  }
  

}
