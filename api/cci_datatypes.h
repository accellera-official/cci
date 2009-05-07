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
