// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#include "cci_params.h"

// Choose one symbol variant
#define CCI_USE_HORIZONTAL_REPITION
//#define CCI_USE_VERTICAL_REPITION
//#define CCI_USE_TEMPLATE_META_PROGRAMMING

//
// PART 2: Template types sc_uint<w>, sc_int<w>
//
// This creates symbols for standard parameter types in this library!
//

#define CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, MUTABILITY_TYPE) \
  template  cci::cnf::cci_param_impl_if<PTYPE, MUTABILITY_TYPE>*  create_cci_param (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const std::string &nam,                   const bool is_top_level_name, cci_cnf_broker_if* broker_accessor); \
  template  cci::cnf::cci_param_impl_if<PTYPE, MUTABILITY_TYPE>*  create_cci_param (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const std::string &nam, const PTYPE& val, const bool is_top_level_name, cci_cnf_broker_if* broker_accessor); \
  template  cci::cnf::cci_param_impl_if<PTYPE, MUTABILITY_TYPE>*  create_cci_param (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const std::string &nam, const char* val,  const bool is_top_level_name, cci_cnf_broker_if* broker_accessor); \
  template                                                  void  init_cci_param   (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par); \
  template                                                  void  destroy_cci_param(cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE>* param    );

#define CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(PTYPE) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::mutable_parameter) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::immutable_parameter) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::elaboration_time_parameter) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::other_parameter)



#ifdef CCI_USE_HORIZONTAL_REPITION
  

namespace cci { namespace cnf {

  // *************************************************************
  // ** Create template types using BOOST Horizontal Repetition **
  // *************************************************************

// Highest template specialization available for sc_int and sc_uint
#define SC_INT_CREATOR_MAX_SIZE 64

#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/iteration/local.hpp>

#define CREATOR_width(z, n, PTYPE)                    \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(PTYPE< BOOST_PP_INC(n) >)
BOOST_PP_REPEAT(SC_INT_CREATOR_MAX_SIZE,    CREATOR_width, sc_dt::sc_int)
BOOST_PP_REPEAT(SC_INT_CREATOR_MAX_SIZE,    CREATOR_width, sc_dt::sc_uint)
#undef CREATOR_width


} } // end namespace

#endif
#ifdef CCI_USE_VERTICAL_REPITION
  
namespace cci { namespace cnf {

  // ***********************************************************
  // ** Create template types using BOOST Vertical Repetition **
  // ***********************************************************

// Highest template specialization available for sc_int and sc_uint
// Maximum recursion depth is 256!!
#define SC_INT_CREATOR_MAX_SIZE 64
  
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/iteration/local.hpp>

#define CREATOR_width(z, n, PTYPE)                    \
CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(PTYPE< BOOST_PP_INC(n) >)
  
#define BOOST_PP_LOCAL_MACRO(n)   CREATOR_width(~, n, sc_dt::sc_int)
#define BOOST_PP_LOCAL_LIMITS     (0, SC_INT_CREATOR_MAX_SIZE - 1)
#include BOOST_PP_LOCAL_ITERATE()

#define BOOST_PP_LOCAL_MACRO(n)   CREATOR_width(~, n, sc_dt::sc_uint)
#define BOOST_PP_LOCAL_LIMITS     (0, SC_INT_CREATOR_MAX_SIZE - 1)
#include BOOST_PP_LOCAL_ITERATE()

#undef CREATOR_width
  
  
} } // end namespace

  
#endif
#ifdef CCI_USE_TEMPLATE_META_PROGRAMMING

__OPEN_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__

  // ***********************************************************
  // ** Create template types using template meta programming **
  // ***********************************************************

using namespace cci::cnf;
using namespace sc_dt;
  
#define PLACE_FUNCTION_POINTERS_T_TM(T,TM) \
  cci_param_impl_if<T<LOWER>, TM>*  (*call_create1_##T##TM)(cci_param<T<LOWER>, TM>*, const char*, const bool); \
  cci_param_impl_if<T<LOWER>, TM>*  (*call_create2_##T##TM)(cci_param<T<LOWER>, TM>*, const char*, const T<LOWER>&, const bool); \
  cci_param_impl_if<T<LOWER>, TM>*  (*call_create3_##T##TM)(cci_param<T<LOWER>, TM>*, const char*, const char*,  const bool); \
  void (*call_create4_##T##TM)(cci_param<T<LOWER>, TM>*); \
  void (*call_create5_##T##TM)(cci_param<T<LOWER>, TM>*);

#define PLACE_FUNCTION_POINTERS_T(T) \
  PLACE_FUNCTION_POINTERS_T_TM(T, mutable_parameter) \
  PLACE_FUNCTION_POINTERS_T_TM(T, immutable_parameter) \
  PLACE_FUNCTION_POINTERS_T_TM(T, elaboration_time_parameter) \
  PLACE_FUNCTION_POINTERS_T_TM(T, other_parameter)
#define PLACE_FUNCTION_POINTERS() \
  PLACE_FUNCTION_POINTERS_T(sc_int) \
  PLACE_FUNCTION_POINTERS_T(sc_uint)

#define PLACE_FUNCTION_CALLS_T_TM(T,TM) \
  call_create1_##T##TM(&create_cci_param<T<LOWER>, TM>), \
  call_create2_##T##TM(&create_cci_param<T<LOWER>, TM>), \
  call_create3_##T##TM(&create_cci_param<T<LOWER>, TM>), \
  call_create4_##T##TM(&init_cci_param<T<LOWER>, TM>), \
  call_create5_##T##TM(&destroy_cci_param<T<LOWER>, TM>), 
#define PLACE_FUNCTION_CALLS_T(T) \
  PLACE_FUNCTION_CALLS_T_TM(T, mutable_parameter) \
  PLACE_FUNCTION_CALLS_T_TM(T, immutable_parameter) \
  PLACE_FUNCTION_CALLS_T_TM(T, elaboration_time_parameter) \
  PLACE_FUNCTION_CALLS_T_TM(T, other_parameter)
#define PLACE_FUNCTION_CALLS() \
  PLACE_FUNCTION_CALLS_T(sc_int) \
  PLACE_FUNCTION_CALLS_T(sc_uint)
  
  template <int LOWER, int UPPER, bool CONT>
  struct looper_sc_u_int
  {
  };

  template <int LOWER, int UPPER> 
  struct looper_sc_u_int<LOWER, UPPER, false> 
  { 
    PLACE_FUNCTION_POINTERS()
    looper_sc_u_int() : PLACE_FUNCTION_CALLS() i(0)
    {}
    int i; 
  }; 
  
  template <int LOWER, int UPPER> 
  struct looper_sc_u_int<LOWER, UPPER, true> 
  : looper_sc_u_int<LOWER+1,UPPER,((LOWER+1)<UPPER) > 
  { 
    PLACE_FUNCTION_POINTERS()
    looper_sc_u_int() : PLACE_FUNCTION_CALLS() i(0)
    {}
    int i;
  };

  template 
  struct looper_sc_u_int<1,64,true>;
  
__CLOSE_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__  
  
#endif
  
  
