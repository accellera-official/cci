//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2010 : GreenSocs Ltd
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

#include "scml_orig_ip.h"

#define SHOW_W_TIME(msg)   std::printf("@%s /%d: ", sc_core::sc_time_stamp().to_string().c_str(), (unsigned)sc_core::sc_delta_count()); std::cout << msg << std::endl

Scml_Orig_IP::~Scml_Orig_IP() {
  std::cout << std::endl << std::endl;
  std::cout << "Destructor "<<name()<<std::endl;
  show_properties();
  std::cout << std::endl << std::endl;
  std::cout << std::endl << std::endl;
}

void Scml_Orig_IP::main_action() {
  wait(sc_core::SC_ZERO_TIME); // let the ScmlTe  stIP set the values in case of gc scml_properties
  show_properties();

  std::cout << std::endl << std::endl;
  SHOW_W_TIME("m_int_prop += 100" << std::endl);
  m_int_prop = m_int_prop + 100;
  SHOW_W_TIME("m_double_prop *= 2" << std::endl);
  m_double_prop *= 2;
  show_properties();
  
  wait(1, sc_core::SC_PS);
  show_properties();

  std::cout << std::endl << std::endl;
  SHOW_W_TIME("m_string_prop = \"value set by model\"" << std::endl);
  m_string_prop = "value set by model";
  SHOW_W_TIME("m_bool_prop = false" << std::endl);
  m_bool_prop = false;
  show_properties();
}

void Scml_Orig_IP::show_properties() {
  std::cout << std::endl << std::endl;
  SHOW_W_TIME(" "<< name() <<" scml properties: "<< std::endl);
  std::cout << "  String "<<m_string_prop.getName()<<": " << m_string_prop << std::endl;
  std::cout << "  Int    "<<m_int_prop.getName()<<":    " << m_int_prop << std::endl;
  std::cout << "  Bool   "<<m_bool_prop.getName()<<":   " << m_bool_prop << std::endl;
  std::cout << "  Double "<<m_double_prop.getName()<<": " << m_double_prop << std::endl;
}
