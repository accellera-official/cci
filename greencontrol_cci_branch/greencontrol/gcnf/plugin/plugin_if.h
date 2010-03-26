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

// doygen comments

#ifndef __PLUGIN_IF_H__
#define __PLUGIN_IF_H__

#include "greencontrol/core/gc_globals.h"


namespace gs {
namespace cnf {


/// Interface to allow the observer database to get access to the parameter database of the ConfigPlugin.
/**
 * The ConfigPlugin has to implement this interface to be connected to the port of the
 * observer database.
 */
struct plugin_if
{
public:

  virtual ~plugin_if() { }

  /// @see gs::cnf::param_database_if::setParam
  //virtual bool setParam(const std::string &hier_parname, const std::string &value) = 0;

  /// @see gs::cnf::param_database_if::existsParam
  virtual bool existsParam(const std::string &hier_parname) = 0;

};


} // end namespace cnf
} // end namespace gs

#endif
