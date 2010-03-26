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

#ifndef __COMMAND_IF_H__
#define __COMMAND_IF_H__

namespace gs {
namespace ctr {


/// Interface to be implemented by each API/Plugin.
/**
 * This interface has to be implemented by the APIs and Plugins, so other modules can get the name
 * and a descrition of the commands provided by an API/Plugin pair. The APIs and Plugins should use
 * internal static methods to be provided by the plugins, so the code that provides the information
 * exists only once.
 */
class command_if
{
public:

  virtual ~command_if() {}

  /**
   * This method has to return the name of the API/Plugin implementing command_if.
   */
  virtual std::string getName() = 0;

  /**
   * This method has to return the name of the specified command.
   */
  virtual std::string getCommandName(unsigned int cmd) = 0;

  /**
   * This method has to return a description of the specified command.
   */ 
  virtual std::string getCommandDescription(unsigned int cmd) = 0;  
};


} // end namespace ctr
} // end namespace gs

#endif
