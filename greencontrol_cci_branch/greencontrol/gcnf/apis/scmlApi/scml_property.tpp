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

#include "scml_property.h"


using std::string;
using std::ostream;
using std::istream;


/************************************************************************
 * gcnfScmlProperty_base IMPLEMENTATIONS
 */

inline gcnfScmlProperty_base::gcnfScmlProperty_base(const string &par_name)
: sc_object(sc_gen_unique_name("__scml_par__"))
//: sc_object()
{
  std::stringstream ss;
  ss << name() << SC_NAME_DELIMITER << par_name;
  m_par_name = ss.str().c_str();

}

inline gcnfScmlProperty_base::~gcnfScmlProperty_base()
{
}

inline int gcnfScmlProperty_base::getIntValue() const
{
  throw WrongTypeException("int", getType());
  return 0;
}

inline bool gcnfScmlProperty_base::getBoolValue() const
{
  throw WrongTypeException("bool", getType());
  return false;
}

inline double gcnfScmlProperty_base::getDoubleValue() const
{
  throw WrongTypeException("double", getType());
  return 0.0;
}

inline string gcnfScmlProperty_base::getStringValue() const
{
  throw WrongTypeException("string", getType());
  return "";
}

inline unsigned int gcnfScmlProperty_base::getUIntValue() const
{
  throw WrongTypeException("unsigned int", getType());
  return 0;
}

inline void gcnfScmlProperty_base::setIntValue(int value)
{
  throw WrongTypeException("int", getType());
}

inline void gcnfScmlProperty_base::setBoolValue(bool value)
{
  throw WrongTypeException("bool", getType());
}

inline void gcnfScmlProperty_base::setDoubleValue(double value)
{
  throw WrongTypeException("double", getType());
}

inline void gcnfScmlProperty_base::setStringValue(const ::std::string &value)
{
  throw WrongTypeException("string", getType());
}

inline void gcnfScmlProperty_base::setUIntValue(unsigned int value)
{
  throw WrongTypeException("unsigned int", getType());
}

inline std::string gcnfScmlProperty_base::getType() const
{
  return "UNKNOWN";
}
