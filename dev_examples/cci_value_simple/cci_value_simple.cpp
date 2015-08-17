/*****************************************************************************
  Copyright 2006-2015 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2006-2014 OFFIS Institute for Information Technology
  All rights reserved.

  Copyright 2006-2015 Intel Corporation
  All rights reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 *****************************************************************************/

/**
 * @file   cci_value_simple/main.cpp
 * @author Philipp A. Hartmann, OFFIS/Intel
 * @brief  Simple example of the new cci_value interface
 */

#include "cci_cfg/cci_value.h"
#include "cci_cfg/cci_value_traits.h"

using cci::cci_value;
using cci::cci_value_list;
using cci::cci_value_map;

int sc_main( int, char*[] )
{
  std::string json;
  {
    cci_value v;

    sc_assert( v.is_null() );
    sc_assert( v.json_serialize(json) );
    std::cout << "JSON (null)   : " << cci_value::to_json(v) << std::endl;
    sc_assert( v.json_deserialize(json) );

    v.set_bool( true );
    sc_assert( v.is_bool() );
    sc_assert( v.is_true() );
    sc_assert( !v.is_false() );
    sc_assert( v.json_serialize(json) );
    std::cout << "JSON (bool)   : " << cci_value::to_json(v) << std::endl;
    sc_assert( v.json_deserialize(json) );
  }
  {
    cci_value i;

    i.set<char>(42);
    sc_assert( i.is_int() );
    sc_assert( i.json_serialize(json) );
    std::cout << "JSON (int)    : " << cci_value::to_json(i) << std::endl;
    sc_assert( i.json_deserialize(json) );

    sc_assert( i.get_int() == 42 );
    sc_assert( i.get<long>() == 42 );
    sc_assert( i.get_uint() == 42 );
    sc_assert( i.get_uint64() == 42 );
    sc_assert( i.get<unsigned long>() == 42 );
  }
  {
    sc_dt::sc_int<17> i( "0b0101010" );
    cci_value v;
    v.set( i );

    sc_assert( v.is_int() );
    sc_assert( v.json_serialize(json) );
    std::cout << "JSON (sc_int) : " << cci_value::to_json(v) << std::endl;
    sc_assert( v.json_deserialize(json) );

    sc_dt::sc_int_base ib( i.length() );
    sc_assert( v.try_get(ib) );
    sc_assert( v.try_get(i) );
    sc_assert( ib == i );
  }
  {
    cci_value v(1.5);
    sc_assert( v.is_double() );
    sc_assert( v.get<float>() == 1.5f );
    v.set_null();
    v.set<float>( 42.f );
    sc_assert( v.is_double() );

    sc_assert( v.json_serialize(json) );
    std::cout << "JSON (double) : " << cci_value::to_json(v) << std::endl;
    sc_assert( v.json_deserialize(json) );
  }
  {
    std::string str("string");
    cci_value v( "string" ), w( str );

    sc_assert( v.is_string() && w.is_string() );
    sc_assert( str == v.get_string() );
    sc_assert( w == v.get_string() );
    sc_assert( v.json_serialize(json) );
    std::cout << "JSON (string) : " << cci_value::to_json(v) << std::endl;
    sc_assert( v.json_deserialize(json) );
  }
  {
    const char json[] = "[12,34]";
    cci_value v = cci_value::from_json( json );

    sc_assert( v.is_list() );
    sc_assert( cci_value::to_json(v) == json );

    std::vector<int> seq[2];
    v.get_list()
      .push_back("56")
      .push_back( seq ) // array of std::vector
      .push_back(
        cci_value_list()
          .push_back( 0ul )
          .push_back( cci_value() )
        )
      .push_back( sc_dt::SC_LOGIC_Z );

    sc_assert( v.get_list()[3].is_list() );
    sc_assert( v.get_list()[4].get_list().size() == 2 );

    cci_value::list_reference vl = v.get_list()[3].get_list();
    sc_assert( vl.size() == 2 );
    vl[0].get_list().push_back(78);
    vl[1].get_list().push_back(9);
    sc_assert( vl.try_get(seq) );
    sc_assert( seq[0][0] == 78 && seq[1][0] == 9 );
    std::cout << "JSON (list)   : " << cci_value::to_json(v) << std::endl;
  }
  {
    const char json[] = "{\"value\":10,\"unit\":\"ns\"}";
    cci_value_map v = cci_value::from_json( json ).get_map();

    sc_assert( v.is_map() );
    sc_assert( v.has_entry("value") );
    sc_assert( v.has_entry("unit") );
    sc_assert( !v.has_entry("scale") );

    sc_core::sc_time t;
    sc_assert( v.try_get(t) );
    std::cout << "JSON (sc_time): " << cci_value::to_json( cci_value(t) ) << std::endl;

    sc_assert( cci_value::to_json(v) == json );
    v.push_entry( "scale", 1 );
    sc_assert( v.size() == 3 );
    sc_assert( v.has_entry("scale") );
    std::cout << "JSON (map)    : " << cci_value::to_json(v) << std::endl;
  }

  return EXIT_SUCCESS;
}
