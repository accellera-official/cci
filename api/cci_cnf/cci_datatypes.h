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

#ifndef CCI_CNF_CCI_DATATYPES_H_INCLUDED_
#define CCI_CNF_CCI_DATATYPES_H_INCLUDED_

#include "cci_config_macros.h"

__CCI_OPEN_CONFIG_NAMESPACE__

  /// Originator information when functions being called by the main object (which only the owner shall use)
  extern const char* PARAM_ORIGINATOR;

  /// Enumeration for basic data types, whose rough getting and setting is supported by base class directly
  enum basic_param_type {
    /// default, type unknown
    param_type_not_available = 0, 
    /// used for all param data types that can handle integer numbers
    param_type_number,   
    /// used for all param data types that can handle real numbers
    param_type_real,     
    /// used for all param data types that can handle bool
    param_type_bool,     
    /// used for all param data types that can handle strings
    param_type_string,   
    /// used for all param lists 
    param_type_list,     
    /// used for all param other data types (objects etc.)
    param_type_other     
  };
  
  
  /// Enumeration for cci_param template specifying the parameter type according the lock behavior
  enum param_mutable_type {
    /// Mutable Parameter
    mutable_param = 0,
    /// Immutable Parameter
    immutable_param,
    /// Elaboration Time Parameter (Mutable up to end_of_elaboration)
    elaboration_time_param,
    /// Vendor specific/other Parameter type
    other_param
  };
  
  /// Enumeration for convenient cci_param constructor: for chose if a param gets a top-level-name
  enum cci_top_level_name { CCI_TOP_LEVEL_NAME=0 };
  
  
__CCI_CLOSE_CONFIG_NAMESPACE__

#endif
