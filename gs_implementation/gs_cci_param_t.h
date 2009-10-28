/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2009 by all Contributors.
  All Rights reserved.

  Developed by GreenSocs : http://www.greensocs.com/
   Christian Schroeder, schroeder@eis.cs.tu-bs.de
   Mark Burton, mark@greensocs.com

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/


#ifndef __GS_CCI_PARAM_BASE_H__
#define __GS_CCI_PARAM_BASE_H__


#include <string>
#include <iostream>
#include <map>
#include <set>
#include <boost/shared_ptr.hpp>

#include "cci.h"
#include "greencontrol/config.h"

namespace cci {
  
  class internal_callback_forwarder {
  public:
    internal_callback_forwarder(boost::shared_ptr<cci::callb_adapt_b> _adapt, const gs::cnf::callback_type _type)
    : adapt(_adapt)
    , type(_type) {
    }
    void call(gs::gs_param_base& par, gs::cnf::callback_type cbtype) {
      if (par.is_destructing()) {
        if (type == gs::cnf::destroy_param) { // = cci::destroy_param
          SC_REPORT_WARNING("GreenSocs/cci/not_supported", "destruction callback not supported by GreenSocs parameters");
          // This cannot work because the cast the the already destructed cci_base_param fails.
          //adapt->call(*cci_p, cci::destroy_param);
        }
      } else {
        if (type == gs::cnf::post_write) { // = cci::post_write
          gs::gs_param_base *p = &par;
          cci_base_param *cci_p = dynamic_cast<cci_base_param*> (p);
          assert (cci_p != NULL && "Got a wrong parameter type");
          adapt->call(*cci_p, cci::post_write);
        }          
      }
    }
    const gs::cnf::callback_type get_type() {
      return type;
    }
  protected:
    boost::shared_ptr<cci::callb_adapt_b> adapt;
    gs::cnf::callback_type type;
  };

  template <typename T>
  class gs_cci_param_t
  : public cci_param<T>
  , public gs::gs_param<T>
  {
  protected:
    /// Typedef for the value.
    typedef T val_type;
    /// Typedef for the param itself.
    typedef gs_cci_param_t<T> my_type;
    
  public:

    using cci_param<T>::operator=;

    explicit gs_cci_param_t(const std::string& n, 
                            const val_type &val, 
                            const bool force_top_level_name = false,
                            const bool register_at_db = true)
    : gs::gs_param<val_type>(n, val, NULL, force_top_level_name, register_at_db) {
    }

    explicit gs_cci_param_t(const std::string& n, 
                            const bool force_top_level_name = false,
                            const bool register_at_db = true)
    : gs::gs_param<val_type>(n, std::string(""), NULL, force_top_level_name, register_at_db) {
    }
    
    ~gs_cci_param_t() {
    }
    
    /*void init(const val_type& default_val) {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
    }
    
    void init() {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
    }*/
    
    const std::string& get_name() const {
      return gs::gs_param<T>::getName();
    }
   
    const bool is_destructing() const {
      return gs::gs_param<T>::is_destucting();
    }

    void set(const val_type& val) {
      gs::gs_param<T>::setValue(val);
    }
    
    const val_type& get() const {
      return gs::gs_param<T>::getValue();
    }
    
    get_param_error_type get_string(std::string& retvalue) {
      retvalue = gs::gs_param<T>::getString();
      return get_param_success;
    }

    boost::shared_ptr<callb_adapt_b> register_callback(const callback_type type, boost::shared_ptr< cci::callb_adapt_b> callb) {
      gs::cnf::callback_type cb = gs::cnf::no_callback;
      switch(type) {
        case pre_read: //SC_REPORT_WARNING("GreenSocs/cci/not_supported", "not supported by GreenSocs parameters"); break;
          cb = gs::cnf::pre_read;
          break;
        case post_read: SC_REPORT_WARNING("GreenSocs/cci/not_supported", "not supported by GreenSocs parameters"); break;
        case pre_write: //SC_REPORT_WARNING("GreenSocs/cci/not_supported", "not supported by GreenSocs parameters"); break;
          cb = gs::cnf::pre_write;
          break;
        case create_param: //SC_REPORT_WARNING("GreenSocs/cci/not_supported", "create_param callbacks not supported at parameters"); break;
          cb = gs::cnf::create_param;
          break;
        case post_write:
          cb = gs::cnf::post_write;
          break;
        case destroy_param:
          cb = gs::cnf::destroy_param;
          break;
      }
      if (cb != gs::cnf::no_callback) {
        internal_callback_forwarder *fw = new internal_callback_forwarder(callb, cb);
        fw_vec.push_back(fw);
        gs::gs_param<T>::registerParamCallback( 
                                               boost::shared_ptr< ::gs::cnf::ParamCallbAdapt_b>(
                                                                                                new ::gs::cnf::ParamTypedCallbAdapt<internal_callback_forwarder>
                                                                                                (fw, 
                                                                                                 &internal_callback_forwarder::call, 
                                                                                                 callb->get_observer(), 
                                                                                                 const_cast<gs::gs_param_base*>(static_cast<gs::gs_param_base*>(this)))
                                                                                                ), fw->get_type() );
      }
      return callb;
    }
    
    void unregister_all_callbacks(void* observer) {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
    }
    
    bool unregister_param_callback(callb_adapt_b* callb)  {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
      return false;
    }
    
    bool has_callbacks() {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
      return false;
    }
    
  protected:
    
    /// String whose reference can be returned as string value
    mutable std::string return_string;
    
    std::vector<internal_callback_forwarder*> fw_vec;
    
  };

} // namespace cci

#endif
