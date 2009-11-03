//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2008 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
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

#ifndef __SHOW_DATABASE_H__
#define __SHOW_DATABASE_H__

#include "greencontrol/config.h"

namespace gs {
namespace cnf {

  
  /// Show full database
  /**
   * Shows parameter type and name of all parameters in the db.
   * @param api  cnf_api that should be used to get the parameter list; default=NULL, uses default one
   */
  static inline void show_database(gs::cnf::cnf_api *api = NULL) {
    if (api == NULL)
      api = gs::cnf::GCnf_Api::getApiInstance(NULL);
    std::vector<std::string> vec = api->getParamList();
    std::cout << "All parameters in the GreenConfig database:"<< std::endl;
    std::cout << "  Number of parameters: "<<std::dec<< vec.size()<< std::endl;
    std::vector<std::string>::iterator iter;
    for (unsigned int i = 0; i<vec.size(); i++) {
      std::cout << "   ";
      std::cout << api->getPar(vec[i])->getTypeString();
      std::cout.width(20 - api->getPar(vec[i])->getTypeString().length());
      std::cout << ": ";
      std::cout << vec[i];
      std::cout << " = " << api->getValue(vec[i]);
      std::set<param_attribute> attbs = api->getPar(vec[i])->get_param_attributes();
      std::cout << ", "<<attbs.size() <<" attributes: ";
      for ( std::set<param_attribute>::iterator it = attbs.begin() ; it != attbs.end(); it++ ) {
        if (it != attbs.begin()) std::cout << ", ";
        std::cout << param_attributeToString(*it);
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  
} // end namespace cnf
} // end namespace gs

#endif
