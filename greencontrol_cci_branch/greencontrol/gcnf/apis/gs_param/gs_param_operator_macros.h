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

#ifndef __GS_PARAM_OPERATOR_MACROS_H__
#define __GS_PARAM_OPERATOR_MACROS_H__

// /////////////////////////////////////////////////////////////////// //
// This file defines macros for the template specializations of gs_param

// Macros:
// GC_SPECIALISATIONS_EQUAL_OPERATORS
// GC_SPECIALISATIONS_INCREMENT_OPERATORS
// GC_SPECIALISATIONS_ARITHMETIC_OPERATORS
// GC_SPECIALISATIONS_BINARY_OPERATORS


// Operator = with my_type
// Operator = with val_type
/*#define GC_SPECIALISATIONS_EQUAL_OPERATORS                            \
  my_type& operator= (const my_type& v) {                               \
  m_api->setParam( m_par_name,                                          \
  const_cast<my_type&>(v).get() );                                      \
  return *this;                                                         \
  }                                                                     \
  \
  my_type& operator= (val_type v) { \
  this->setValue(v); \
  return *this; \
  }                                                                  
*/

// Operator -- prefix
// Operator -- postfix
#define GC_SPECIALISATIONS_DECREMENT_OPERATORS                       \
  inline my_type&                                                    \
  operator -- ()                                                     \
  {                                                                  \
    val_type tmp_val = getValue();                                   \
    --tmp_val;                                                       \
    this->setValue( tmp_val );                                       \
    return *this;                                                    \
  }                                                                  \
                                                                     \
  inline val_type                                                    \
  operator -- (int)                                                  \
  {                                                                  \
    val_type tmp_old_val = getValue();                               \
    val_type tmp_set_val = tmp_old_val;                              \
    tmp_set_val--;                                                   \
    this->setValue( tmp_set_val );                                   \
    return tmp_old_val;                                              \
  }                                                                  \
  
// Operator ++ prefix
// Operator ++ postfix
#define GC_SPECIALISATIONS_INCREMENT_OPERATORS                       \
  inline my_type&                                                    \
  operator ++ ()                                                     \
  {                                                                  \
    val_type tmp_val = getValue();                                   \
    ++tmp_val;                                                       \
    this->setValue( tmp_val );                                       \
    return *this;                                                    \
  }                                                                  \
                                                                     \
  inline val_type                                                    \
  operator ++ (int)                                                  \
  {                                                                  \
    val_type tmp_old_val = getValue();                               \
    val_type tmp_set_val = tmp_old_val;                              \
    tmp_set_val++;                                                   \
    this->setValue( tmp_set_val);                                    \
    return tmp_old_val;                                              \
  }

// Operator +=
// Operator -=
// Operator /=
// Operator *=
#define GC_SPECIALISATIONS_ARITHMETIC_OPERATORS                         \
  inline my_type& operator += (val_type val)                            \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp += val;                                                         \
    this->setValue( tmp );                                              \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  inline my_type&                                                       \
  operator -= (val_type val)                                            \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp -= val;                                                         \
    this->setValue( tmp );                                              \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  inline my_type&                                                       \
  operator /= (val_type val)                                            \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp /= val;                                                         \
    this->setValue( tmp );                                              \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  inline my_type&                                                       \
  operator *= (val_type val)                                            \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp *= val;                                                         \
    this->setValue( tmp );                                              \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  inline val_type operator* (gs_param<val_type> &second)                \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp = tmp * second.getValue();                                      \
    return tmp;                                                         \
  }                                                                     \
  inline val_type operator- (gs_param<val_type> &second)                \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp = tmp - second.getValue();                                      \
    return tmp;                                                         \
  }                                                                     \
  inline val_type operator+ (gs_param<val_type> &second)                \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp = tmp + second.getValue();                                      \
    return tmp;                                                         \
  }                                                                     \
  inline val_type operator/ (gs_param<val_type> &second)                \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp = tmp / second.getValue();                                      \
    return tmp;                                                         \
  }                                                                     \

  
// Operator %=
// Operator ^=
// Operator |=
// Operator &=
// Operator <<=
// Operator >>=
#define GC_SPECIALISATIONS_BINARY_OPERATORS                             \
  inline my_type&                                                       \
  operator %= (val_type val)                                            \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp %= val;                                                         \
    this->setValue( tmp );                                              \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  inline my_type&                                                       \
  operator ^= (val_type val)                                            \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp ^= val;                                                         \
    this->setValue( tmp );                                              \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  inline my_type&                                                       \
  operator |= (val_type val)                                            \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp |= val;                                                         \
    this->setValue( tmp );                                              \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  inline my_type&                                                       \
  operator &= (val_type val)                                            \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp &= val;                                                         \
    this->setValue( tmp );                                              \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  inline my_type&                                                       \
  operator <<= (val_type val)                                           \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp <<= val;                                                        \
    this->setValue( tmp );                                              \
    return *this;                                                       \
  }                                                                     \
                                                                        \
  inline my_type&                                                       \
  operator >>= (val_type val)                                           \
  {                                                                     \
    val_type tmp = getValue();                                          \
    tmp >>= val;                                                        \
    this->setValue( tmp );                                              \
    return *this;                                                       \
  }                                                                     

#endif
