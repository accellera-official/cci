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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT




#ifndef __CCI_CONFIG_H__
#define __CCI_CONFIG_H__


#include <systemc>

//
//  This is the main include file for the config part of the CCI.
//  The top-level include file is cci.h
//

//#define CCI_CNF_VERBOSE

#ifdef CCI_CNF_VERBOSE
# define CCI_CNF_DUMP(msg) { std::cout<<msg<<std::endl; } 
#else
# define CCI_CNF_DUMP(msg) {  } 
#endif

/// Namespace for OSCI Configuration, Control & Inspection (CCI) standard, Config part
#define __CCI_OPEN_CONFIG_NAMESPACE__  namespace cci { namespace cnf {
#define __CCI_CLOSE_CONFIG_NAMESPACE__ } }

/// Main CCI namespace
/**
 * This is the OSCI CCI namespace
 */
namespace cci {
  /// Namespace for CCI configuration part
  /**
   * This is the OSCI CCI config namespace
   */
  namespace cnf {
  }
}

#include "cci_error_types.h"
#include "cci_datatypes.h"
#include "cci_originator.h"

__CCI_OPEN_CONFIG_NAMESPACE__

  // forward declaration
  class cci_cnf_broker_if;

  /// Returns a configuration broker accessor instance responsible for handling the requests of the given originator 
  /**
   * Forward declaration, to be implemented by the tool (broker vendor).
   *
   * @param originator Originator points to or names the object or module who wants access the config broker; might be NULL for global default broker to be returned
   * @return Configuration broker accessor.
   */
  cci_cnf_broker_if* get_cnf_broker_instance(cci_originator* originator);
  cci_cnf_broker_if* get_cnf_broker_instance(const cci_originator& originator);

__CCI_CLOSE_CONFIG_NAMESPACE__

#include "cci_callbacks.h"
#include "cci_cnf_broker_if.h"

#include "cci_base_param_impl_if.h"
#include "cci_base_param.h"

#include "cci_param_impl_if.h"

__CCI_OPEN_CONFIG_NAMESPACE__
  
/// Parameter factory function being called from the cci param constructor to construct the underlying parameter
/**
 * Forward declaration, to be implemented by the parameter vendor(s) for all supported data types.
 *
 * The implementation shall return a new parameter object.
 *
 * @param owner_par CCI Parameter which will store the returned param
 * @param nam The (local or top-level) name the parameter shall get
 * @param force_top_level_name If the given name shall be a top-level name
 */
template<typename T, cci::cnf::param_mutable_type TM>
cci::cnf::cci_param_impl_if<T, TM>* create_cci_param(cci::cnf::cci_param<T, TM> *owner_par, const char* nam, const bool force_top_level_name);

/// Parameter factory function being called from the cci param constructor to construct the underlying parameter
/**
 * Forward declaration, to be implemented by the parameter vendor(s) for all supported data types.
 *
 * The implementation shall return a new parameter object.
 *
 * @param owner_par CCI Parameter which will store the returned param
 * @param nam       The (local or top-level) name the parameter shall get
 * @param val       Default value for this parameter
 * @param force_top_level_name If the given name shall be a top-level name
 */
template<typename T, cci::cnf::param_mutable_type TM>
cci::cnf::cci_param_impl_if<T, TM>* create_cci_param(cci::cnf::cci_param<T, TM> *owner_par, const char* nam, const T& val, const bool force_top_level_name);

/// Parameter factory function being called from the cci param constructor to construct the underlying parameter
/**
 * Forward declaration, to be implemented by the parameter vendor(s) for all supported data types.
 *
 * The implementation shall return a new parameter object.
 *
 * @param owner_par CCI Parameter which will store the returned param
 * @param nam       The (local or top-level) name the parameter shall get
 * @param val       Default value (as JSON string) for this parameter
 * @param force_top_level_name If the given name shall be a top-level name
 */
template<typename T, cci::cnf::param_mutable_type TM>
cci::cnf::cci_param_impl_if<T, TM>* create_cci_param(cci::cnf::cci_param<T, TM> *owner_par, const char* nam, const char* val, const bool force_top_level_name);

/// Parameter factory function that is called after construction and after adding to the broker from within the cci param constructor
/**
 * Forward declaration, to be implemented by the parameter vendor(s) for all supported data types.
 *
 * The implementation is free to use this function for initialization.
 * Note that the registration with the broker had already been done.
 *
 * @param owner_par CCI Parameter which shall be initialized
 */
template<typename T, cci::cnf::param_mutable_type TM>
void init_cci_param(cci::cnf::cci_param<T, TM> *owner_par);

/// Parameter factory function that is called by the cci_param wrapper
/**
 * Forward declaration, to be implemented by the parameter vendor(s) for all supported data types.
 *
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
