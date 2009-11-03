//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
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

// doxygen comments

#ifndef __gc_transaction_gs_param_h__
#define __gc_transaction_gs_param_h__


#include "greencontrol/config.h"


namespace gs {
namespace cnf {

  
// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< gc_transaction > //////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

/// Template specialization for gs_param<gc_transaction>.
/**
 * Default value = resetted transaction.
 */
template<>
  class gs_param<ControlTransaction>
: public gs_param_t<ControlTransaction>
{
  /// Typedef for the value.
  typedef ControlTransaction val_type;
  
public:
  GS_PARAM_HEAD;
  
  // ///////////////////////
  //  operators
  
  
  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("ControlTransaction");
  }
  
  /// Overloads gs_param_base::getType
  const Param_type getType() const {
    return PARTYPE_NOT_AVAILABLE;
  }
  
  /// Overloads gs_param_t<T>::convertValueToString
  std::string convertValueToString(const val_type &val) const {
    return static_convertValueToString(val);
  }  
  inline static std::string static_convertValueToString(const val_type &val) {
    return val.toString();
  }
  
  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    std::istringstream ss(str);
    GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: string='"<< ss.str().c_str()<<"'");
    if (ss.str().length() == 0) {
      target_val.reset();
      GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: reset to default");
      return true;
    }
    // TODO
    SC_REPORT_ERROR("gc_transaction::deserialize()", "deserialize not implemented for ControlTransaction");
    return false;
  }
  
};


} // end namespace cnf
} // end namespace gs

#endif
