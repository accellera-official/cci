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


#ifndef __GS_CCI_BASE_PARAM_H__
#define __GS_CCI_BASE_PARAM_H__

#ifndef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#error "failed include check"
#else
#undef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#endif

#include <string>
#include <iostream>
#include <map>
#include <set>
#include <boost/shared_ptr.hpp>

#include "cci.h"
#include "greencontrol/config.h"

namespace cci {
  
  // TODO: remove
  using std::cout;
  using std::endl;
  
  class cci_base_param
  : public cci_base_param_if
  {
  protected:
    /// Typedef for the param itself.
    typedef cci_base_param my_type;

    /// Callback forwarder class
    /**
     * This is instantiated and registered at the base param to forward a 
     * callback to the cci world when called by the gs_base_param.
     */
    class internal_callback_forwarder {
    public:
      internal_callback_forwarder(boost::shared_ptr<cci::callb_adapt_b> _adapt, const gs::cnf::callback_type _type, my_type& _par)
      : adapt(_adapt.get())
      , type(_type)
      , param(&_par)
      , calling_gs_adapter() {
      }
      ~internal_callback_forwarder() {
        //cout << "Destructing callback forwarder for param "<<param->get_name() << endl;
        if (calling_gs_adapter) {
          calling_gs_adapter->unregister_at_parameter();
        }
      }
      // This gets called by the base gs_param
      gs::cnf::callback_return_type call(gs::gs_param_base& par, gs::cnf::callback_type cbtype) {
        assert (param == dynamic_cast<cci_base_param*>(&par) && "Got a wrong parameter type");
        gs::cnf::callback_return_type returned_gs_message = gs::cnf::return_nothing;
        cci::callback_return_type returned_cci_message = return_nothing;
        switch(cbtype) {
          case gs::cnf::destroy_param:
            assert(par.is_destructing());
            // TODO This cannot work because the cast the the already destructed cci_base_param fails.
            // TODO adapt->call(*cci_p, cci::destroy_param);
            break;
          case gs::cnf::pre_read: // = cci::pre_read
            returned_cci_message = adapt->call(*param, cci::pre_read);
            break;
          //case gs::cnf::post_read:
          //  break;
          case gs::cnf::pre_write: // = cci::pre_write
            returned_cci_message = adapt->call(*param, cci::pre_write);
            break;
          case gs::cnf::post_write: // = cci::post_write
            returned_cci_message = adapt->call(*param, cci::post_write);
            break;
          default:
            returned_cci_message = return_other_error;
            assert(false);
        }
        switch (returned_cci_message) {
          case return_value_change_rejected:
            returned_gs_message = gs::cnf::return_value_change_rejected;
            break;
          case return_other_error:
            returned_gs_message = gs::cnf::return_other_error;
            break;
          default:
            returned_gs_message = gs::cnf::return_nothing;
        }
        return returned_gs_message;
      }
      const gs::cnf::callback_type get_type() {
        return type;
      }

      cci::callb_adapt_b* adapt;
      gs::cnf::callback_type type;
      my_type *param;
      boost::shared_ptr< ::gs::cnf::ParamCallbAdapt_b> calling_gs_adapter;
    };
    
    /// Internal guard which is called on a value change to update the value status variable(s)
    struct status_guard {
      status_guard(my_type &_owner) 
      : owner(&_owner) {
      }
      // This gets called by the base gs_param
      callback_return_type call(cci_base_param& changed_param, const callback_type& cb_reason) {
        owner->m_is_default_value = false;
        owner->m_is_invalid_value = false;
        owner->m_is_initial_value = false;
        return return_nothing;
      }
      my_type* owner;
    };
    
    friend class status_guard;
    
  public:

    explicit cci_base_param(gs::gs_param_base& gs_param_base
                            , bool force_top_level_name /*= false*/
                            , bool register_at_db /*= true*/
                            , bool has_default_value ) // if there is a default value
    : m_gs_param_base(gs_param_base) 
    , m_is_default_value(has_default_value)
    , m_is_invalid_value(!has_default_value)
    , m_is_initial_value(false)
    , m_status_guard(*this)
    , m_init_called(false)
    {
    }

  protected:
    void init() {
      bool m_init_called = true;
      register_callback(post_write, &m_status_guard, boost::bind(&status_guard::call, &m_status_guard, _1, _2)); // internal callback for status variables
    }

  public:
    virtual ~cci_base_param() {
      assert(m_init_called && "If this happens, the construction did not call the base param init function!");
    }
    
    virtual const basic_param_type get_basic_type() const { return partype_not_available; }

    virtual const std::string& get_name() const {
      return m_gs_param_base.getName();
    }
    
    // ///////   Set and Get with JSON String Representation   //////////// //

    virtual void json_deserialize(const std::string& json_string) = 0;
    
    virtual const std::string& json_serialize() const = 0;
    
    // //////////////// stuff /////////////////////////// //
    
    virtual bool lock(void* pwd = NULL)   { return m_gs_param_base.lock(pwd);    }
    virtual bool unlock(void* pwd = NULL) { return m_gs_param_base.unlock(pwd);  }
    virtual bool locked() const           { return m_gs_param_base.locked();     }
    //void set(const val_type& val, void* lock_pwd) { gs::gs_param<T>::setValue(val, lock_pwd); }

    virtual void set_documentation(const std::string& doc) {
      my_documentation = doc;
    }
    
    virtual std::string get_documentation() const {
      return my_documentation;
    }
    
    virtual bool is_default_value() {
      return m_is_default_value;
    }
    
    virtual bool is_initial_value() {
      // TODO: should be located where the init value is applied in the API (if object is already existing) and during param instantiation when database initial value is applied
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
      return m_is_initial_value;
    }
    
    virtual bool is_invalid_value() {
      return m_is_invalid_value;
    }
    
    virtual void set_invalid_value() {
      m_is_invalid_value = true;
    }
    
    // /////////////////// CALLBACKS ///////////////////// //

    virtual boost::shared_ptr<callb_adapt_b> register_callback(const callback_type type, void* observer, callb_func_ptr function) {
      // call the pure virtual function performing the registration
      return register_callback(type, boost::shared_ptr< callb_adapt_b>(new callb_adapt_b(observer, function, this)));
    }
    
    virtual boost::shared_ptr<callb_adapt_b> register_callback(const callback_type type, boost::shared_ptr< cci::callb_adapt_b> callb) {
      gs::cnf::callback_type cb = gs::cnf::no_callback;
      switch(type) {
        case pre_read:
          cb = gs::cnf::pre_read;
          break;
        //case post_read: SC_REPORT_WARNING("GreenSocs/cci/not_supported", "not supported by GreenSocs parameters"); break;
        case pre_write:
          cb = gs::cnf::pre_write;
          break;
        case create_param:
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
        internal_callback_forwarder *fw = new internal_callback_forwarder(callb, cb, *this);
        fw_vec.push_back(fw);
        fw->calling_gs_adapter = 
            m_gs_param_base.registerParamCallback( 
                                               boost::shared_ptr< ::gs::cnf::ParamCallbAdapt_b>(
                                                                                                new ::gs::cnf::ParamTypedCallbAdapt<internal_callback_forwarder>
                                                                                                (fw, 
                                                                                                 &internal_callback_forwarder::call, 
                                                                                                 callb->get_observer(), 
                                                                                                 const_cast<gs::gs_param_base*>(static_cast<gs::gs_param_base*>(&m_gs_param_base)))
                                                                                                ), fw->get_type() );
      }
      return callb;
    }
    
    virtual void unregister_all_callbacks(void* observer) {
      bool succ = true;
      internal_callback_forwarder* fw;
      while (succ) {
        succ = false;
        for (int i = 0; i < fw_vec.size(); ++i) {
          if (fw_vec[i]->adapt->get_observer() == observer) {
            fw = fw_vec[i];
            fw_vec.erase(fw_vec.begin()+i);
            delete fw;
            succ = true;
            break;
          }
        }
      }
    }
    
    virtual bool unregister_param_callback(boost::shared_ptr<cci::callb_adapt_b> callb)  {
      return unregister_param_callback(callb.get());
    }
    
    virtual bool unregister_param_callback(cci::callb_adapt_b* callb)  {
      internal_callback_forwarder* fw;
      for (int i = 0; i < fw_vec.size(); ++i) {
        if (fw_vec[i]->adapt == callb) {
          fw = fw_vec[i];
          fw_vec.erase(fw_vec.begin()+i);
          delete fw;
          return true;
        }
      }
      return false;
    }
    
    virtual bool has_callbacks() {
      return (fw_vec.size() > 0);
    }
    
  protected:
    
    /// String whose reference can be returned as string value
    mutable std::string return_string;
    
    std::vector<internal_callback_forwarder*> fw_vec;
    
    bool m_is_default_value;
    bool m_is_invalid_value;
    bool m_is_initial_value;
    status_guard m_status_guard;
    
    std::string my_documentation;
    
    gs::gs_param_base &m_gs_param_base;

    bool m_init_called;

  };


} // namespace cci

#endif
