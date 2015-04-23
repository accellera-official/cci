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

#include "cci_cnf/cci_value.h"
#include "cci_cnf/cci_value_traits.h"

#include <string>
#include <iostream>
#include <sstream>

__OPEN_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__

  /// Prototype parameter implementation
  template<typename T, cci::cnf::param_mutable_type TM = cci::cnf::mutable_param>
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

    gs_cci_param(cci::cnf::cci_param<value_type, TM>& owner_par, const std::string &nam, const cci::cnf::cci_value& val    , const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) :
				base_type(owner_par, nam, val, is_top_level_name, true, broker_accessor)
    {}

    gs_cci_param(cci::cnf::cci_param<value_type, TM>& owner_par, const std::string &nam, const value_type& val, const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) :
		base_type(owner_par, nam, val,              is_top_level_name, /*register_at_db=*/true, /*(dummy)*/true, broker_accessor)   { }
  
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
      return cci::cnf::cci_value::to_json( cci::cnf::cci_value(val) );
    }

    void json_deserialize(value_type& target_val, const std::string& str) {
      cci::cnf::cci_value v = cci::cnf::cci_value::from_json( str );
      target_val = v.get<value_type>();
    }

    // //////////////// CCI VALUE HANDLING /////////////////////////// //

    virtual const void * get_default_value() const {
      if (!this->m_gs_param.has_default_value())
        cci::cnf::cci_report_handler::get_param_failed("Param has no default value.");
      return &this->m_gs_param.get_default_value();
    }

    // //////////////// CCI VALUE HANDLING /////////////////////////// //
    
	virtual void set_value(const cci::cnf::cci_value& val) {
      value_type v = val.get<value_type>();
      this->set( &v );
    }

	virtual cci::cnf::cci_value get_value() const {
	   return cci::cnf::cci_value(this->m_gs_param.getValue());
    }

	virtual void destroy()
	{
		delete this;
	}

  };  

__CLOSE_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__


#endif
