/*******************************************************************************
  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2010 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.
******************************************************************************/

/**
 *  @file   simple_ip.h
 *  @brief  A simple IP model that demonstrate the use of mutable CCI
 *          parameters for each fully supported datatype.
 *  @author A. Nizamudheen, TI
 */

#ifndef INC_SIMPLE_IP_H
#define INC_SIMPLE_IP_H

//------------------------------------------------------------------------------
#include <string>
#include "cci"
#include "xreport.hpp"
//#include "boost/cstdint.hpp"

#define MK64(a,b) (((boost::int64_t)(a)<< 32) | (b))

//------------------------------------------------------------------------------
/// The SimpleIP owns CCI Params representing basic data types
SC_MODULE(simple_ip) {
 public:
  /// Ctor
 SC_CTOR(simple_ip)
   : bool_param  ("bool_param",  true)
   , char_param  ("char_param",  'A')
   , uchar_param ("uchar_param", 'U')
   , int_param   ("int_param",   -1)
   , uint_param  ("uint_param",  0xFEEDF00D)
   //, int16_param ("int16_param", 0x1234)
   //, int64_param ("int64_param", MK64(0x9,0x876543210))
   , float_param ("float_param", 3.14)
   , double_param("double_param",2.714)
   , string_param("string_param","C++ String")
   {
      XREPORT("@Ctor: Default value of "<< bool_param.get_name()  << " is "  << bool_param.get());
      XREPORT("@Ctor: Default value of "<< char_param.get_name()  << " is "  << char_param.get());
      XREPORT("@Ctor: Default value of "<< uchar_param.get_name() << " is "  << uchar_param.get());
      XREPORT("@Ctor: Default value of "<< int_param.get_name()   << " is "  << int_param.get());
      XREPORT("@Ctor: Default value of "<< uint_param.get_name()  << " is 0x"<< std::hex << uint_param.get());
      //XREPORT("@Ctor: Default value of "<< int16_param.get_name() << " is 0x"<< std::hex << int16_param.get());
      //XREPORT("@Ctor: Default value of "<< int64_param.get_name() << " is 0x"<< std::hex << int64_param.get());
      XREPORT("@Ctor: Default value of "<< float_param.get_name() << " is "  << float_param.get());
      XREPORT("@Ctor: Default value of "<< double_param.get_name()<< " is "  << double_param.get());
      XREPORT("@Ctor: Default value of "<< string_param.get_name()<< " is "  << string_param.get());

      SC_THREAD(execute);
    }

  /// Main execution block (no functionality)
  void execute() {
    // Wait for 20 ns
    wait (20, sc_core::SC_NS);
  }

 private:
  /// Parameters of basic data types
  cci::cnf::cci_param<bool>          bool_param;
  cci::cnf::cci_param<char>          char_param;
  cci::cnf::cci_param<unsigned char> uchar_param;
  cci::cnf::cci_param<int>           int_param;
  cci::cnf::cci_param<unsigned int>  uint_param;
  //cci::cnf::cci_param<short int>     int16_param;
  //cci::cnf::cci_param<boost::int64_t> int64_param;
  cci::cnf::cci_param<float>         float_param;
  cci::cnf::cci_param<double>        double_param;
  cci::cnf::cci_param<std::string>   string_param;
}; // simple_ip

#endif // INC_SIMPLE_IP_H

