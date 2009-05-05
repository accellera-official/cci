//   OSCI CCI WG
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// 
// ENDLICENSETEXT


#ifndef __CCI_PARAM_H__
#define __CCI_PARAM_H__

#include <string>
#include <iostream>
#include <sstream>

#include <systemc>
#include "cci_paramcallbadapt.h"
#include "cci_cnf_api.h"
#include "cci_param_base.h"
#include "cci_param_t.h"
//#include "cci_param_operator_macros.h"

namespace cci {


/* Constructors with comments:
protected:
  /// Typedef for this specialized class.
  typedef cci_param<val_type> my_type;

  using cci_param_t<val_type>::my_value;
  using cci_param_t<val_type>::m_api;
  using cci_param_t<val_type>::m_par_name;

  using cci_param_t<val_type>::convertStringToValue;
private:
  /// Not allowed constructor!
  explicit cci_param(const val_type &val) { sc_assert(false); }

public:
  // Explicit constructors to avoid implicit construction of parameters.

  /// Empty constructor. Name will be set in base
  explicit cci_param() : cci_param_t<val_type>(false, NULL, true) { cci_param_t<val_type>::init();  }

  /// Constructor with (local/hierarchical) name.
  explicit cci_param(const std::string &nam) : cci_param_t<val_type>(nam        , false, NULL, true) { cci_param_t<val_type>::init(); }
  explicit cci_param(const char *nam       ) : cci_param_t<val_type>(string(nam), false, NULL, true) { cci_param_t<val_type>::init(); }

  /// Constructor with (local/hierarchical) name and string representation of initial value.
  explicit cci_param(const std::string &nam, const std::string &val, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, NULL, true ) { cci_param_t<val_type>::init(convertStringToValue(val));         } 
  explicit cci_param(const char *nam,        const char *val       , const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, NULL, true ) { cci_param_t<val_type>::init(convertStringToValue(string(val))); } 
  explicit cci_param(const std::string &nam, const char *val       , const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, NULL, true ) { cci_param_t<val_type>::init(convertStringToValue(string(val))); } 
  explicit cci_param(const char *nam,        const std::string &val, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, NULL, true ) { cci_param_t<val_type>::init(convertStringToValue(val));         } 

  /// Constructor with (local/hierarchical) name and initial value.
  explicit cci_param(const std::string &nam, const val_type &val, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, NULL, true )  { cci_param_t<val_type>::init(val); }    
  explicit cci_param(const char *nam,        const val_type &val, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, NULL, true )  { cci_param_t<val_type>::init(val); }    

  // Constructors with parent array
  explicit cci_param(                                                cci_param_array* parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(             force_top_level_name, parent_array, true) { cci_param_t<val_type>::init(); }
  explicit cci_param(const std::string &nam                        , cci_param_array* parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, parent_array, true) { cci_param_t<val_type>::init(); } 
  explicit cci_param(const char *nam                               , cci_param_array* parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, parent_array, true) { cci_param_t<val_type>::init(); } 
  explicit cci_param(const std::string &nam, const std::string &val, cci_param_array* parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, parent_array, true) { cci_param_t<val_type>::init(convertStringToValue(val));         } 
  explicit cci_param(const char *nam,        const char *val       , cci_param_array* parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, parent_array, true) { cci_param_t<val_type>::init(convertStringToValue(string(val))); } 
  explicit cci_param(const std::string &nam, const char *val       , cci_param_array* parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, parent_array, true) { cci_param_t<val_type>::init(convertStringToValue(string(val))); } 
  explicit cci_param(const char *nam,        const std::string &val, cci_param_array* parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, parent_array, true) { cci_param_t<val_type>::init(convertStringToValue(val));         } 
  explicit cci_param(const std::string &nam, const val_type &val   , cci_param_array* parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, parent_array, true) { cci_param_t<val_type>::init(val); }    
  explicit cci_param(const char *nam,        const val_type &val   , cci_param_array* parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, parent_array, true) { cci_param_t<val_type>::init(val); }    
 
  // Constructors with parent array
  explicit cci_param(                                                cci_param_array& parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(             force_top_level_name, &parent_array, true) { cci_param_t<val_type>::init(); }
  explicit cci_param(const std::string &nam                        , cci_param_array& parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, &parent_array, true) { cci_param_t<val_type>::init(); } 
  explicit cci_param(const char *nam                               , cci_param_array& parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, &parent_array, true) { cci_param_t<val_type>::init(); } 
  explicit cci_param(const std::string &nam, const std::string &val, cci_param_array& parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, &parent_array, true) { cci_param_t<val_type>::init(convertStringToValue(val));         } 
  explicit cci_param(const char *nam,        const char *val       , cci_param_array& parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, &parent_array, true) { cci_param_t<val_type>::init(convertStringToValue(string(val))); } 
  explicit cci_param(const std::string &nam, const char *val       , cci_param_array& parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, &parent_array, true) { cci_param_t<val_type>::init(convertStringToValue(string(val))); } 
  explicit cci_param(const char *nam,        const std::string &val, cci_param_array& parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, &parent_array, true) { cci_param_t<val_type>::init(convertStringToValue(val));         } 
  explicit cci_param(const std::string &nam, const val_type &val   , cci_param_array& parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, &parent_array, true) { cci_param_t<val_type>::init(val); }    
  explicit cci_param(const char *nam,        const val_type &val   , cci_param_array& parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, &parent_array, true) { cci_param_t<val_type>::init(val); }    

  // Constructors with register_at_db bool
  explicit cci_param(const std::string &nam, const std::string &val, cci_param_array* parent_array, const bool force_top_level_name, const bool register_at_db) : cci_param_t<val_type>(nam, force_top_level_name,  parent_array, register_at_db) { cci_param_t<val_type>::init(convertStringToValue(val));         } 
  explicit cci_param(const std::string &nam, const std::string &val, cci_param_array& parent_array, const bool force_top_level_name, const bool register_at_db) : cci_param_t<val_type>(nam, force_top_level_name, &parent_array, register_at_db) { cci_param_t<val_type>::init(convertStringToValue(val));         } 

  /// Destructor
  virtual ~cci_param() { cci_param_t<val_type>::destruct_cci_param(); }

  /// To resolve the correct = operator
  using cci_param_t<val_type>::operator =;
  // TODO: other operators??

  using cci_param_t<val_type>::cci_param_base::name;
  using cci_param_t<val_type>::setString;
  using cci_param_t<val_type>::getString;
  using cci_param_t<val_type>::setValue;
  using cci_param_t<val_type>::getValue;

  /// Overloads cci_param_t<T>::deserialize in cci_param_t<T>
  const bool deserialize(val_type &target_val, const std::string& str) {
    return static_deserialize(target_val, str);
  }
 
*/

#define cci_PARAM_HEAD                                                  \
protected:                                                             \
  typedef cci_param<val_type> my_type;                                  \
  using cci_param_t<val_type>::my_value;                                \
  using cci_param_t<val_type>::m_api;                                   \
  using cci_param_t<val_type>::m_par_name;                              \
  using cci_param_t<val_type>::convertStringToValue;                    \
private:                                                               \
  explicit cci_param(const val_type &val) { sc_assert(false); }         \
                                                                       \
public:                                                                \
  explicit cci_param() : cci_param_t<val_type>(false, NULL, true) { cci_param_t<val_type>::init();  } \
  \
  explicit cci_param(const std::string &nam) : cci_param_t<val_type>(nam        , false, NULL, true) { cci_param_t<val_type>::init(); } \
  explicit cci_param(const char *nam       ) : cci_param_t<val_type>(string(nam), false, NULL, true) { cci_param_t<val_type>::init(); } \
  \
  explicit cci_param(const std::string &nam, const std::string &val, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, NULL, true ) { cci_param_t<val_type>::init(convertStringToValue(val));         } \
  explicit cci_param(const char *nam,        const char *val       , const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, NULL, true ) { cci_param_t<val_type>::init(convertStringToValue(string(val))); } \
  explicit cci_param(const std::string &nam, const char *val       , const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, NULL, true ) { cci_param_t<val_type>::init(convertStringToValue(string(val))); } \
  explicit cci_param(const char *nam,        const std::string &val, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, NULL, true ) { cci_param_t<val_type>::init(convertStringToValue(val));         } \
  \
  explicit cci_param(const std::string &nam, const val_type &val, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, NULL, true )  { cci_param_t<val_type>::init(val); }    \
  explicit cci_param(const char *nam,        const val_type &val, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, NULL, true )  { cci_param_t<val_type>::init(val); }    \
  \
  explicit cci_param(                                                cci_param_array* parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(             force_top_level_name, parent_array, true) { cci_param_t<val_type>::init(); } \
  explicit cci_param(const std::string &nam                        , cci_param_array* parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, parent_array, true) { cci_param_t<val_type>::init(); } \
  explicit cci_param(const char *nam                               , cci_param_array* parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, parent_array, true) { cci_param_t<val_type>::init(); } \
  explicit cci_param(const std::string &nam, const std::string &val, cci_param_array* parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, parent_array, true) { cci_param_t<val_type>::init(convertStringToValue(val));         } \
  explicit cci_param(const char *nam,        const char *val       , cci_param_array* parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, parent_array, true) { cci_param_t<val_type>::init(convertStringToValue(string(val))); } \
  explicit cci_param(const std::string &nam, const char *val       , cci_param_array* parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, parent_array, true) { cci_param_t<val_type>::init(convertStringToValue(string(val))); } \
  explicit cci_param(const char *nam,        const std::string &val, cci_param_array* parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, parent_array, true) { cci_param_t<val_type>::init(convertStringToValue(val));         } \
  explicit cci_param(const std::string &nam, const val_type &val   , cci_param_array* parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, parent_array, true) { cci_param_t<val_type>::init(val); }    \
  explicit cci_param(const char *nam,        const val_type &val   , cci_param_array* parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, parent_array, true) { cci_param_t<val_type>::init(val); }    \
  \
  explicit cci_param(                                                cci_param_array& parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(             force_top_level_name, &parent_array, true) { cci_param_t<val_type>::init(); } \
  explicit cci_param(const std::string &nam                        , cci_param_array& parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, &parent_array, true) { cci_param_t<val_type>::init(); } \
  explicit cci_param(const char *nam                               , cci_param_array& parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, &parent_array, true) { cci_param_t<val_type>::init(); } \
  explicit cci_param(const std::string &nam, const std::string &val, cci_param_array& parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, &parent_array, true) { cci_param_t<val_type>::init(convertStringToValue(val));         } \
  explicit cci_param(const char *nam,        const char *val       , cci_param_array& parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, &parent_array, true) { cci_param_t<val_type>::init(convertStringToValue(string(val))); } \
  explicit cci_param(const std::string &nam, const char *val       , cci_param_array& parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, &parent_array, true) { cci_param_t<val_type>::init(convertStringToValue(string(val))); } \
  explicit cci_param(const char *nam,        const std::string &val, cci_param_array& parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, &parent_array, true) { cci_param_t<val_type>::init(convertStringToValue(val));         } \
  explicit cci_param(const std::string &nam, const val_type &val   , cci_param_array& parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(nam        , force_top_level_name, &parent_array, true) { cci_param_t<val_type>::init(val); }    \
  explicit cci_param(const char *nam,        const val_type &val   , cci_param_array& parent_array, const bool force_top_level_name = false) : cci_param_t<val_type>(string(nam), force_top_level_name, &parent_array, true) { cci_param_t<val_type>::init(val); }    \
  \
  explicit cci_param(const std::string &nam, const std::string &val, cci_param_array* parent_array, const bool force_top_level_name, const bool register_at_db) : cci_param_t<val_type>(nam, force_top_level_name,  parent_array, register_at_db) { cci_param_t<val_type>::init(convertStringToValue(val));         } \
  explicit cci_param(const std::string &nam, const std::string &val, cci_param_array& parent_array, const bool force_top_level_name, const bool register_at_db) : cci_param_t<val_type>(nam, force_top_level_name, &parent_array, register_at_db) { cci_param_t<val_type>::init(convertStringToValue(val));         } \
  \
  virtual ~cci_param() { cci_param_t<val_type>::destruct_cci_param(); }              \
                                                                                  \
                                                                                  \
  const bool deserialize(val_type &target_val, const std::string& str) {          \
    return static_deserialize(target_val, str);                                   \
  }                                                                               \
                                                                                  \
  using cci_param_t<val_type>::operator =;                                         \
                                                                                  \
  using cci_param_t<val_type>::cci_param_base::name;                                \
  using cci_param_t<val_type>::setString;                                          \
  using cci_param_t<val_type>::getString;                                          \
  using cci_param_t<val_type>::setValue;                                           \
  using cci_param_t<val_type>::getValue                                           


/// The parameters, cci_param class, templated.
/**
 * Each explicitely supported data type has a template specialisation.
 * See specialisations.
 * All data types for which no specialization is existing, will use this class.
 */
template<typename T>
class cci_param 
: public cci_param_t<T>
{
protected:
  /// Typedef for the value.
  typedef T val_type;
  
  /// Typedef for this specialized class.
  typedef cci_param<val_type> my_type;
  
  using cci_param_t<val_type>::my_value;
  using cci_param_t<val_type>::m_api;
  using cci_param_t<val_type>::m_par_name;
  
  using cci_param_t<val_type>::convertStringToValue;
private:
  /// Not allowed constructor!
  explicit cci_param(const val_type &val) { assert(false); }
  
public:
  // Explicit constructors to avoid implicit construction of parameters.
  
  /// Empty constructor. Name will be set in base
  explicit cci_param() : cci_param_t<val_type>(false, NULL, true) { cci_param_t<val_type>::init();  }
  
  /// Constructor with (local/hierarchical) name.
  explicit cci_param(const std::string &nam) : cci_param_t<val_type>(nam             , false, true) { cci_param_t<val_type>::init(); }
  explicit cci_param(const char *nam       ) : cci_param_t<val_type>(std::string(nam), false, true) { cci_param_t<val_type>::init(); }
  
  /// Constructor with (local/hierarchical) name and string representation of initial value.
  explicit cci_param(const std::string &nam, const std::string &val, const bool force_top_level_name = false) : cci_param_t<val_type>(nam             , force_top_level_name, true ) { cci_param_t<val_type>::init(convertStringToValue(val));         } 
  explicit cci_param(const char *nam,        const char *val       , const bool force_top_level_name = false) : cci_param_t<val_type>(std::string(nam), force_top_level_name, true ) { cci_param_t<val_type>::init(convertStringToValue(std::string(val))); } 
  explicit cci_param(const std::string &nam, const char *val       , const bool force_top_level_name = false) : cci_param_t<val_type>(nam             , force_top_level_name, true ) { cci_param_t<val_type>::init(convertStringToValue(std::string(val))); } 
  explicit cci_param(const char *nam,        const std::string &val, const bool force_top_level_name = false) : cci_param_t<val_type>(std::string(nam), force_top_level_name, true ) { cci_param_t<val_type>::init(convertStringToValue(val));         } 
  
  /// Constructor with (local/hierarchical) name and initial value.
  explicit cci_param(const std::string &nam, const val_type &val, const bool force_top_level_name = false) : cci_param_t<val_type>(nam             , force_top_level_name, true )  { cci_param_t<val_type>::init(val); }    
  explicit cci_param(const char *nam,        const val_type &val, const bool force_top_level_name = false) : cci_param_t<val_type>(std::string(nam), force_top_level_name, true )  { cci_param_t<val_type>::init(val); }    
  
  // Constructors with register_at_db bool
  explicit cci_param(const std::string &nam, const std::string &val, const bool force_top_level_name, const bool register_at_db) : cci_param_t<val_type>(nam, force_top_level_name, register_at_db) { cci_param_t<val_type>::init(convertStringToValue(val));         } 
  
  /// Destructor
  virtual ~cci_param() { cci_param_t<val_type>::destruct_cci_param(); }
  
  /// To resolve the correct = operator
  using cci_param_t<val_type>::operator =;
  // TODO: other operators??
  
  //using cci_param_t<val_type>::cci_param_base::name;
  using cci_param_t<val_type>::setString;
  using cci_param_t<val_type>::getString;
  using cci_param_t<val_type>::setValue;
  using cci_param_t<val_type>::getValue;
  
  /// Overloads cci_param_t<T>::deserialize in cci_param_t<T>
  const bool deserialize(val_type &target_val, const std::string& str) {
    return static_deserialize(target_val, str);
  }
  
  
  /// Overloads cci_param_t<T>::convertValueToString
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
    //cout << "cci_param_t<T>::string_to_value("<<str<<", "<<val<<"), set val to: ";
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
  
  template<class T> bool operator == (cci_param<T> &p1, cci_param<T>& p2) {
    return p1.getValue() == p2.getValue();
  }

  template<class T> bool operator == (cci_param<T> &p1, T& p2) {
    return p1.getValue() == p2;
  }

  template<class T> bool operator == (T& p1, cci_param<T>& p2) {
    return p1 == p2.getValue();
  }
  
  
// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// NATIVE DATA TYPES ///////////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

// TODO #include "cci_param_native.hpp"
  
  
} // end namespace cci

#endif
