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


#ifndef __CCI_VALUE_H__
#define __CCI_VALUE_H__


#include "cci_datatypes.h"


__CCI_OPEN_CONFIG_NAMESPACE__


  class cci_value;
  
  /// List of cci_values
  typedef std::vector<cci_value> cci_value_list;
  
  /// Value class for objects representing arbitrary types of cci parameters
  /**
   * The constructor sets the value and the type, the empty constructor applies
   * an empty/invalid value, setting the type to partype_not_available.
   *
   * The value can be read using the correct get function according the type
   * returned by type().
   * There may be other get functions but the correct one, returning another
   * representation of the value, e.g. the type is cci::partype_number, the correct
   * get function would be get_int64(), but also get_int(), get_string() and get_real() 
   * might return valid string representations. If a particular get function cannot 
   * return a valid representation of the value, an sc_report error of id
   * cci_report_types::cci_value_failure is thrown.
   */
  class cci_value {
  public:
    /// Constructor with no (null) value (sets type to cci::partype_not_available)
    cci_value();  
    /// Constructor setting the string value (sets type to cci::partype_string)
    cci_value(const char*           value); 
    /// Constructor setting the string value (sets type to cci::partype_string)
    cci_value(const std::string&    value);
    /// Constructor setting the bool value (sets type to cci::partype_bool)
    cci_value(bool                  value);
    /// Constructor setting the number value (sets type to cci::partype_number)
    cci_value(int                   value);
    /// Constructor setting the number value (sets type to cci::partype_number)
    cci_value(sc_dt::int64          value);
    /// Constructor setting the real value (sets type to cci::partype_real)
    cci_value(double                value);

    /// Constructor setting the list value (sets type to cci::partype_list)
    cci_value(const cci_value_list& value);
    /// Constructor copying another value
    cci_value(const cci_value& other);
    
    /// Checks for equality
    bool operator==(const cci_value& lhs) const;

    /// Assigns a new value and type
    cci_value& operator=(const cci_value& lhs);
    
    /// Returns the best matching basic type
    basic_param_type type() const;
    
    /// Returns the value's string representation
    /**
     * @throw cci_report_types::cci_value_failure if the value cannot be represented as this data type (or it is not implemented)
     * @return The value's string representation.
     */
    const std::string&      get_string()   const;
    
    /// Returns the value's list representation
    /**
     * @throw cci_report_types::cci_value_failure if the value cannot be represented as this data type (or it is not implemented)
     * @return The value's list representation.
     */
    const cci_value_list&   get_list() const;
    
    /// Returns the value's bool representation
    /**
     * @throw cci_report_types::cci_value_failure if the value cannot be represented as this data type (or it is not implemented)
     * @return The value's bool representation.
     */
    bool           get_bool()  const;
    
    /// Returns the value's int representation
    /**
     * @throw cci_report_types::cci_value_failure if the value cannot be represented as this data type (or it is not implemented)
     * @return The value's int representation.
     */
    int            get_int()   const;
    
    /// Returns the value's int64 representation
    /**
     * @throw cci_report_types::cci_value_failure if the value cannot be represented as this data type (or it is not implemented)
     * @return The value's int64 representation.
     */
    sc_dt::int64 get_int64() const;
    
    /// Returns the value's double representation
    /**
     * @throw cci_report_types::cci_value_failure if the value cannot be represented as this data type (or it is not implemented)
     * @return The value's double representation.
     */
    double         get_real()  const;

  protected:
    /// Best matching basic param type
    basic_param_type m_type;
    /// string value (if this is of type string)
    std::string m_value_string;
    /// bool value (if this is of type bool)
    bool m_value_bool;
    /// sc_dt::int64 value (if this is of type number / int / sc_dt::int64)
    sc_dt::int64 m_value_number;
    /// double value (if this is of type double / real)
    double m_value_real;
    /// list value (if this is of type cci::cnf::cci_value_list)
    cci_value_list m_value_list;
    
  };
  

__CCI_CLOSE_CONFIG_NAMESPACE__

#endif
