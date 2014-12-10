/* The following code is derived, directly or indirectly, from the SystemC
 * source code Copyright (c) 1996-2010 by all Contributors.
 * All Rights reserved.
 *
 * The contents of this file are subject to the restrictions and limitations
 * set forth in the SystemC Open Source License Version 2.2.0 (the "License");
 * One may not use this file except in compliance with such restrictions and
 * limitations.  One may obtain instructions on how to receive a copy of the
 * License at http://www.systemc.org/.  Software distributed by Contributors
 * under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 * ANY KIND, either express or implied. See the License for the specific
 * language governing rights and limitations under the License.
 ********************************************************************************/

/**
 * @file    parameter_owner.h
 * @brief   This header instantiates various CCI parameters with default values
 * @author  Girish Verma, CircuitSutra Technologies    <girish@circuitsutra.com>
 *          P V S Phaneendra, CircuitSutra Technologies    <pvs@circuitsutra.com>
 * @date    31st August, 2011 (Wednesday)
 */

#ifndef   EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_USER_DATATYPE_H_
#define   EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_USER_DATATYPE_H_

#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <cci>
#include <systemc.h>
#include <cci_cnf/cci_shared_ptr.h>  // HACK; a proper publicly shareable CCI wrapper needed
#include <greencontrol/config.h>
#include <string>
#include <iostream>

#include "json_spirit/json_spirit.h"

/// Route_table_ut structure defines three fields of a typical router
/// source_addr, dest_addr, and index
struct route_table_ut {
 public:
  route_table_ut()
      : s_address(0x0),  // Source Address
        d_address(0x0),  // Destination Address
        index(0x0) {}    // Index

  /// Overloaded Constructor
  route_table_ut(int saddr, int daddr, int idx)
      : s_address(saddr),  // Source Address
        d_address(daddr),  // Destination Address
        index(idx) {}      // Index

 public:
  /// Source address field
  int s_address;
  /// Destination address field
  int d_address;
  /// Index field
  int index;
};

// TODO(girish/pvs): Compilation Error here!
/// Provide parameter vendor implementation support for user-defined data type
typedef gs::cnf::gs_param<route_table_ut> user_data_type;

/// Overload extraction operator of C++
std::ostream& operator <<(std::ostream& os, const route_table_ut& ud) {
  json_spirit::Object user_dt_obj;
  std::ostringstream json_ostream;
  std::string json_str;
  user_dt_obj.push_back(json_spirit::Pair("s_address",
                                          static_cast<int>(ud.s_address)));
  user_dt_obj.push_back(json_spirit::Pair("d_address",
                                          static_cast<int>(ud.d_address)));
  user_dt_obj.push_back(json_spirit::Pair("index",
                                          static_cast<int>(ud.index)));

  json_spirit::write(user_dt_obj, json_ostream);
  json_str = json_ostream.str();

  os << json_ostream.str();

  return os;
}

/// Overload insersion operation of C++
std::istream& operator >>(std::istream& is, route_table_ut& ud) {
  json_spirit::Value jvalue;
  read(is, jvalue);
  json_spirit::Object usr_dt_obj = jvalue.get_value<json_spirit::Object>();
  for (unsigned int i = 0; i < usr_dt_obj.size(); i++) {
    if (usr_dt_obj[i].name_.compare("s_address") == 0)
      ud.s_address = usr_dt_obj[i].value_.get_value<int>();

    if (usr_dt_obj[i].name_.compare("d_address") == 0)
      ud.d_address = usr_dt_obj[i].value_.get_value<int>();

    if (usr_dt_obj[i].name_.compare("index") == 0)
      ud.index = usr_dt_obj[i].value_.get_value<int>();
  }

  /* json_spirit lib modify  error flag  */
  is.clear();
  is.setstate(ifstream::eofbit);

  return is;
}
/// ex16_user_datatype

#endif  // EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_USER_DATATYPE_H_
