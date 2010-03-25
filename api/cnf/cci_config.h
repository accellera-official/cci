// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//    Mark Burton, mark@greensocs.com
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




#ifndef __CCI_CONFIG_H__
#define __CCI_CONFIG_H__


#include <systemc>

//
//  This is the main include file for the config part of the CCI.
//  The top-level include file is cci.h
//

#define CCI_CNF_VERBOSE

#ifdef CCI_CNF_VERBOSE
# define CCI_CNF_DUMP(msg) { std::cout<<msg<<std::endl; } 
#else
# define CCI_CNF_DUMP(msg) {  } 
#endif

/// Namespace for OSCI Configuration, Control & Inspection (CCI) standard, Config part
#define __CCI_OPEN_CONFIG_NAMESPACE__  namespace cci { namespace cnf {
#define __CCI_CLOSE_CONFIG_NAMESPACE__ } }

__CCI_OPEN_CONFIG_NAMESPACE__

  // forward declaration
  class cci_cnf_api;

  /// Returns a configuration API instance responsible for handling the requests of the given module 
  /**
   * Forward declaration, to be implemented by the tool.
   *
   * @param mod Pointer to the module who wants access the config API, can be NULL if e.g. top-level or non-SystemC code is requesting the API.
   * @return Configuration API.
   */
  cci_cnf_api* get_cnf_api_instance(sc_core::sc_module* mod = NULL);

__CCI_CLOSE_CONFIG_NAMESPACE__

#include "cci_error_types.h"
#include "cci_datatypes.h"
#include "cci_callbacks.h"
#include "cci_cnf_api.h"

#include "cci_base_param_if.h"
#include "cci_base_param.h"

#include "cci_param_if.h"

namespace cci_impl {
  
  template<typename T, cci::cnf::param_mutable_type TM>
  static cci::cnf::cci_param_if<T, TM>* CreateParam(cci::cnf::cci_param<T, TM> *owner_par, const char* nam, const bool force_top_level_name);
  
  template<typename T, cci::cnf::param_mutable_type TM>
  static cci::cnf::cci_param_if<T, TM>* CreateParam(cci::cnf::cci_param<T, TM> *owner_par, const char* nam, const T& val, const bool force_top_level_name);
  
  template<typename T, cci::cnf::param_mutable_type TM>
  static cci::cnf::cci_param_if<T, TM>* CreateParam(cci::cnf::cci_param<T, TM> *owner_par, const char* nam, const char* val, const bool force_top_level_name);
  
  template<typename T, cci::cnf::param_mutable_type TM>
  static void InitParam(cci::cnf::cci_param_if<T, TM> *owner_par);

  template<class T, cci::cnf::param_mutable_type TM>
  void DestroyParam(cci::cnf::cci_param_if<T, TM>* param);

} // end namespace cci_impl

#include "cci_param.h"

#include "cci_broker_module.h"


#endif
