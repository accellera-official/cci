/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2009-2010 GreenSocs
  All rights reserved.
  Authors:
      Christian Schroeder <schroeder@eis.cs.tu-bs.de>
      Mark Burton <mark@greensocs.com>

  Copyright 2016 Ericsson
  All rights reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 *****************************************************************************/


#include "cci_cfg/cci_report_handler.h"   //@TODO - better if use of this could be corralled into a CPP; now it leaks a private class to all users
#include "cci_param_untyped_handle.h"

CCI_OPEN_NAMESPACE_


template<class cci_param_if_T, class cci_broker_if_T>
callb_adapt_T<cci_param_if_T, cci_broker_if_T>::callb_adapt_T(void* _observer_ptr, param_callb_func_ptr _func, cci_param_if_T* _caller_param)
: observer_ptr(_observer_ptr)
, caller_param(_caller_param)
, caller_broker(NULL)
, par_func(_func)
, bro_func(NULL)
{
#ifdef CCI_PARAM_CALLBACK_VERBOSE
  if (caller_param) printf("callb_adapt: Create parameter callback adapter %p for caller parameter '%s'.\n", (void*)this, caller_param->get_name().c_str());
  else printf("callb_adapt: Create parameter callback adapter %p for no parameter.\n", (void*)this);
#endif
}

template<class cci_param_if_T, class cci_broker_if_T>
callb_adapt_T<cci_param_if_T, cci_broker_if_T>::callb_adapt_T(void* _observer_ptr, broker_callb_func_ptr _func, cci_broker_if_T* _caller_broker)
: observer_ptr(_observer_ptr)
, caller_param(NULL)
, caller_broker(_caller_broker)
, par_func(NULL)
, bro_func(_func)
{
#ifdef CCI_PARAM_CALLBACK_VERBOSE
  if (caller_param) printf("callb_adapt: Create parameter callback adapter %p for caller parameter '%s'.\n", (void*)this, caller_param->get_name().c_str());
  else printf("callb_adapt: Create parameter callback adapter %p for no parameter.\n", (void*)this);
#endif
}

template<class cci_param_if_T, class cci_broker_if_T>
callb_adapt_T<cci_param_if_T, cci_broker_if_T>::~callb_adapt_T() {
  unregister_at_parameter();
#ifdef CCI_PARAM_CALLBACK_VERBOSE
  printf("callb_adapt: Deleting parameter callback adapter %p (shared pointer deleted)\n", (void*)this);
#endif
}

template<class cci_param_if_T, class cci_broker_if_T>
void callb_adapt_T<cci_param_if_T, cci_broker_if_T>::unregister_at_parameter() {
  if (caller_param != NULL) {
#ifdef CCI_PARAM_CALLBACK_VERBOSE
    printf("callb_adapt: Unregister parameter callback adapter %p at caller parameter '%s'.\n", (void*)this, caller_param->get_name().c_str());
#endif
    // remove this out of the parameter's callback list
    // Avoid repeated call during destruction by automatically setting caller_param = NULL
    /*bool succ = */caller_param->unregister_callback(this);
    //assert (succ && "Unregistering this callback at parameter failed!");
  }
  if (caller_broker != NULL) {
    SC_REPORT_ERROR(__CCI_SC_REPORT_MSG_TYPE_PREFIX__, "Not possible - TODO?.");
#ifdef CCI_PARAM_CALLBACK_VERBOSE
    printf("callb_adapt: Unregister broker callback adapter %p at caller broker '%s'.\n", (void*)this, caller_broker->name().c_str());
#endif
    // remove this out of the parameter's callback list
    //caller_broker->unregister_callback(this);
  }
  
}

template<class cci_param_if_T, class cci_broker_if_T>
callback_return_type callb_adapt_T<cci_param_if_T, cci_broker_if_T>::call(cci_param_if_T& changed_param, const callback_type& cb_reason) {
  if (par_func) {
    return par_func(changed_param, cb_reason);
  } else if (bro_func) {
    return bro_func("n", cb_reason);
    //return bro_func(changed_param.get_name(), cb_reason);
  } else {
    SC_REPORT_ERROR(__CCI_SC_REPORT_MSG_TYPE_PREFIX__, "No callback registered yet.");
  }      
  return return_nothing;
}

template<class cci_param_if_T, class cci_broker_if_T>
callback_return_type callb_adapt_T<cci_param_if_T, cci_broker_if_T>::call(const std::string& changed_param_name, const callback_type& cb_reason) {
  if (par_func) {
    assert(caller_broker && "Caller broker must have been set in constructor when using this call function");
    cci_param_if_T* p = caller_broker->get_param_handle(changed_param_name);
    if (p) return par_func(*p, cb_reason);
    else {
      SC_REPORT_ERROR(__CCI_SC_REPORT_MSG_TYPE_PREFIX__, "Cannot call this callback function with not explicit parameter.");  
    }
  } else if (bro_func) {
    return bro_func(changed_param_name, cb_reason);    
  } else {
    SC_REPORT_ERROR(__CCI_SC_REPORT_MSG_TYPE_PREFIX__, "No callback registered yet.");
  }      
  return return_nothing;
}

template<class cci_param_if_T, class cci_broker_if_T>
void* callb_adapt_T<cci_param_if_T, cci_broker_if_T>::get_observer() {
  return (void*) observer_ptr;
}

template<class cci_param_if_T, class cci_broker_if_T>
cci_param_if_T* callb_adapt_T<cci_param_if_T, cci_broker_if_T>::get_caller_param() {
  return caller_param;
}

template<class cci_param_if_T, class cci_broker_if_T>
cci_param_if_T* callb_adapt_T<cci_param_if_T, cci_broker_if_T>::get_caller_broker() {
  return caller_broker;
}


CCI_CLOSE_NAMESPACE_
