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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


__CCI_OPEN_CONFIG_NAMESPACE__


template<class cci_base_param_T, class cci_cnf_broker_if_T>
callb_adapt_T<cci_base_param_T, cci_cnf_broker_if_T>::callb_adapt_T(void* _observer_ptr, param_callb_func_ptr _func, cci_base_param_T* _caller_param)
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

template<class cci_base_param_T, class cci_cnf_broker_if_T>
callb_adapt_T<cci_base_param_T, cci_cnf_broker_if_T>::callb_adapt_T(void* _observer_ptr, broker_callb_func_ptr _func, cci_cnf_broker_if_T* _caller_broker)
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

template<class cci_base_param_T, class cci_cnf_broker_if_T>
callb_adapt_T<cci_base_param_T, cci_cnf_broker_if_T>::~callb_adapt_T() {
  unregister_at_parameter();
#ifdef CCI_PARAM_CALLBACK_VERBOSE
  printf("callb_adapt: Deleting parameter callback adapter %p (shared pointer deleted)\n", (void*)this);
#endif
}

template<class cci_base_param_T, class cci_cnf_broker_if_T>
void callb_adapt_T<cci_base_param_T, cci_cnf_broker_if_T>::unregister_at_parameter() {
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

template<class cci_base_param_T, class cci_cnf_broker_if_T>
callback_return_type callb_adapt_T<cci_base_param_T, cci_cnf_broker_if_T>::call(cci_base_param_T& changed_param, const callback_type& cb_reason) {
  if (par_func) {
    return par_func(changed_param, cb_reason);
  } else if (bro_func) {
    return bro_func(changed_param.get_name(), cb_reason);    
  } else {
    SC_REPORT_ERROR(__CCI_SC_REPORT_MSG_TYPE_PREFIX__, "No callback registered yet.");
  }      
  return return_nothing;
}

template<class cci_base_param_T, class cci_cnf_broker_if_T>
callback_return_type callb_adapt_T<cci_base_param_T, cci_cnf_broker_if_T>::call(const std::string& changed_param_name, const callback_type& cb_reason) {
  if (par_func) {
    assert(caller_broker && "Caller broker must have been set in constructor when using this call function");
    cci_base_param_T* p = caller_broker->get_param(changed_param_name);
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

template<class cci_base_param_T, class cci_cnf_broker_if_T>
void* callb_adapt_T<cci_base_param_T, cci_cnf_broker_if_T>::get_observer() {
  return (void*) observer_ptr;
}

template<class cci_base_param_T, class cci_cnf_broker_if_T>
cci_base_param_T* callb_adapt_T<cci_base_param_T, cci_cnf_broker_if_T>::get_caller_param() {
  return caller_param;
}

template<class cci_base_param_T, class cci_cnf_broker_if_T>
cci_base_param_T* callb_adapt_T<cci_base_param_T, cci_cnf_broker_if_T>::get_caller_broker() {
  return caller_broker;
}


__CCI_CLOSE_CONFIG_NAMESPACE__
