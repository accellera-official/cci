// LICENSETEXT
//
//   Copyright (C) 2009-2011 : GreenSocs Ltd
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


#ifndef __CCI_PARAM_H__
#define __CCI_PARAM_H__


#ifndef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
  #error "failed include check"
#else
  #undef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#endif

#include "cci_cfg/cci_value.h"
#include "cci_cfg/cci_value_traits.h"

#include <string>
#include <iostream>
#include <sstream>

__OPEN_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__

  /// Prototype parameter implementation
  template<typename T, cci::param_mutable_type TM = cci::mutable_param>
  class gs_cci_param 
  : public gs_cci_param_t <T>
  {
  public:

    /// Typedef for the value.
    typedef T value_type;
    /// Typedef for the param itself.
    typedef gs_cci_param<value_type, TM> my_type;
    /// Typedef for base type
    typedef gs_cci_param_t<value_type> base_type;

    //using base_type::my_value;
    using base_type::operator=;
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Construction / Destruction   ///////////////////// //

    gs_cci_param(cci::cci_param<value_type, TM>& owner_par, const std::string &nam, const cci::cci_value& val    , const bool is_top_level_name, cci::cci_broker_if* broker_accessor, const std::string& desc = "") :
				base_type(owner_par, nam, val, is_top_level_name, true, broker_accessor, desc)
    {}

    gs_cci_param(cci::cci_param<value_type, TM>& owner_par, const std::string &nam, const value_type& val, const bool is_top_level_name, cci::cci_broker_if* broker_accessor, const std::string& desc = "") :
		base_type(owner_par, nam, val,              is_top_level_name, /*register_at_db=*/true, /*(dummy)*/true, broker_accessor, desc)   { }
  
    ~gs_cci_param() {
    }
    
    /*my_type& operator +=  (value_type);
    my_type& operator -=  (value_type);
    my_type& operator /=  (value_type);
    my_type& operator *=  (value_type);
    value_type operator +  (value_type);
    value_type operator -  (value_type);
    value_type operator /  (value_type);
    value_type operator *  (value_type);
    my_type& operator %=  (value_type);
    my_type& operator ^=  (value_type);
    my_type& operator &=  (value_type);
    my_type& operator |=  (value_type);
    my_type& operator <<= (value_type);
    my_type& operator >>= (value_type);
    
    my_type& operator -- ();    // prefix
    value_type operator -- (int); // postfix
    my_type& operator ++ ();    // prefix
    value_type operator ++ (int); // postfix */ 

    // //////////////// JSON (DE)SERIALIZE /////////////////////////// //


    virtual std::string json_serialize() const {
      ///@todo drop completely from CCI param interface?
	  return json_serialize(this->m_gs_param.getValue());
    }

	virtual void json_deserialize(const std::string& str) {
      ///@todo drop completely from CCI param interface?
      value_type t;
      json_deserialize(t, str);
      this->set(&t);
    }

    std::string json_serialize(const value_type& val) const {
      return cci::cci_value::to_json( cci::cci_value(val) );
    }

    void json_deserialize(value_type& target_val, const std::string& str) {
      cci::cci_value v = cci::cci_value::from_json( str );
      target_val = v.get<value_type>();
    }

    // //////////////// CCI VALUE HANDLING /////////////////////////// //

    virtual const void * get_default_value() const {
      if (!this->m_gs_param.has_default_value())
        cci::cci_report_handler::get_param_failed("Param has no default value.");
      return &this->m_gs_param.get_default_value();
    }

	virtual void set(const void* value)
	{
		if (this->m_gs_param.locked())
		{
			cci::cci_report_handler::set_param_failed("Parameter locked.");
			return;
		}
		if (set_value_allowed())
		{
			if (!this->m_gs_param.setValue(*static_cast<const value_type*>(value)))
				cci::cci_report_handler::set_param_failed("Value not allowed.");
			else
				this->update_latest_write_originator();
		}
	}
    
    virtual void set(const void* value, const void* lock_pwd) {
      if (!this->m_gs_param.check_pwd(lock_pwd)) {
        cci::cci_report_handler::set_param_failed("Wrong key.");
        return;
      }
	  if (set_value_allowed())
	  {
		  if (!this->m_gs_param.setValue(*static_cast<const value_type *>(value), lock_pwd))
			cci::cci_report_handler::set_param_failed("Value not allowed.");
		  else
			this->update_latest_write_originator();
	  }
    }
	
	virtual void set_value(const cci::cci_value& val) {
      value_type v = val.get<value_type>();
      this->set( &v );
    }

	virtual cci::cci_value get_value() const {
	   return cci::cci_value(this->m_gs_param.getValue());
    }

	virtual void destroy()
	{
		delete this;
	}

protected:
	
	//Used verify kernel phase for elaboration time parameters  
	/**  
	 * Before setValue is called for elaboration parameter updates,  
	 * this function is called to determine whether the value updates  
	 * shall be rejected or not  
	 */  
	bool isScElabPhase() {  
		if ( sc_core::sc_get_status() &  (sc_core::SC_BEFORE_END_OF_ELABORATION |  
						  sc_core::SC_ELABORATION |  
						  sc_core::SC_END_OF_ELABORATION)) 
			return true;  
		return false;  
	}  

	/// Check whether value update is allowed
	//@todo Should these checks be moved into the parameter proxy (cci_param)?
	bool set_value_allowed()
	{
      if (TM==cci::elaboration_time_param) {
        if(!isScElabPhase()){
          cci::cci_report_handler::set_param_failed("Attempt to set elaboration parameter during simulation.");
          return false;
        }
      }
      else if (TM==cci::immutable_param) {
        cci::cci_report_handler::set_param_failed("Parameter is immutable.");
        return false;
      }
      return true;
	}

  };  

__CLOSE_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__


#endif
