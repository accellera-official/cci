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

#ifndef __INITIALIZE_IF_H__
#define __INITIALIZE_IF_H__

namespace gs {
namespace cnf {


/// Interface to be implemented by each API.
/**
 * Because the APIs should not be sc_core::sc_modules (they shouldn't be named) they cannot
 * use the callback functions end_of_elaboration etc.
 * So they need to be informed when they are allowed to start the initial configuration.
 * The initial configuration is during the initialize-mode and has to be done without
 * notifys to sc_event (so immediate calls in the payload event queue must be done)
 *
 * The inizialize-mode is marked as left by the end_initialize_mode call.
 *
 */
class initialize_if
{

public:

  virtual ~initialize_if() {}

  /**
   * Inside this method the initial configuration has to be done.
   * This is called by the GreenControl Core during the start_of_simulation callback.
   */
  virtual void start_initial_configuration() = 0;

  /**
   * Informs the APIs about the done start_of_simulation call in the Core:
   * leave the initialze-mode
   */ 
  virtual void end_initialize_mode() = 0;
    
};


} // end namespace cnf
} // end namespace gs

#endif
