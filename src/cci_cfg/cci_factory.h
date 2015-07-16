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

#ifndef CCI_CNF_CCI_FACTORY_H_INCLUDED_
#define CCI_CNF_CCI_FACTORY_H_INCLUDED_

#include <string>
#include "cci_datatypes.h"

/**
 * @file
 * @author Christian Schroeder, GreenSocs Ltd.
 * @author Mark Burton, GreenSocs Ltd.
 * @author Thomas Goodfellow, OFFIS
 * @brief Declarations of parameter factory functions
 */

CCI_OPEN_NAMESPACE_

template<class T, param_mutable_type TM>
class cci_param;

class cci_cnf_broker_if;

/// Parameter factory function being called from the cci param constructor to construct the underlying parameter
/**
 * Forward declaration, to be implemented by the parameter vendor(s) for all supported data types.
 *
 * The implementation shall return a new parameter object.
 *
 * @param owner_par CCI Parameter which will store the returned param
 * @param name      The (local or top-level) name the parameter shall get
 * @param value     Default value for this parameter
 * @param is_top_level_name If the given name shall be a top-level name
 * @param broker_accessor The broker accessor responsible for this parameter
 */
template<typename T, param_mutable_type TM>
cci_param_impl_if* create_cci_param(cci_param<T, TM>* owner_par, const std::string& name, const T& value, const bool is_top_level_name, cci_cnf_broker_if* broker_accessor);

/// Parameter factory function being called from the cci param constructor to construct the underlying parameter
/**
 * Forward declaration, to be implemented by the parameter vendor(s) for all supported data types.
 *
 * The implementation shall return a new parameter object.
 *
 * @param owner_par CCI Parameter which will store the returned param
 * @param name      The (local or top-level) name the parameter shall get
 * @param value     Default value (as JSON string) for this parameter
 * @param is_top_level_name If the given name shall be a top-level name
 * @param broker_accessor The broker accessor responsible for this parameter
 */
template<typename T, param_mutable_type TM>
cci_param_impl_if* create_cci_param(cci_param<T, TM>* owner_par, const std::string& name, const cci_value& value, const bool is_top_level_name, cci_cnf_broker_if* broker_accessor);

CCI_CLOSE_NAMESPACE_

#endif
