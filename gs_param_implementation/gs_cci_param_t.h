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


#ifndef __GS_CCI_PARAM_T_H__
#define __GS_CCI_PARAM_T_H__

#ifndef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#error "failed include check"
#else
#undef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#endif

#include <string>
#include <iostream>
#include <map>
#include <set>


__OPEN_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__
  
  // TODO: remove
  using std::cout;
  using std::endl;
  
  template <typename T>
  class gs_cci_param_t
  : public gs_cci_base_param
  {
  protected:
    /// Typedef for the param itself.
    typedef gs_cci_param_t<T> my_type;
    /// Typedef for base type
    typedef gs_cci_base_param base_type;
    /// Typedef for the param itself.
    typedef cci::cci_param_b<T> my_return_type;
    /// Typedef for the owned gs_param
    typedef gs::gs_param<T> gs_param_type;

  public:
    /// Typedef for the value.
    typedef T value_type;

    //using cci_param<T,TM>::operator=;

    gs_cci_param_t(my_return_type& owner_par
                   , const std::string& n
                   , const cci::cci_value& val
                   , bool is_top_level_name /*= false*/
                   , bool register_at_db /*= true*/
                   , cci::cci_broker_if* broker_accessor
                   , const std::string& desc = "")
    : gs_cci_base_param(owner_par, is_top_level_name, register_at_db, /*has_default_value=*/true, broker_accessor, desc)
    , m_gs_param(n, "", NULL, is_top_level_name, register_at_db)
    , m_owner_par(owner_par)
    {
	  this->m_gs_param.setString(cci::cci_value::to_json(val));
      gs_cci_base_param::m_gs_param_base = &m_gs_param;
    }
    
    gs_cci_param_t(my_return_type& owner_par
                   , const std::string& n 
                   , const value_type &val
                   , bool is_top_level_name /*= false*/
                   , bool register_at_db /*= true*/
                   , bool this_is_with_a_value // Just to make a difference (allow overloading) the constructor taking a string value 
                   , cci::cci_broker_if* broker_accessor
                   , const std::string& desc = "")
    : gs_cci_base_param(owner_par, is_top_level_name, register_at_db, true, broker_accessor, desc)
    , m_gs_param(n, val, is_top_level_name) 
    , m_owner_par(owner_par)
    {
      assert(register_at_db && "Not supported with gs_param?");
      gs_cci_base_param::m_gs_param_base = &m_gs_param;
    }
    
    ~gs_cci_param_t() {
    }
    
    /// Implicit cast operator
    operator my_return_type* () { return &m_owner_par; }
    my_return_type* get_cci_param() { return &m_owner_par; }
    operator my_return_type& () { return m_owner_par; }
    
	virtual cci::basic_param_type get_basic_type() const { return get_value().basic_type(); }

	virtual void set(const void* value)
	{
		value_type val = *static_cast<const value_type*>(value);
		if (m_gs_param.locked())
		{
			cci::cci_report_handler::set_param_failed("Parameter locked.");
			return;
		}
		if (!m_gs_param.setValue(val))
			cci::cci_report_handler::set_param_failed("Value rejected by callback.");
		else
			update_latest_write_originator();
	}
    
    virtual const void * get() const {
      return &m_gs_param.getValue();
    }
    
    virtual void set(const void* val, const void* lock_pwd) {
      if (!m_gs_param.check_pwd(lock_pwd)) {
        cci::cci_report_handler::set_param_failed("Wrong key.");
        return;
      }
      if (!m_gs_param.setValue(*static_cast<const value_type *>(val), lock_pwd))
        cci::cci_report_handler::set_param_failed("Bad value.");
      else
        update_latest_write_originator();
    }

	virtual bool equals(const cci::cci_param_impl_if& rhs) const
	{
		const my_type* other = dynamic_cast<const my_type*>(&rhs);
		if (other)
		{
			return other->m_gs_param.getValue() == this->m_gs_param.getValue();
		}
		return false;
	}

  protected:

    mutable gs_param_type m_gs_param;
    
    /// Owning parameter, to allow implicit casting to the parent
    my_return_type &m_owner_par;
    
  };


__CLOSE_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__

#endif
