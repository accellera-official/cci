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

/**
 * @file
 * @author Christian Schroeder, GreenSocs Ltd.
 * @author Mark Burton, GreenSocs Ltd.
 * @author Thomas Goodfellow, OFFIS
 * @brief Declarations of parameter factory functions
 */

CCI_OPEN_CONFIG_NAMESPACE_
  
/// Parameter factory function being called from the cci param constructor to construct the underlying parameter
/**
 * Forward declaration, to be implemented by the parameter vendor(s) for all supported data types.
 *
 * The implementation shall return a new parameter object.
 *
 * @param owner_par CCI Parameter which will store the returned param
 * @param nam The (local or top-level) name the parameter shall get
 * @param is_top_level_name If the given name shall be a top-level name
 * @param broker_accessor The broker accessor responsible for this parameter
 */
template<typename T, cci::cnf::param_mutable_type TM>
cci::cnf::cci_param_impl_if<T, TM>* create_cci_param(cci::cnf::cci_param<T, TM> *owner_par, const std::string &nam, const bool is_top_level_name, cci_cnf_broker_if* broker_accessor);

/// Parameter factory function being called from the cci param constructor to construct the underlying parameter
/**
 * Forward declaration, to be implemented by the parameter vendor(s) for all supported data types.
 *
 * The implementation shall return a new parameter object.
 *
 * @param owner_par CCI Parameter which will store the returned param
 * @param nam       The (local or top-level) name the parameter shall get
 * @param val       Default value for this parameter
 * @param is_top_level_name If the given name shall be a top-level name
 * @param broker_accessor The broker accessor responsible for this parameter
 */
template<typename T, cci::cnf::param_mutable_type TM>
cci::cnf::cci_param_impl_if<T, TM>* create_cci_param(cci::cnf::cci_param<T, TM> *owner_par, const std::string &nam, const T& val, const bool is_top_level_name, cci_cnf_broker_if* broker_accessor);

/// Parameter factory function being called from the cci param constructor to construct the underlying parameter
/**
 * Forward declaration, to be implemented by the parameter vendor(s) for all supported data types.
 *
 * The implementation shall return a new parameter object.
 *
 * @param owner_par CCI Parameter which will store the returned param
 * @param nam       The (local or top-level) name the parameter shall get
 * @param val       Default value (as JSON string) for this parameter
 * @param is_top_level_name If the given name shall be a top-level name
 * @param broker_accessor The broker accessor responsible for this parameter
 */
template<typename T, cci::cnf::param_mutable_type TM>
cci::cnf::cci_param_impl_if<T, TM>* create_cci_param(cci::cnf::cci_param<T, TM> *owner_par, const std::string &nam, const char* val, const bool is_top_level_name, cci_cnf_broker_if* broker_accessor);

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



CCI_CLOSE_CONFIG_NAMESPACE_

#endif
