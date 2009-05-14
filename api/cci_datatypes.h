//   OSCI CCI WG
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// 
// ENDLICENSETEXT


#ifndef __CCI_DATATYPES_H__
#define __CCI_DATATYPES_H__


namespace cci {

  
  /// Enumeration for natively supported data types
  enum Param_JSON_type {
    partype_not_available = 0, // default, type unknown
    partype_json_number,   // used for all param data types that can handle numbers
    partype_json_bool,     // used for all param data types that can handle bool
    partype_json_string,   // used for all param data types that can handle strings
    partype_json_other     // used for all param other data types (objects, arrays etc.)
  };
  

  // Output of the parameter objects
#ifdef CCI_PARAM_VERBOSE
# define CCI_PARAM_DUMP(msg) { std::cout<<this->name()<<": "<<msg<<std::endl; } 
# define CCI_PARAM_DUMP_WITHNAME(name, msg) { std::cout<<name<<": "<<msg<<std::endl; } 
#else
# define CCI_PARAM_DUMP(msg)
# define CCI_PARAM_DUMP_WITHNAME(name, msg)
#endif

  
} // end namespace cci

#endif
