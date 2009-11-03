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
//         Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>

// doxygen comments

#include "scml_orig_ip.h"

void Scml_Orig_IP::main_action() {
  wait(sc_core::SC_ZERO_TIME); // let the ScmlTestIP set the values in case of gc scml_properties
  
  std::cout << std::endl << std::endl;
  std::cout << "Scml_Orig_IP scml properties: "<< std::endl;
  std::cout << "Int    intProp:    " << intProp << std::endl;
  std::cout << "Bool   boolProp:   " << boolProp << std::endl;
  std::cout << "Double doubleProp: " << doubleProp << std::endl;
  std::cout << "String stringProp: " << stringProp << std::endl;
}
