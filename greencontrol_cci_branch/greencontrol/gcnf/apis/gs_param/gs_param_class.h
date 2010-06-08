//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2009 : GreenSocs Ltd
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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doxygen comments

#ifndef __GS_PARAM_INTERNAL_H__
#define __GS_PARAM_INTERNAL_H__

#include <string>
#include <iostream>
#include <sstream>
#include <boost/tokenizer.hpp> // for parameter array!

#include "gs_param_t.h"
#include "gs_param_operator_macros.h"


namespace gs {
namespace cnf {


/* Constructors with comments:
protected:
  /// Typedef for this specialized class.
  typedef gs_param<val_type> my_type;

  using gs_param_t<val_type>::my_value;
  using gs_param_t<val_type>::m_api;
  using gs_param_t<val_type>::m_par_name;

  using gs_param_t<val_type>::convertStringToValue;
private:
  /// Not allowed constructor!
  explicit gs_param(const val_type &val) { sc_assert(false); }

public:
  // Explicit constructors to avoid implicit construction of parameters.

  /// Empty constructor. Name will be set in base
  explicit gs_param() : gs_param_t<val_type>(false, NULL, true) { gs_param_t<val_type>::init();  }

  /// Constructor with (local/hierarchical) name.
  explicit gs_param(const std::string &nam) : gs_param_t<val_type>(nam        , false, NULL, true) { gs_param_t<val_type>::init(); }
  explicit gs_param(const char *nam       ) : gs_param_t<val_type>(string(nam), false, NULL, true) { gs_param_t<val_type>::init(); }

  /// Constructor with (local/hierarchical) name and string representation of initial value.
  explicit gs_param(const std::string &nam, const std::string &val, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, NULL, true ) { gs_param_t<val_type>::init(convertStringToValue(val));         } 
  explicit gs_param(const char *nam,        const char *val       , const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, NULL, true ) { gs_param_t<val_type>::init(convertStringToValue(string(val))); } 
  explicit gs_param(const std::string &nam, const char *val       , const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, NULL, true ) { gs_param_t<val_type>::init(convertStringToValue(string(val))); } 
  explicit gs_param(const char *nam,        const std::string &val, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, NULL, true ) { gs_param_t<val_type>::init(convertStringToValue(val));         } 

  /// Constructor with (local/hierarchical) name and initial value.
  explicit gs_param(const std::string &nam, const val_type &val, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, NULL, true )  { gs_param_t<val_type>::init(val); }    
  explicit gs_param(const char *nam,        const val_type &val, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, NULL, true )  { gs_param_t<val_type>::init(val); }    

  // Constructors with parent array
  explicit gs_param(                                                gs_param_array* parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(             force_top_level_name, parent_array, true) { gs_param_t<val_type>::init(); }
  explicit gs_param(const std::string &nam                        , gs_param_array* parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, parent_array, true) { gs_param_t<val_type>::init(); } 
  explicit gs_param(const char *nam                               , gs_param_array* parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, parent_array, true) { gs_param_t<val_type>::init(); } 
  explicit gs_param(const std::string &nam, const std::string &val, gs_param_array* parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(val));         } 
  explicit gs_param(const char *nam,        const char *val       , gs_param_array* parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(string(val))); } 
  explicit gs_param(const std::string &nam, const char *val       , gs_param_array* parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(string(val))); } 
  explicit gs_param(const char *nam,        const std::string &val, gs_param_array* parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(val));         } 
  explicit gs_param(const std::string &nam, const val_type &val   , gs_param_array* parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, parent_array, true) { gs_param_t<val_type>::init(val); }    
  explicit gs_param(const char *nam,        const val_type &val   , gs_param_array* parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, parent_array, true) { gs_param_t<val_type>::init(val); }    
 
  // Constructors with parent array
  explicit gs_param(                                                gs_param_array& parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(             force_top_level_name, &parent_array, true) { gs_param_t<val_type>::init(); }
  explicit gs_param(const std::string &nam                        , gs_param_array& parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, &parent_array, true) { gs_param_t<val_type>::init(); } 
  explicit gs_param(const char *nam                               , gs_param_array& parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, &parent_array, true) { gs_param_t<val_type>::init(); } 
  explicit gs_param(const std::string &nam, const std::string &val, gs_param_array& parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, &parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(val));         } 
  explicit gs_param(const char *nam,        const char *val       , gs_param_array& parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, &parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(string(val))); } 
  explicit gs_param(const std::string &nam, const char *val       , gs_param_array& parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, &parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(string(val))); } 
  explicit gs_param(const char *nam,        const std::string &val, gs_param_array& parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, &parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(val));         } 
  explicit gs_param(const std::string &nam, const val_type &val   , gs_param_array& parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, &parent_array, true) { gs_param_t<val_type>::init(val); }    
  explicit gs_param(const char *nam,        const val_type &val   , gs_param_array& parent_array, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, &parent_array, true) { gs_param_t<val_type>::init(val); }    

  // Constructors with register_at_db bool
  explicit gs_param(const std::string &nam, const std::string &val, gs_param_array* parent_array, const bool force_top_level_name, const bool register_at_db) : gs_param_t<val_type>(nam, force_top_level_name,  parent_array, register_at_db) { gs_param_t<val_type>::init(convertStringToValue(val));         } 
  explicit gs_param(const std::string &nam, const std::string &val, gs_param_array& parent_array, const bool force_top_level_name, const bool register_at_db) : gs_param_t<val_type>(nam, force_top_level_name, &parent_array, register_at_db) { gs_param_t<val_type>::init(convertStringToValue(val));         } 

  /// Destructor
  virtual ~gs_param() { gs_param_t<val_type>::destruct_gs_param(); }

  /// To resolve the correct = operator
  using gs_param_t<val_type>::operator =;
  // TODO: other operators??

  /// Overloads gs_param_t<T>::deserialize in gs_param_t<T>
  const bool deserialize(val_type &target_val, const std::string& str) {          
#ifdef GCNF_ENABLE_GS_PARAM_LOCK
    // Check the lock!
    if (gs_param_base::m_locked) {                                                
      GS_PARAM_DUMP("parameter is locked!");                                      
      SC_REPORT_INFO(GCNF_SC_REPORTER(this->getName()), "parameter is locked!");
      return false;                                                               
    }                                                                             
#endif
    return static_deserialize(target_val, str);                                   
  }                                                      

  using gs_param_t<val_type>::name;
  using gs_param_t<val_type>::setString;
  using gs_param_t<val_type>::getString;
  using gs_param_t<val_type>::setValue;
  using gs_param_t<val_type>::getValue;
 
 
*/
  
#define GS_PARAM_HEAD                                                  \
protected:                                                             \
  typedef gs_param<val_type> my_type;                                  \
  using gs_param_t<val_type>::my_value;                                \
  using gs_param_t<val_type>::m_api;                                   \
  using gs_param_t<val_type>::m_par_name;                              \
  using gs_param_t<val_type>::convertStringToValue;                    \
private:                                                               \
  explicit gs_param(const val_type &val) { sc_assert(false); }         \
                                                                       \
public:                                                                \
  explicit gs_param() : gs_param_t<val_type>(false, NULL, true) { gs_param_t<val_type>::init();  } \
  \
  explicit gs_param(const std::string &nam) : gs_param_t<val_type>(nam        , false, NULL, true) { gs_param_t<val_type>::init(); } \
  explicit gs_param(const char *nam       ) : gs_param_t<val_type>(string(nam), false, NULL, true) { gs_param_t<val_type>::init(); } \
  \
  explicit gs_param(const std::string &nam, const std::string &val, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, NULL, true ) { gs_param_t<val_type>::init(convertStringToValue(val));         } \
  explicit gs_param(const char *nam,        const char *val       , const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, NULL, true ) { gs_param_t<val_type>::init(convertStringToValue(string(val))); } \
  explicit gs_param(const std::string &nam, const char *val       , const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, NULL, true ) { gs_param_t<val_type>::init(convertStringToValue(string(val))); } \
  explicit gs_param(const char *nam,        const std::string &val, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, NULL, true ) { gs_param_t<val_type>::init(convertStringToValue(val));         } \
  \
  explicit gs_param(const std::string &nam, const val_type &val, const bool force_top_level_name = false) : gs_param_t<val_type>(nam        , force_top_level_name, NULL, true )  { gs_param_t<val_type>::init(val); }    \
  explicit gs_param(const char *nam,        const val_type &val, const bool force_top_level_name = false) : gs_param_t<val_type>(string(nam), force_top_level_name, NULL, true )  { gs_param_t<val_type>::init(val); }    \
  \
  explicit gs_param(                                                gs_param_array* parent_array) : gs_param_t<val_type>(             false, parent_array, true) { gs_param_t<val_type>::init(); } \
  explicit gs_param(const std::string &nam                        , gs_param_array* parent_array) : gs_param_t<val_type>(nam        , false, parent_array, true) { gs_param_t<val_type>::init(); } \
  explicit gs_param(const char *nam                               , gs_param_array* parent_array) : gs_param_t<val_type>(string(nam), false, parent_array, true) { gs_param_t<val_type>::init(); } \
  explicit gs_param(const std::string &nam, const std::string &val, gs_param_array* parent_array) : gs_param_t<val_type>(nam        , false, parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(val));         } \
  explicit gs_param(const char *nam,        const char *val       , gs_param_array* parent_array) : gs_param_t<val_type>(string(nam), false, parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(string(val))); } \
  explicit gs_param(const std::string &nam, const char *val       , gs_param_array* parent_array) : gs_param_t<val_type>(nam        , false, parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(string(val))); } \
  explicit gs_param(const char *nam,        const std::string &val, gs_param_array* parent_array) : gs_param_t<val_type>(string(nam), false, parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(val));         } \
  explicit gs_param(const std::string &nam, const val_type &val   , gs_param_array* parent_array) : gs_param_t<val_type>(nam        , false, parent_array, true) { gs_param_t<val_type>::init(val); }    \
  explicit gs_param(const char *nam,        const val_type &val   , gs_param_array* parent_array) : gs_param_t<val_type>(string(nam), false, parent_array, true) { gs_param_t<val_type>::init(val); }    \
  \
  explicit gs_param(                                                gs_param_array& parent_array) : gs_param_t<val_type>(             false, &parent_array, true) { gs_param_t<val_type>::init(); } \
  explicit gs_param(const std::string &nam                        , gs_param_array& parent_array) : gs_param_t<val_type>(nam        , false, &parent_array, true) { gs_param_t<val_type>::init(); } \
  explicit gs_param(const char *nam                               , gs_param_array& parent_array) : gs_param_t<val_type>(string(nam), false, &parent_array, true) { gs_param_t<val_type>::init(); } \
  explicit gs_param(const std::string &nam, const std::string &val, gs_param_array& parent_array) : gs_param_t<val_type>(nam        , false, &parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(val));         } \
  explicit gs_param(const char *nam,        const char *val       , gs_param_array& parent_array) : gs_param_t<val_type>(string(nam), false, &parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(string(val))); } \
  explicit gs_param(const std::string &nam, const char *val       , gs_param_array& parent_array) : gs_param_t<val_type>(nam        , false, &parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(string(val))); } \
  explicit gs_param(const char *nam,        const std::string &val, gs_param_array& parent_array) : gs_param_t<val_type>(string(nam), false, &parent_array, true) { gs_param_t<val_type>::init(convertStringToValue(val));         } \
  explicit gs_param(const std::string &nam, const val_type &val   , gs_param_array& parent_array) : gs_param_t<val_type>(nam        , false, &parent_array, true) { gs_param_t<val_type>::init(val); }    \
  explicit gs_param(const char *nam,        const val_type &val   , gs_param_array& parent_array) : gs_param_t<val_type>(string(nam), false, &parent_array, true) { gs_param_t<val_type>::init(val); }    \
  \
  explicit gs_param(const std::string &nam, const std::string &val, gs_param_array* parent_array, const bool force_top_level_name, const bool register_at_db) : gs_param_t<val_type>(nam, force_top_level_name,  parent_array, register_at_db) { gs_param_t<val_type>::init(convertStringToValue(val));         } \
  explicit gs_param(const std::string &nam, const std::string &val, gs_param_array& parent_array, const bool force_top_level_name, const bool register_at_db) : gs_param_t<val_type>(nam, force_top_level_name, &parent_array, register_at_db) { gs_param_t<val_type>::init(convertStringToValue(val));         } \
  \
  virtual ~gs_param() { gs_param_t<val_type>::destruct_gs_param(); }              \
                                                                                  \
                                                                                  \
  const bool deserialize(val_type &target_val, const std::string& str) {          \
    GCNF_ENABLE_GS_PARAM_LOCK_GUARD( \
    if (m_locked) {                                                \
      GS_PARAM_DUMP("parameter is locked!");                                      \
      SC_REPORT_INFO(GCNF_SC_REPORTER(this->getName()), "parameter is locked!");        \
      return false;                                                               \
    } \
    ) \
    return static_deserialize(target_val, str);                                   \
  }                                                                               \
                                                                                  \
  using gs_param_t<val_type>::operator =;                                         \
                                                                                  \
  using gs_param_t<val_type>::name;                                \
  using gs_param_t<val_type>::setString;                                          \
  using gs_param_t<val_type>::getString;                                          \
  using gs_param_t<val_type>::setValue;                                           \
  using gs_param_t<val_type>::getValue                                                                                     


/// The parameters, gs_param class, templated.
/**
 * Each explicitely supported data type has a template specialisation.
 * See specialisations.
 * All data types for which no specialization is existing, will use this class.
 */
template<typename T>
class gs_param 
: public gs_param_t<T>
{
  /// Typedef for the value.
  typedef T val_type;
  
  using gs_param_t<val_type>::m_locked;
  using gs_param_t<val_type>::m_lock_pwd;
  
public:
  GS_PARAM_HEAD;

  /// Overloads gs_param_t<T>::convertValueToString
  std::string convertValueToString(const val_type &val) const {
    std::ostringstream ss;
    ss << val;
    return ss.str();
  }

  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  /**
   * Does not change the target_val if str == ""
   */
  inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
    //cout << "gs_param_t<T>::string_to_value("<<str<<", "<<val<<"), set val to: ";
    std::istringstream ss(str);
    val_type tmp;
    if (str.length() == 0) {
      return true;
    }
    ss >> tmp;
    // no conversion error
    if (ss.eof() && !ss.fail() && !ss.bad()) {
      target_val = tmp;
    } 
    else {
      std::stringstream ess;
      ess << "Conversion error: '" << str << "'";
      SC_REPORT_WARNING("deserialize", ess.str().c_str());
      return false;
    }
    return true;
  }

};
  
  template<class T> bool operator == (gs_param<T> &p1, gs_param<T>& p2) {
    return p1.getValue() == p2.getValue();
  }

  template<class T> bool operator == (gs_param<T> &p1, T& p2) {
    return p1.getValue() == p2;
  }

  template<class T> bool operator == (T& p1, gs_param<T>& p2) {
    return p1 == p2.getValue();
  }
  
// for compatibility
#define gcnf_param gs_param;

  
// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// NATIVE DATA TYPES ///////////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

#include "gs_param_native.hpp"
  


// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// SystemC DATA TYPES //////////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //
  
#include "gs_param_systemc.hpp"
  
  
  
// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// SystemC DATA TYPES //////////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //
  
#include "gs_param_array.hpp"
  
  
  
// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// OTHER SPECIAL TYPES /////////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //
  
#include "gs_param_specials.hpp"

// gs_param<sc_attribute<T> >
#include "gs_param_sc_attribute.hpp"  

// gs_param<sc_attribute<std::string> >
#define GCNF_SC_ATTRIBUTE_STRING_SPECIALIZATION
#include "gs_param_sc_attribute.hpp"  
  
  
} // end namespace cnf
using cnf::gs_param; // make gs_param available in gs namespace
} // end namespace gs

#endif
