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
//
//   This program is free software.
// 
//   If you have no applicable agreement with GreenSocs Ltd, this software
//   is licensed to you, and you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   If you have a applicable agreement with GreenSocs Ltd, the terms of that
//   agreement prevail.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//   02110-1301  USA 
// 
// ENDLICENSETEXT

// doxygen comments

#ifndef __TESTIP_NEG_H__
#define __TESTIP_NEG_H__

#include <systemc>

#include "../demo_globals.h"
#include "greencontrol/config.h"


/// Test IP which uses the gs_param API for negative regression tests.
/**
 * This is a SystemC module which demonstrates the GreenControl parameter API.
 */
class TestIP_neg
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(TestIP_neg);
	
  /// Constructor
  TestIP_neg(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      int_param ("int_param" ),
      str_param ("str_param", "50"),
      str_param2("str_param2"),
      uint_param("uint_param"),
      uint_param2("uint_param2", "333"),
      bool_param("bool_param"),
      dbl_param ("dbl_param" ),

      float_param ("float_param", "12.4"),

      sc_int_param("sc_int_param", "12"),
      sc_uint_param("sc_uint_param", "1200"),
      sc_bigint_param("sc_bigint_param", "99"),
      sc_biguint_param("sc_biguint_param", "9999"),

      sc_bit_param("sc_bit_param", "SC_LOGIC_1"),
      sc_bit_param2("sc_bit_param2", "1"),
      sc_bv_param("sc_bv_param", "101"),

      sc_logic_param("sc_logic_param", "SC_LOGIC_1"), // TODO "1" and "0" work
      sc_lv_param("sc_lv_param", "10zxZX01")

  { 
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();

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

  gs::gs_param<sc_dt::sc_bit>          sc_bit_param;
  gs::gs_param<sc_dt::sc_bit>          sc_bit_param2;
  gs::gs_param<sc_dt::sc_bv<3> >       sc_bv_param;
  
  gs::gs_param<sc_dt::sc_logic>        sc_logic_param;
  gs::gs_param<sc_dt::sc_lv<8> >       sc_lv_param;

  void config_callback(const std::string parname, const std::string value);

};


#endif

