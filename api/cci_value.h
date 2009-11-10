// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//    Mark Burton, mark@greensocs.com
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
    
    const std::string&      get_string()   const;
    const cci_value_list&   get_list() const;
    bool           get_bool()  const;
    int            get_int()   const;
    boost::int64_t get_int64() const;
    double         get_real()  const;

  protected:
    basic_param_type m_type;
    std::string m_value_string;
    bool m_value_bool;
    boost::int64_t m_value_number;
    double m_value_real;
    cci_value_list m_value_list;
    
  };
  
} // end namespace cci

#endif
