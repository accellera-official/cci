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

/// Main CCI namespace
namespace cci {
  /// Namespace for CCI configuration part
  namespace cnf {
  }
}
__CCI_OPEN_CONFIG_NAMESPACE__

  // forward declaration
  class cci_cnf_broker;

  /// Returns a configuration broker instance responsible for handling the requests of the given module 
  /**
   * Forward declaration, to be implemented by the tool.
   *
   * @param mod Pointer to the module who wants access the config broker, can be NULL if e.g. top-level or non-SystemC code is requesting the broker.
   * @return Configuration broker.
   */
  cci_cnf_broker* get_cnf_broker_instance(sc_core::sc_module* mod = NULL);

__CCI_CLOSE_CONFIG_NAMESPACE__

#include "cci_error_types.h"
#include "cci_datatypes.h"
#include "cci_callbacks.h"
#include "cci_cnf_broker.h"

#include "cci_base_param_if.h"
#include "cci_base_param.h"

#include "cci_param_if.h"

__CCI_OPEN_CONFIG_NAMESPACE__
  
/// Parameter factory function being called from the cci param constructor to construct the underlying parameter
/**
 * The implementation shall return a new parameter object.
 *
 * @param owner_par CCI Parameter which will store the returned param
 * @param nam The (local or top-level) name the parameter shall get
 * @param force_top_level_name If the given name shall be a top-level name
 */
template<typename T, cci::cnf::param_mutable_type TM>
static cci::cnf::cci_param_if<T, TM>* create_cci_param(cci::cnf::cci_param<T, TM> *owner_par, const char* nam, const bool force_top_level_name);

/// Parameter factory function being called from the cci param constructor to construct the underlying parameter
/**
 * The implementation shall return a new parameter object.
 *
 * @param owner_par CCI Parameter which will store the returned param
 * @param nam       The (local or top-level) name the parameter shall get
 * @param val       Default value for this parameter
 * @param force_top_level_name If the given name shall be a top-level name
 */
template<typename T, cci::cnf::param_mutable_type TM>
static cci::cnf::cci_param_if<T, TM>* create_cci_param(cci::cnf::cci_param<T, TM> *owner_par, const char* nam, const T& val, const bool force_top_level_name);

/// Parameter factory function being called from the cci param constructor to construct the underlying parameter
/**
 * The implementation shall return a new parameter object.
 *
 * @param owner_par CCI Parameter which will store the returned param
 * @param nam       The (local or top-level) name the parameter shall get
 * @param val       Default value (as JSON string) for this parameter
 * @param force_top_level_name If the given name shall be a top-level name
 */
template<typename T, cci::cnf::param_mutable_type TM>
static cci::cnf::cci_param_if<T, TM>* create_cci_param(cci::cnf::cci_param<T, TM> *owner_par, const char* nam, const char* val, const bool force_top_level_name);

/// Parameter factory function that is called after construction and after adding to the broker from within the cci param constructor
/**
 * The implementation is free to use this function for initialization.
 * Note that the registration with the broker had already been done.
 *
 * @param owner_par CCI Parameter which shall be initialized
 */
template<typename T, cci::cnf::param_mutable_type TM>
static void init_cci_param(cci::cnf::cci_param<T, TM> *owner_par);

/// Parameter factory function that is called by the cci_param wrapper
/**
 * This shall never be called anywhere else that the cci_param destructor. 
 * The implementation shall delete (or free) the underlying parameter.
 *
 * @param param CCI Parameter which is destructed, whose underlying parameter can be deleted.
 */
template<class T, cci::cnf::param_mutable_type TM>
void destroy_cci_param(cci::cnf::cci_param<T, TM>* param);

__CCI_CLOSE_CONFIG_NAMESPACE__

#include "cci_param.h"

#include "cci_broker_module.h"


#endif
