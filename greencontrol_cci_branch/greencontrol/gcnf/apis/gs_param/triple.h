//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2008 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Robert Guenzel <guenzel@eis.cs.tu-bs.de>,
//   Gerrit Meier
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#ifndef __triple_h__
#define __triple_h__


namespace gs {
namespace cnf {
  
  
  class gs_param_base;

  /// Help struct for the SCRSI-Tool
  struct triple {
    std::string m_op;
    const gs_param_base &m_p1, &m_p2;
    triple(const std::string &op, const gs_param_base &p1, const gs_param_base &p2)
      : m_op(op), m_p1(p1), m_p2(p2) { }
  };

  
} //end namespace cnf
} //end namespace gs


#endif
