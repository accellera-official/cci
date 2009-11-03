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

#ifndef __GS_PARAM_ATTRIBUTE_CLASSES_H__
#define __GS_PARAM_ATTRIBUTE_CLASSES_H__

#include "gs_param_class.h"


#define GS_PARAM_CONSTRUCTORS(class_name, attr)      \
    explicit class_name() {} \
    explicit class_name(const std::string &nam) : gs_param<val_type>(nam) {val_type::add_param_attribute(attr);} \
    explicit class_name(const std::string &nam, const std::string &val, const bool force_top_level_name = false) : gs_param<val_type>(nam, val, force_top_level_name) {gs_param<val_type>::add_param_attribute(attr);} \
    explicit class_name(const std::string &nam, const val_type &val, const bool force_top_level_name = false) : gs_param<val_type>(nam, val, force_top_level_name) {gs_param<val_type>::add_param_attribute(attr);} \
    explicit class_name(                                                gs_param_array* parent_array) : gs_param<val_type>(     parent_array) {gs_param<val_type>::add_param_attribute(attr);} \
    explicit class_name(const std::string &nam                        , gs_param_array* parent_array) : gs_param<val_type>(nam, parent_array) {gs_param<val_type>::add_param_attribute(attr);} \
    explicit class_name(const std::string &nam, const std::string &val, gs_param_array* parent_array) : gs_param<val_type>(nam, parent_array) {gs_param<val_type>::add_param_attribute(attr);} \
    explicit class_name(const std::string &nam, const val_type &val   , gs_param_array* parent_array) : gs_param<val_type>(nam, parent_array) {gs_param<val_type>::add_param_attribute(attr);} \
    explicit class_name(                                                gs_param_array& parent_array) : gs_param<val_type>(     &parent_array) {gs_param<val_type>::add_param_attribute(attr);} \
    explicit class_name(const std::string &nam                        , gs_param_array& parent_array) : gs_param<val_type>(nam, &parent_array) {gs_param<val_type>::add_param_attribute(attr);} \
    explicit class_name(const std::string &nam, const std::string &val, gs_param_array& parent_array) : gs_param<val_type>(nam, &parent_array) {gs_param<val_type>::add_param_attribute(attr);} \
    explicit class_name(const std::string &nam, const val_type &val   , gs_param_array& parent_array) : gs_param<val_type>(nam, &parent_array) {gs_param<val_type>::add_param_attribute(attr);} \
    explicit class_name(const std::string &nam, const std::string &val, gs_param_array* parent_array, const bool force_top_level_name, const bool register_at_db) : gs_param<val_type>(nam, val, parent_array, force_top_level_name, register_at_db) {gs_param<val_type>::add_param_attribute(attr);} \
    explicit class_name(const std::string &nam, const std::string &val, gs_param_array& parent_array, const bool force_top_level_name, const bool register_at_db) : gs_param<val_type>(nam, val, parent_array, force_top_level_name, register_at_db) {gs_param<val_type>::add_param_attribute(attr);} \


namespace gs {
namespace cnf {

  /// The parameter attribute class template
  template<param_attribute ATTR, typename T>
  class gs_param_attribute
    : public gs_param<T>
  {
    /// Typedef for the value.
    typedef T val_type;

  public:
    GS_PARAM_CONSTRUCTORS(gs_param_attribute, ATTR)

    using gs_param<T>::operator=;

    operator gs_param<T>& () {return *this;}

  };


  // Unluckly C++ does not allow template typedef like this outside a class:
  //   template <typename T>
  //   typedef gs_param_attribute<param_attributes::output, T> gs_output;
  // so we have to rewrite each type


  /// The parameter initialized with 'output' attribute
  template <typename T>
  struct gs_output
    : public gs_param<T>
  {
    /// Typedef for the value.
    typedef T val_type;

    GS_PARAM_CONSTRUCTORS(gs_output, param_attributes::output)

    using gs_param<T>::operator=;

    operator gs_param<T>& () {return *this;}
  };

  /// The parameter initialized with 'state' attribute
  template <typename T>
  struct gs_state
    : public gs_param<T>
  {
    /// Typedef for the value.
    typedef T val_type;

    GS_PARAM_CONSTRUCTORS(gs_state, param_attributes::state)

    using gs_param<T>::operator=;

    operator gs_param<T>& () {return *this;}
  };


} // namespace cnf

using cnf::gs_param_attribute;
using cnf::gs_output;
using cnf::gs_state;

} // namespace gs


#undef GS_PARAM_CONSTRUCTORS

#endif
