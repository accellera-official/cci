//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
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

#ifndef __SCML_OPERATOR_MACROS_H__
#define __SCML_PARAM_OPERATOR_MACROS_H__

// /////////////////////////////////////////////////////////////////// //
// This file defines macros for the template specializations of gs_param

// Macros:
// SPECIALISATIONS_EQUAL_OPERATORS
// SPECIALISATIONS_INCREMENT_OPERATORS
// SPECIALISATIONS_ARITHMETIC_OPERATORS
// SPECIALISATIONS_BINARY_OPERATORS


// Operator = with my_type
// Operator = with val_type
#define SPECIALISATIONS_EQUAL_OPERATORS                      \
  my_type& operator= (const scml_property<val_type>& v) {    \
    m_api->setParam( m_full_par_name, (val_type)*const_cast<scml_property<val_type>*>(&v) );       \
    return *this;                                            \
  }                                                          \
                                                             \
  my_type& operator= (val_type v) {                          \
    m_api->setParam( m_full_par_name, v );                        \
    return *this;                                            \
  }                                                          \

// Operator -- prefix
// Operator -- postfix
#define SPECIALISATIONS_DECREMENT_OPERATORS                          \
  inline my_type&                                                    \
  operator -- ()                                                     \
  {                                                                  \
    val_type tmp_val = getValue();                                   \
    --tmp_val;                                                       \
    m_api->setParam( m_full_par_name, tmp_val );                          \
    return *this;                                                    \
  }                                                                  \
                                                                     \
  inline val_type                                                    \
  operator -- (int)                                                  \
  {                                                                  \
    val_type tmp_old_val = getValue();                               \
    val_type tmp_set_val = tmp_old_val;                              \
    tmp_set_val--;                                                   \
    m_api->setParam( m_full_par_name, tmp_set_val );                      \
    return tmp_old_val;                                              \
  }                                                                  \
  
// Operator ++ prefix
// Operator ++ postfix
#define SPECIALISATIONS_INCREMENT_OPERATORS                          \
  inline my_type&                                                    \
  operator ++ ()                                                     \
  {                                                                  \
    val_type tmp_val = getValue();                                   \
    ++tmp_val;                                                       \
    m_api->setParam( m_full_par_name, tmp_val );                          \
    return *this;                                                    \
  }                                                                  \
                                                                     \
  inline val_type                                                    \
  operator ++ (int)                                                  \
  {                                                                  \
    val_type tmp_old_val = getValue();                               \
    val_type tmp_set_val = tmp_old_val;                              \
    tmp_set_val++;                                                   \
    m_api->setParam( m_full_par_name, tmp_set_val);                       \
    return tmp_old_val;                                              \
  }

// Operator +=
// Operator -=
// Operator /=
// Operator *=
#define SPECIALISATIONS_ARITHMETIC_OPERATORS                            \
  inline my_type& operator += (val_type val)                            \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp += val;                                                         \
    m_api->setParam( m_full_par_name, tmp );                                 \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  inline my_type&                                                       \
  operator -= (val_type val)                                            \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp -= val;                                                         \
    m_api->setParam( m_full_par_name, tmp );                                 \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  inline my_type&                                                       \
  operator /= (val_type val)                                            \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp /= val;                                                         \
    m_api->setParam( m_full_par_name, tmp );                                 \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  inline my_type&                                                       \
  operator *= (val_type val)                                            \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp *= val;                                                         \
    m_api->setParam( m_full_par_name, tmp );                                 \
    return *this;                                                       \
  }                                                                     \
  
// Operator %=
// Operator ^=
// Operator |=
// Operator &=
// Operator <<=
// Operator >>=
#define SPECIALISATIONS_BINARY_OPERATORS                                \
  inline my_type&                                                       \
  operator %= (val_type val)                                            \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp %= val;                                                         \
    m_api->setParam( m_full_par_name, tmp );                                 \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  inline my_type&                                                       \
  operator ^= (val_type val)                                            \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp ^= val;                                                         \
    m_api->setParam( m_full_par_name, tmp );                                 \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  inline my_type&                                                       \
  operator |= (val_type val)                                            \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp |= val;                                                         \
    m_api->setParam( m_full_par_name, tmp );                                 \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  inline my_type&                                                       \
  operator &= (val_type val)                                            \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp &= val;                                                         \
    m_api->setParam( m_full_par_name, tmp );                                 \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  inline my_type&                                                       \
  operator <<= (val_type val)                                           \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp <<= val;                                                        \
    m_api->setParam( m_full_par_name, tmp );                                 \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  inline my_type&                                                       \
  operator >>= (val_type val)                                           \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp >>= val;                                                        \
    m_api->setParam( m_full_par_name, tmp );                                 \
    return *this;                                                       \
  }                                                                     

#endif
