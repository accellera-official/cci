// LICENSETEXT
//
//   Copyright (C) 2009-2010 : GreenSocs Ltd
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


#ifndef __CCI_PARAMS_H__
#define __CCI_PARAMS_H__

#ifndef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#define __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#endif
#include "gs_cci_base_param.h"
#undef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__

#ifndef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#define __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#endif
#include "gs_cci_param.h"
#undef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__

#ifndef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#define __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#endif
#include "gs_cci_value.h"
#undef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__

namespace cci_impl {
  
  template<typename T, cci::param_mutable_type TM>
  static cci::cci_param_if<T, TM>* CreateParam(cci::cci_param<T, TM> *owner_par, const char* nam, const bool force_top_level_name) {
    cci::cci_param_if<T, TM>* impl_par = new cci_impl::cci_param<T, TM>(*owner_par, nam, force_top_level_name);
    std::cout << (std::hex) << "new impl_par=" << impl_par << std::endl;
    return impl_par;
  }

  
  template<typename T, cci::param_mutable_type TM>
  static cci::cci_param_if<T, TM>* CreateParam(cci::cci_param<T, TM> *owner_par, const char* nam, const T& val, const bool force_top_level_name) {
    cci::cci_param_if<T, TM>* impl_par = new cci_impl::cci_param<T, TM>(*owner_par, nam, val, force_top_level_name);
    std::cout << (std::hex) << "new impl_par=" << impl_par << std::endl;
    return impl_par;
  }


  template<typename T, cci::param_mutable_type TM>
  static cci::cci_param_if<T, TM>* CreateParam(cci::cci_param<T, TM> *owner_par, const char* nam, const char* val, const bool force_top_level_name) {
    cci::cci_param_if<T, TM>* impl_par = new cci_impl::cci_param<T, TM>(*owner_par, nam, val, force_top_level_name);
    std::cout << (std::hex) << "new impl_par=" << impl_par << std::endl;
    return impl_par;
  }

  template<typename T, cci::param_mutable_type TM>
  static void InitParam(cci::cci_param_if<T, TM> *owner_par) {
    cci_impl::cci_param<T, TM>* p = dynamic_cast<cci_impl::cci_param<T, TM>*> (owner_par);
    assert(p != NULL && "The given parameter should be created by the CreateParam function thus being of the implementation type.");
    p->init();
  }
  

  template<class T, cci::param_mutable_type TM>
  void DestroyParam(cci::cci_param_if<T, TM>* param) {
    std::cout << (std::hex) << "delete param=" << param << std::endl;
    delete param; param = NULL;
  }
  

} // end namespace cci_impl

#endif

