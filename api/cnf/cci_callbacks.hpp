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


__CCI_OPEN_CONFIG_NAMESPACE__

template<class T_cci_base_param_if>
callb_adapt_B<T_cci_base_param_if>::callb_adapt_B(void* _observer_ptr, callb_func_ptr _func, T_cci_base_param_if* _caller_param)
: observer_ptr(_observer_ptr)
, caller_param(_caller_param)
, func(_func)
{
#ifdef CCI_PARAM_CALLBACK_VERBOSE
  if (caller_param) printf("callb_adapt_B: Create parameter callback adapter %p for caller parameter '%s'.\n", (void*)this, caller_param->get_name().c_str());
  else printf("callb_adapt_B: Create parameter callback adapter %p for no parameter.\n", (void*)this);
#endif
}
  
template<class T_cci_base_param_if>
callb_adapt_B<T_cci_base_param_if>::~callb_adapt_B() {
  unregister_at_parameter();
#ifdef CCI_PARAM_CALLBACK_VERBOSE
  printf("callb_adapt_B: Deleting parameter callback adapter %p (shared pointer deleted)\n", (void*)this);
#endif
}

template<class T_cci_base_param_if>
void callb_adapt_B<T_cci_base_param_if>::unregister_at_parameter() {
  if (caller_param != NULL) {
#ifdef CCI_PARAM_CALLBACK_VERBOSE
    printf("callb_adapt_B: Unregister parameter callback adapter %p at caller parameter '%s'.\n", (void*)this, caller_param->get_name().c_str());
#endif
    // remove this out of the parameter's callback list
    // Avoid repeated call during destruction by automatically setting caller_param = NULL
    /*bool succ = */caller_param->unregister_param_callback(this);
    //assert (succ && "Unregistering this callback at parameter failed!");
  }
}

template<class T_cci_base_param_if>
callback_return_type callb_adapt_B<T_cci_base_param_if>::call(cci_base_param& changed_param, const callback_type& cb_reason) {
  if (func) {
    return func(changed_param, cb_reason);
  } else {
    SC_REPORT_ERROR(CCI_SC_REPORT_MSG_TYPE_PREFIX, "No callback registered yet.");
  }      
  return return_nothing;
}

template<class T_cci_base_param_if>
void* callb_adapt_B<T_cci_base_param_if>::get_observer() {
  return (void*) observer_ptr;
}

template<class T_cci_base_param_if>
T_cci_base_param_if* callb_adapt_B<T_cci_base_param_if>::get_caller_param() {
  return caller_param;
}

  
__CCI_CLOSE_CONFIG_NAMESPACE__
