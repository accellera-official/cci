/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 ****************************************************************************/

/**
 *  @file   simple_ip.h
 *  @brief  A simple IP model that demonstrate the use of mutable CCI
 *          parameters for each fully supported datatype.
 *  @author A. Nizamudheen, TI
 */

#ifndef EXAMPLES_EX02_FULLY_SUPPORTED_DATA_TYPE_PARAM_EX02_SIMPLE_IP_H_
#define EXAMPLES_EX02_FULLY_SUPPORTED_DATA_TYPE_PARAM_EX02_SIMPLE_IP_H_

#include <cci_configuration>
#include <string>
#include "xreport.hpp"
 #include "boost/cstdint.hpp"

 #define MK64(a, b) (((boost::int64_t)(a)<< 32) | (b))

/**
 *  @class  ex02_simple_ip
 *  @brief  The class for the simple_ip which owns CCI params representing basic data types
 */
SC_MODULE(ex02_simple_ip) {
 public:
  /**
   *  @fn     void ex02_simple_ip()
   *  @brief  class constructor
   *  @return void
   */
  SC_CTOR(ex02_simple_ip)
      : bool_param("bool_param", true),
        char_param("char_param", 'A'),
        uchar_param("uchar_param", 'U'),
        int_param("int_param", -1),
        uint_param("uint_param", 0xFEEDF00D),
        int16_param ("int16_param", 0x1234),
        int64_param ("int64_param", MK64(0x9,0x876543210)),
        float_param("float_param", 3.14),
        double_param("double_param", 2.714),
        string_param("string_param", "C++ String") {
    XREPORT("@Ctor: Default value of " << bool_param.get_name() << " is "
            << bool_param.get_value());
    XREPORT("@Ctor: Default value of " << char_param.get_name() << " is "
            << char_param.get_value());
    XREPORT("@Ctor: Default value of " << uchar_param.get_name() << " is "
            << uchar_param.get_value());
    XREPORT("@Ctor: Default value of " << int_param.get_name() << " is "
            << int_param.get_value());
    XREPORT("@Ctor: Default value of " << uint_param.get_name() << " is 0x"
            << std::hex << uint_param.get_value());
    XREPORT("@Ctor: Default value of " << int16_param.get_name() <<" is 0x"
		    << std::hex << int16_param.get_value());
    XREPORT("@Ctor: Default value of " << int64_param.get_name() <<" is 0x" 
	        << std::hex << int64_param.get_value());
    XREPORT("@Ctor: Default value of " << float_param.get_name() << " is "
            << float_param.get_value());
    XREPORT("@Ctor: Default value of " << double_param.get_name() << " is "
            << double_param.get_value());
    XREPORT("@Ctor: Default value of " << string_param.get_name() << " is "
            << string_param.get_value());

    SC_THREAD(execute);
  }

  /**
   *  @fn     void execute()
   *  @brief  Main execution block (no real functionality)
   *  @return void
   */
  void execute() {
    // Wait for 20ns to allow config_ip to update parameter value
    wait(20, sc_core::SC_NS);
  }

 private:
  /// Parameters of basic data types
  cci::cci_param<bool> bool_param; ///< boolean parameter
  cci::cci_param<char> char_param; ///< char parameter
  cci::cci_param<unsigned char> uchar_param; ///< unsigned char parameter
  cci::cci_param<int> int_param; ///< int parameter
  cci::cci_param<unsigned int> uint_param; ///< unsigned int parameter
  cci::cci_param<short int> int16_param;  ///< 16-bit integer parameter
  cci::cci_param<boost::int64_t> int64_param; ///< 64-bit integer parameter
  cci::cci_param<float> float_param; ///< float parameter
  cci::cci_param<double> double_param; ///< double parameter
  cci::cci_param<std::string> string_param;  ///< string parameter
};
// ex02_simple_ip

#endif  // EXAMPLES_EX02_FULLY_SUPPORTED_DATA_TYPE_PARAM_EX02_SIMPLE_IP_H_
