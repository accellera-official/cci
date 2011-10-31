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


#ifndef __PARAMETEROWNERMODULE_H__
#define __PARAMETEROWNERMODULE_H__


#include <systemc>
#include "ex_globals.h"
#include "cci.h"
#include "test_datatype_without_parameter_implementation.h"

/// Module which owns some cci parameters.
class ParameterOwnerModule
: public sc_core::sc_module
{
  
public:
  
  SC_HAS_PROCESS(ParameterOwnerModule);

  /* Test data types:
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
   TODO:
  std::vector<std::string>
  sc_dt::sc_int_base
  sc_dt::sc_uint_base
  sc_dt::sc_signed
  sc_dt::sc_unsigned
  sc_dt::sc_bit
  sc_dt::sc_logic
  sc_core::sc_time
  sc_dt::sc_int<w> w=1...64
  sc_dt::sc_uint<w> w=1...64
  sc_dt::sc_bigint<w> w=1...1024
  sc_dt::sc_biguint<w> w=1...1024
   */ 
  
  /// Constructor
  ParameterOwnerModule(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  , int_param   ("int_param"   , 50 )
  , uint_param  ("uint_param"  , 12000)
  , bool_param  ("bool_param"  , true)
  , double_param("double_param", 12.345)
  , float_param ("float_param" , 12.345)
  , str_param   ("str_param"   , "This is a test string.")
  , ll_param    ("ll_param"    , 9000000000000000009ull)
  , uchar_param ("uchar_param" , 255)
  , schar_param ("schar_param" , -128)
  , char_param  ("char_param"  , -128)
  , ushort_param("ushort_param", 65535)
  , some_param  ("some_param")
  { 
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();
  
  /// Example parameters
  cci::cnf::cci_param<int>             int_param;
  cci::cnf::cci_param<unsigned int>    uint_param;
  cci::cnf::cci_param<bool>            bool_param;
  cci::cnf::cci_param<double>          double_param;
  cci::cnf::cci_param<float>           float_param;
  cci::cnf::cci_param<std::string>     str_param;
  cci::cnf::cci_param<long long>       ll_param;
  cci::cnf::cci_param<unsigned char>   uchar_param;
  cci::cnf::cci_param<signed char>     schar_param;
  cci::cnf::cci_param<char>            char_param;
  cci::cnf::cci_param<unsigned short>  ushort_param;
  cci::cnf::cci_param<test_datatype_without_parameter_implementation> some_param;
  
};


#endif
