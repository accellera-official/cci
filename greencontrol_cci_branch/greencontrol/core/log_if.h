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

#ifndef __LOG_IF_H__
#define __LOG_IF_H__

namespace gs {
namespace ctr {


/// Interface to be implemented by parameters or classes, that are passed via a ControlTransaction.
/**
 * This interface is to be implemented by parameters or classes, that are passed via an AnyPointer in a
 * ControlTransaction. These classes have to provide some information about their content, type etc. in
 * the form of std::strings, so the GC_Logger is able to get further information about the payload in
 * a ControlTransaction.
 */
class log_if
{
public:
  virtual ~log_if() {}

  /// Should return a string with full information about the parameter (e.g. name, type and value).
  virtual std::string toString() = 0;

  /// Should return the value of the parameter as a string.
  virtual std::string getValueLog() = 0;

  /// Should return the type of the parameter as a string, if available.
  virtual std::string getTypeLog() = 0;

  /// Should return a string with the name of the parameter, if available.
  virtual std::string getNameLog() = 0;
};


} // end namespace ctr
} // end namespace gs

#endif
