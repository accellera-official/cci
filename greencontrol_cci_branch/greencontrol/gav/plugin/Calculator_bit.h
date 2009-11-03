//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2008 : GreenSocs Ltd
//    http://www.greensocs.com/ , email: info@greensocs.com
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

#ifndef __Calculator_bit_h__
#define __Calculator_bit_h__


#include "gav_globals.h"
#include "greencontrol/config.h"
#include "calc_if.h"
#include "Calculator.h"
#include <deque>


namespace gs {
namespace av {

  
  /// Calculator class that derives from <code>Calculator<T></code> and has bitwise operators
  /**
   * This class may not be used for e.g. T=double.
   * Use this calculator if you need bitwise operators.
   */
  template<typename T>
  class Calculator_bit
  : public Calculator<T> {
    
    typedef Calculator<T> base_calc;    
    
  public:
    
    /// Constructor registering additional bitwise functions ('operations')
    Calculator_bit(const char* name)
    : base_calc(name)
    {
      typename base_calc::fpointer fptr;
      // additional bitwise functions
      fptr=&bit_and;
      base_calc::m_func_vec.push_back(fptr);
      base_calc::m_op_map["&"]=base_calc::m_func_vec.size() - 1;
       
      fptr=&bit_or;
      base_calc::m_func_vec.push_back(fptr);
      base_calc::m_op_map["|"]=base_calc::m_func_vec.size() - 1;
    }
  
  protected:

    /// bitwise and
    static T bit_and(const T x, const T y, bool sloppy) {
      return (x & y);
    }
    /// bitwise or
    static T bit_or(const T x, const T y, bool sloppy) {
      return (x | y);
    }
    
  };


} // end namespace av
} // end namespace gs

#endif // __Calculator_bit_h__
