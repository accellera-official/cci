//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//   Marius Montón <marius.monton@uab.cat>,
//     Hardware-Software Prototypes and Solutions Lab (CEPHIS). 
//     Microelectronics and Electronics SystemS Dpt. UAB.
//     http://cephis.uab.cat
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doxygen comments

#ifndef __TESTIP3_H__
#define __TESTIP3_H__

//#define USE_PRIV_API

#define SC_INCLUDE_FX
#include <systemc>

#include "greencontrol/config.h"

using std::string;


/// Test IP which uses the gs_param API.
/**
 * This is a SystemC module which demonstrates the GreenControl parameter API.
 */
class TestIP3
: public sc_core::sc_module
{
  
public:
  
  GC_HAS_CALLBACKS();

  SC_HAS_PROCESS(TestIP3);
	
  /// Constructor
  TestIP3(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
#ifdef USE_PRIV_API
  , m_privApi(this)
#endif
  , int_param ("int_param" )
  , str_param ("str_param", "50")
  , str_param2("str_param2")
  , uint_param("uint_param")
  , uint_param2("uint_param2", "333")
  , bool_param("bool_param")
  , dbl_param ("dbl_param" )

  , float_param ("float_param", "12.4")

  , sc_int_param("sc_int_param", "12")
  , sc_uint_param("sc_uint_param", "1200")
  , sc_bigint_param("sc_bigint_param", "99")
  , sc_biguint_param("sc_biguint_param", "9999")

  , sc_bit_param("sc_bit_param", "SC_LOGIC_1")
  , sc_bit_param2("sc_bit_param2", "1")
  , sc_bv_param("sc_bv_param", "101")

  , sc_logic_param("sc_logic_param", "SC_LOGIC_1") // TODO "1" and "0" work
  , sc_lv_param("sc_lv_param", "10zxZX01")

  , char_param1 ("char_param1" )
  , char_param2 ("char_param2" )
  , char_param3 ("char_param3" )
  , uchar_param ("uchar_param" )
  , schar_param ("schar_param" )
  
  , time_param1("time_param1", sc_core::sc_time(500, sc_core::SC_NS))
  , time_param2("time_param2")
  , time_param3("time_param3")
  { 
    SC_THREAD(main_action);
  }
  
  /// Destructor
  ~TestIP3() {
    GC_UNREGISTER_CALLBACKS();
  }

  /// Main action to make tests with parameters.
  void main_action();

#ifdef USE_PRIV_API
  /// Private API
  gs::cnf::GCnf_private_Api  m_privApi;
#endif
  
  /// Example parameter.
  gs::gs_param<int>             int_param;
  gs::gs_param<std::string>     str_param;
  gs::gs_param<std::string>     str_param2;
  gs::gs_param<unsigned int>    uint_param;
  gs::gs_param<unsigned int>    uint_param2;
  gs::gs_param<bool>            bool_param;
  gs::gs_param<double>          dbl_param;

  gs::gs_param<float>           float_param;
  
  gs::gs_param<sc_dt::sc_int<8> >      sc_int_param;
  gs::gs_param<sc_dt::sc_uint<16> >    sc_uint_param;
  gs::gs_param<sc_dt::sc_bigint<70> >  sc_bigint_param;
  gs::gs_param<sc_dt::sc_bigint<70> >  sc_biguint_param;

  gs::gs_param<sc_dt::sc_bit>           sc_bit_param;
  gs::gs_param<sc_dt::sc_bit>           sc_bit_param2;
  gs::gs_param<sc_dt::sc_bv<3> >        sc_bv_param;
  
  gs::gs_param<sc_dt::sc_logic>         sc_logic_param;
  gs::gs_param<sc_dt::sc_lv<8> >        sc_lv_param;

  gs::gs_param<char>             char_param1;
  gs::gs_param<char>             char_param2;
  gs::gs_param<char>             char_param3;
  gs::gs_param<unsigned char>    uchar_param;
  gs::gs_param<signed char>      schar_param;

  gs::gs_param<sc_core::sc_time>          time_param1;
  gs::gs_param<sc_core::sc_time>          time_param2;
  gs::gs_param<sc_core::sc_time>          time_param3;

  // Callback function for changed parameters
  void config_callback(gs::gs_param_base& par);

  // Callback funtion to be called when new parameters are added or set implicitely
  void new_param_callback(const std::string parname, const std::string value);

};


#endif

