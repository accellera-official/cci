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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doxygen comments

#ifndef __SHOW_DATABASE_H__
#define __SHOW_DATABASE_H__

#include "greencontrol/config.h"
#include <sstream>
#include <algorithm>

namespace gs {
namespace cnf {

  
  /// Show full database
  /**
   * Shows parameter type and name of all parameters in the db.
   * Does not impact the is_used status.
   *
   * Example:
   *    type   : param.name
   *
   * @param api  cnf_api_if that should be used to get the parameter list; default=NULL, uses default one
   * @param indentation  If to indent the output according the longest entity
   * @param show_values  If to show the parameter values
   */
  static inline void show_database(gs::cnf::cnf_api_if *api = NULL, bool indentation = true, bool show_values = true) {
    struct local {
      // ----------- begin local function simple ---------------------------
      static void show_database_simple(gs::cnf::cnf_api_if *api = NULL, bool show_values = true) {
        const unsigned int position_par_name   = 26;
        const unsigned int position_par_val    = 60;
        const unsigned int position_par_attrib = 80;
        if (api == NULL)
          api = gs::cnf::GCnf_Api::getApiInstance(NULL);
        std::vector<std::string> vec = api->getParamList();
        std::cout << "All parameters in the GreenConfig database:"<< std::endl;
        std::cout << "  Number of parameters: "<<std::dec<< vec.size()<< std::endl;
        std::vector<std::string>::iterator iter;
        for (unsigned int i = 0; i<vec.size(); i++) {
          std::stringstream ss;
          // explicit param
          if (api->getPar(vec[i])) {
            ss << "   ";
            ss << api->getPar(vec[i])->getTypeString();
            ss.width(position_par_name - ss.str().length());
            ss << ": " << vec[i];
            ss.width(position_par_val - ss.str().length());
            if (show_values) ss << " = " << api->getValue(vec[i], "", true); // not impact is_used status
            std::set<param_attribute> attbs = api->getPar(vec[i])->get_param_attributes();
            ss.width(position_par_attrib - ss.str().length());
            if (attbs.size() > 0) {
              ss << ", "<<attbs.size() <<" attributes: ";
              for ( std::set<param_attribute>::iterator it = attbs.begin() ; it != attbs.end(); it++ ) {
                if (it != attbs.begin()) std::cout << ", ";
                ss << param_attributeToString(*it);
              }
            } else {
              ss << ", no attributes ";
            }
          }
          // implicit param
          else {
            ss << "   ";
            ss << "<implicit>";
            ss.width(position_par_name - ss.str().length());
            ss << ": " << vec[i];
            ss.width(position_par_val - ss.str().length());
            ss << " = " << api->getValue(vec[i], "", true); // not impact is_used status
            if (api->is_used(vec[i])) ss << ", used"; else ss << ", not used";
          }
          std::cout << ss.str() << std::endl;
        }
        std::cout << std::endl;
      }
      // ----------- end local function simple ---------------------------
    
      // ----------- begin local function with indentation ---------------
      static void show_database_indentation(gs::cnf::cnf_api_if *api = NULL, bool show_values = true) {
        unsigned int position_par_name_indentation_max   = 0;
        unsigned int position_par_val_indentation_max    = 0;
        unsigned int position_par_attrib_indentation_max = 0;
        if (api == NULL)
          api = gs::cnf::GCnf_Api::getApiInstance(NULL);
        std::vector<std::string> vec = api->getParamList();
        std::cout << "All parameters in the GreenConfig database:"<< std::endl;
        std::cout << "  Number of parameters: "<<std::dec<< vec.size()<< std::endl;
        std::vector<std::string>::iterator iter;
        
        std::vector<std::stringstream*> ss_vec;
        for (unsigned int i = 0; i<vec.size(); i++) {
          ss_vec.push_back(new std::stringstream());
          ss_vec[i]->fill(' ');
        }
        
        // Parameter types
        for (unsigned int i = 0; i<vec.size(); i++) {
          // explicit param
          if (api->getPar(vec[i])) {
            *ss_vec[i] << "   ";
            *ss_vec[i] << api->getPar(vec[i])->getTypeString();
          }
          // implicit param
          else {
            *ss_vec[i] << "   ";
            *ss_vec[i] << "<implicit>";
          }
          // update max indentation
          //std::cout << "position_par_name_indentation_max["<<i<<"]=max("<<position_par_name_indentation_max<<","<<ss_vec[i]->str().length()<<")="<<std::max<unsigned int>(position_par_name_indentation_max, ss_vec[i]->str().length())<<std::endl;
          position_par_name_indentation_max = std::max<unsigned int>(position_par_name_indentation_max, ss_vec[i]->str().length());
        }
        position_par_name_indentation_max += 3;
        
        // Parameter names
        for (unsigned int i = 0; i<vec.size(); i++) {
          ss_vec[i]->width(position_par_name_indentation_max - ss_vec[i]->str().length());
          *ss_vec[i] << ": ";
          *ss_vec[i] << vec[i];
          // update max indentation
          //std::cout << "position_par_val_indentation_max["<<i<<"]=max("<<position_par_val_indentation_max<<","<<ss_vec[i]->str().length()<<")="<<std::max<unsigned int>(position_par_val_indentation_max, ss_vec[i]->str().length())<<std::endl;
          position_par_val_indentation_max = std::max<unsigned int>(position_par_val_indentation_max, ss_vec[i]->str().length());
        }
        position_par_val_indentation_max += 3;
        
        // Parameter values
        if (show_values) {
          for (unsigned int i = 0; i<vec.size(); i++) {
            //std::cout << ss_vec[i]->str().length() << " \"" << ss_vec[i]->str() << "\"" << std::endl;
            ss_vec[i]->width(position_par_val_indentation_max - ss_vec[i]->str().length());
            *ss_vec[i] << "= ";
            *ss_vec[i] << api->getValue(vec[i], "", true); // not impact is_used status
            //std::cout << ss_vec[i]->str().length() << " \"" << ss_vec[i]->str() << "\"" << std::endl;
            // update max indentation
            //std::cout << "position_par_attrib_indentation_max["<<i<<"]=max("<<position_par_attrib_indentation_max<<","<<ss_vec[i]->str().length()<<")="<<std::max<unsigned int>(position_par_attrib_indentation_max, ss_vec[i]->str().length())<<std::endl;
            position_par_attrib_indentation_max = std::max<unsigned int>(position_par_attrib_indentation_max, ss_vec[i]->str().length());
          }
          position_par_attrib_indentation_max += 3;
        }
        
        // Parameter attributes and is_used status
        for (unsigned int i = 0; i<vec.size(); i++) {
          ss_vec[i]->width(position_par_attrib_indentation_max - ss_vec[i]->str().length());
          // explicit param
          if (api->getPar(vec[i])) {
            std::set<param_attribute> attbs = api->getPar(vec[i])->get_param_attributes();
            //ss_vec[i]->width(position_par_attrib_indentation_max - ss_vec[i]->str().length());
            *ss_vec[i] << ",";
            if (attbs.size() > 0) {
              *ss_vec[i] << attbs.size() <<" attributes: ";
              for ( std::set<param_attribute>::iterator it = attbs.begin() ; it != attbs.end(); it++ ) {
                if (it != attbs.begin()) *ss_vec[i] << ", ";
                *ss_vec[i] << param_attributeToString(*it);
              }
            } else {
              *ss_vec[i] << " no attributes";
            }
          }
          // implicit param
          else {
            if (!api->is_used(vec[i])) {
              *ss_vec[i] << ",";
              *ss_vec[i] << " UNCONSUMED!";
            }
          }
        }

        // Output
        for (unsigned int i = 0; i<vec.size(); i++) {
          std::cout << ss_vec[i]->str() << std::endl;
          delete ss_vec[i]; ss_vec[i] = NULL;
        }
        
        std::cout << std::endl;
      }
      // ----------- begin local function with indentation ---------------
    }; // end struct local
    
    if (indentation)
      local::show_database_indentation(api, show_values);
    else
      local::show_database_simple(api, show_values);    
  }

  
  
} // end namespace cnf
} // end namespace gs

#endif
