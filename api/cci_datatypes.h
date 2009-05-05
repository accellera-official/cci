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
  enum Param_type {
    // default, used when not specified
    PARTYPE_NOT_AVAILABLE = 0,
    // POD
    PARTYPE_INT,
    PARTYPE_UINT,
    PARTYPE_BOOL,
    PARTYPE_DOUBLE,
    PARTYPE_FLOAT,
    PARTYPE_STRING,
    PARTYPE_ULONGLONG,
    PARTYPE_LONGLONG,
    PARTYPE_UCHAR,
    PARTYPE_USHORT,
    // SystemC
    PARTYPE_SC_BIT,
    PARTYPE_SC_INT_BASE,
    PARTYPE_SC_INT,
    PARTYPE_SC_UINT_BASE,
    PARTYPE_SC_UINT,
    PARTYPE_SC_SIGNED,
    PARTYPE_SC_BIGINT,
    PARTYPE_SC_UNSIGNED,
    PARTYPE_SC_BIGUINT,
    PARTYPE_SC_LOGIC,
    PARTYPE_SC_TIME,
    PARTYPE_SC_ATTRIBUTE,
    // Last
    LAST_PARTYPE
  };
  
  namespace param_attributes {
    /// Enumeration for parameter attributes
    enum param_attribute_enum {
      undefined = 0,
      config,
      runtime_config,
      elab_config,
      read_only,
      analysis,
      temp,
      output,
      internal,
      gr_attribute,
      gr_register,
      gr_splitio_register,
      gr_sharedio_register,
      gr_bit_range,
      last_attribute
    };
  } // end namespace param_attribute
  
  static inline std::string param_attributeToString(unsigned int att) {
    switch (att) {
      case param_attributes::undefined:
        return std::string("undefined");
      case param_attributes::config:
        return std::string("config");
      case param_attributes::runtime_config:
        return std::string("runtime_config");
      case param_attributes::elab_config:
        return std::string("elab_config");
      case param_attributes::read_only:
        return std::string("read_only");
      case param_attributes::analysis:
        return std::string("analysis");
      case param_attributes::temp:
        return std::string("temp");
      case param_attributes::output:
        return std::string("output");
      case param_attributes::internal:
        return std::string("internal");
      case param_attributes::gr_attribute:
        return std::string("gr_attribute");
      case param_attributes::gr_register:
        return std::string("gr_register");
      case param_attributes::gr_splitio_register:
        return std::string("gr_splitio_register");
      case param_attributes::gr_sharedio_register:
        return std::string("gr_sharedio_register");
      case param_attributes::gr_bit_range:
        return std::string("gr_bit_range");
      default:
        return std::string("_unknown_");
    }
  }
  
  typedef unsigned int param_attribute;
  
  

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
