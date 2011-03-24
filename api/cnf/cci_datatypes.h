// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
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



#ifndef __CCI_DATATYPES_H__
#define __CCI_DATATYPES_H__


#include <string>
#include <iostream>


__CCI_OPEN_CONFIG_NAMESPACE__

  /// Originator information when functions being called by the main object (which only the owner shall use)
  static const char* PARAM_ORIGINATOR = "owning_param";

  /// Enumeration for basic data types, whose rough getting and setting is supported by base class directly
  enum basic_param_type {
    /// default, type unknown
    partype_not_available = 0, 
    /// used for all param data types that can handle integer numbers
    partype_number,   
    /// used for all param data types that can handle real numbers
    partype_real,     
    /// used for all param data types that can handle bool
    partype_bool,     
    /// used for all param data types that can handle strings
    partype_string,   
    /// used for all param lists 
    partype_list,     
    /// used for all param other data types (objects etc.)
    partype_other     
  };
  
  
  /// Enumeration for cci_param template specifying the parameter type according the lock behavior
  enum param_mutable_type {
    /// Mutable Parameter
    mutable_parameter = 0,
    /// Immutable Parameter
    immutable_parameter,
    /// Elaboration Time Parameter (Mutable up to end_of_elaboration)
    elaboration_time_parameter,
    /// Vendor specific/other Parameter type
    other_parameter
  };
  
  /// Enumeration for convenient cci_param constructor: for chose if a param gets a top-level-name
  enum cci_top_level_name { CCI_TOP_LEVEL_NAME=0 };
  
// Debug output of the parameter objects
#ifdef CCI_PARAM_VERBOSE
# define CCI_PARAM_DUMP(msg) { std::cout<<this->name()<<": "<<msg<<std::endl; } 
# define CCI_PARAM_DUMP_WITHNAME(name, msg) { std::cout<<name<<": "<<msg<<std::endl; } 
#else
# define CCI_PARAM_DUMP(msg)
# define CCI_PARAM_DUMP_WITHNAME(name, msg)
#endif

  
__CCI_CLOSE_CONFIG_NAMESPACE__

#endif
