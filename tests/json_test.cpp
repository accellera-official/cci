/*
 *  json_test.cpp
 *  greenstarcore
 *
 *  Created by Christian Schroeder on 25.08.09.
 *  Copyright 2009 TU Braunschweig, E.I.S.. All rights reserved.
 *
 */

#include "gs_cci.h"
#include "../gs_implementation/json_spirit/json_spirit.h"

using namespace std;

class JSON_string_owner {
public:
  std::string my_json_string;
};

static std::string json_Value_type_to_string(json_spirit::Value_type ty) {
  switch (ty) {
    case json_spirit::obj_type:   return "obj_type";
    case json_spirit::array_type: return "array_type";
    case json_spirit::str_type:   return "str_type";
    case json_spirit::bool_type:  return "bool_type";
    case json_spirit::int_type:   return "int_type";
    case json_spirit::real_type:  return "real_type";
    case json_spirit::null_type:  return "null_type";
  }
  return "unknown";
}

int sc_main(int argc, char *argv[]) {

  JSON_string_owner jo;
  
  jo.my_json_string = "[\"I am a string in a JSON string\", 1234566787899, 10.45, true]";
  
  json_spirit::Value value;
  read(jo.my_json_string, value);
  
  cout << "JSON value type: " << json_Value_type_to_string(value.type()) << endl;
  
  json_spirit::Array arr = value.get_array();
  
  for (json_spirit::Array::iterator it = arr.begin(); it != arr.end(); it++) {
    cout << "  type " << json_Value_type_to_string((*it).type()) << endl;
  }
  
  
  return EXIT_SUCCESS; 
}
