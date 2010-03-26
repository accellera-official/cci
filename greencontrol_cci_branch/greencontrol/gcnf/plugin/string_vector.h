//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Michael Ruetz <m.ruetz@tu-bs.de>,
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>
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

#ifndef __STRING_VECTOR_H__
#define __STRING_VECTOR_H__

#include "greencontrol/core/log_if.h"

namespace gs {
namespace cnf {


/// This is an extended std::vector<std::string> with additional log_if methods.
/**
 * This class extends an std::vector<std::string> by adding the methods demanded by the log_if interface.
 */
class string_vector
  : public std::vector<std::string>
  , public log_if
{
public:

  /// Copy operator.
  string_vector& operator = (const vector<string>& v)
  {
    vector<string>::operator=(v);
    return *this;
  }

  /// Returns a string representation of the vector.
  std::string toString()
  {
    std::stringstream ss;
    ss << getTypeLog() << " = { " << getValueLog() << " }";
    return ss.str();
  }

  /// Returns a string representation of the payload.
  std::string getValueLog()
  {
    std::stringstream ss;
    unsigned int i=0;

    for(vector<string>::iterator it = this->begin(); it != this->end(); ++it)
    {
      i++;
      ss << *it;
      if(i < this->size())
        ss << ", ";
    }

    return ss.str();
  }

  /// Returns the datatype of the vector.
  std::string getTypeLog()
  {
    return std::string("vector<string>");
  }

  /// Should return the name of the vector.
  std::string getNameLog()
  {
    return std::string("N/A");
  }
};


} // end namespace cnf
} // end namespace gs

#endif
