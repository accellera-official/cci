/*****************************************************************************
 
 The following code is derived, directly or indirectly, from the SystemC
 source code Copyright (c) 1996-2009 by all Contributors.
 All Rights reserved.
 
 Developed by GreenSocs : http://www.greensocs.com/
 Christian Schroeder, schroeder@eis.cs.tu-bs.de
 Mark Burton, mark@greensocs.com
 
 The contents of this file are subject to the restrictions and limitations
 set forth in the SystemC Open Source License Version 3.0 (the "License");
 You may not use this file except in compliance with such restrictions and
 limitations. You may obtain instructions on how to receive a copy of the
 License at http://www.systemc.org/. Software distributed by Contributors
 under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 ANY KIND, either express or implied. See the License for the specific
 language governing rights and limitations under the License.
 
 *****************************************************************************/

#ifndef __CCI_VALUE_H__
#define __CCI_VALUE_H__


#include "cci_datatypes.h"


namespace cci {

  class cci_value;
  typedef std::vector<cci_value> cci_value_list;
  
  /// Value class for objects representing arbitrary types of cci parameters
  /**
   *
   */
  class cci_value {
  public:
    cci_value();  // no (null) value
    cci_value(const char*           value); 
    cci_value(const std::string&    value);
    cci_value(bool                  value);
    cci_value(int                   value);
    cci_value(boost::int64_t        value);
    cci_value(double                value);

    cci_value(const cci_value_list& value);
    cci_value(const cci_value& other);
    
    bool operator==(const cci_value& lhs) const;
    cci_value& operator=(const cci_value& lhs);
    
    basic_param_type type() const;
    
    const std::string&      get_str()   const;
    const cci_value_list&   get_list() const;
    bool           get_bool()  const;
    int            get_int()   const;
    boost::int64_t get_int64() const;
    double         get_real()  const;
  };
  
} // end namespace cci

#endif
