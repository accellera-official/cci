/*******************************************************************************
 * The following code is derived, directly or indirectly, from the SystemC
 * source code Copyright (c) 1996-2010 by all Contributors.
 * All Rights reserved.
 * 
 * The contents of this file are subject to the restrictions and limitations
 * set forth in the SystemC Open Source License Version 2.2.0 (the "License");
 * One may not use this file except in compliance with such restrictions and
 * limitations.  One may obtain instructions on how to receive a copy of the
 * License at http://www.systemc.org/.  Software distributed by Contributors
 * under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 * ANY KIND, either express or implied. See the License for the specific
 * language governing rights and limitations under the License.
 * *******************************************************************************/

/*!
 * \file      parameter_impl_by_abc.cpp
 * \brief     This class demonstrates the parameter implementation by a 
 *            different provider
 * \date      14th December, 2011
 * \author    Girish Verma, CircuitSutra Technologies     <girish@circuitsutra.com>
 *            P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 */
#include <parameter_impl_by_abc.h>

std::ostream& operator << (std::ostream& os, const user_data_type& ud)
{
	os << "\n\t[PARAM IMPL] : Source Address : " << ud.s_address << "\tDestination Address : "\
		<< ud.d_address << "\tIndex : " << ud.index;

	return os;
}	

//__CCI_OPEN_CONFIG_NAMESPACE__
cci_param_user_data_type::cci_param_user_data_type (const std::string& _name, const user_data_type&  _dvalue ) 
	: value(_dvalue)
	, default_value(_dvalue)
	, default_flag(true)
	, invalid_flag(false)
	, lock_flag(false)
	, callback_flag(false)
	, l_password(NULL)
	, documentation("")
	{
	  nam = _name ; 

		/* DO some hack for cci_value */
	}
	

//Virtual function in cci_param_impl_if
void cci_param_user_data_type::set(const user_data_type& val)
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;
	value = val;
}

const user_data_type& cci_param_user_data_type::get() const 
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;
	return value;
}

void cci_param_user_data_type::set(const user_data_type& val, void* lock_pwd)
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;
	if (l_password == lock_pwd )
		value = val;
}

std::string cci_param_user_data_type::json_serialize(const user_data_type& val) const
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;
	return std::string("not IMplemented");
}

void cci_param_user_data_type::json_deserialize(user_data_type& target_val, const std::string& str)
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;
	std::cout<<"With Json string "<<str<<std::endl;


}

const user_data_type& cci_param_user_data_type::get_default_value()
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;

	return default_value;

}


//Virtual function in cci_base_param_impl_if
void cci_param_user_data_type::json_deserialize(const std::string& json_string)
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;
	std::cout<<"With Json string "<<json_string<<std::endl;
}

std::string cci_param_user_data_type::json_serialize() const
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;
	return std::string("Not implemented");
}


const cci::cnf::basic_param_type cci_param_user_data_type::get_basic_type() const
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;

	return cci::cnf::partype_list;
}

void cci_param_user_data_type::set_value(const cci::cnf::cci_value& val)
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;
	c_value = val;
}

cci::cnf::cci_value cci_param_user_data_type::get_value() 
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;
	return c_value;
}

void cci_param_user_data_type::set_documentation(const std::string& doc)
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;

	documentation = doc;

}

std::string cci_param_user_data_type::get_documentation() const 
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;
	return documentation;
}

bool cci_param_user_data_type::is_default_value()
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;

	return default_flag;
}

bool cci_param_user_data_type::is_invalid_value()
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;

	return invalid_flag;

}

void cci_param_user_data_type::set_invalid_value()
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;
	invalid_flag=true;
}

bool cci_param_user_data_type::is_initial_value() const
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;
	return initial_flag;
}

const std::string& cci_param_user_data_type::get_name() const
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;
	std::cout<<"Return Name = "<<nam<<std::endl;

	return nam;
}

cci::shared_ptr<cci::cnf::callb_adapt> cci_param_user_data_type::register_callback(const cci::cnf::callback_type type, void* observer, cci::cnf::param_callb_func_ptr function)
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;
	/* Complex Later */

	static cci::shared_ptr<cci::cnf::callb_adapt> dummy;
	return dummy; // dummy return value for now

}

cci::shared_ptr<cci::cnf::callb_adapt> cci_param_user_data_type::register_callback(const cci::cnf::callback_type type, cci::shared_ptr<cci::cnf::callb_adapt> callb)
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;
	/* Complex Later */
	return callb; // dummy return value for now

}

void cci_param_user_data_type::unregister_all_callbacks(void* observer) 
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;

	/* Complex Later */
}

bool cci_param_user_data_type::unregister_param_callback(cci::shared_ptr<cci::cnf::callb_adapt> callb)
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;

	/* Complex Later */
	return true; // dummy return value for now
}

bool cci_param_user_data_type::unregister_param_callback(cci::cnf::callb_adapt* callb)
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;

	/* Complex Later */
	return true; // dummy return value for now
}

bool cci_param_user_data_type::has_callbacks() 
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;
	return callback_flag;
}

bool cci_param_user_data_type::lock(void* pwd )
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;
	/* Check if already */
	l_password = pwd;
	lock_flag = true ;
	return lock_flag;
}

bool cci_param_user_data_type::unlock(void* pwd )
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;

	if ( pwd == l_password )
	{
		lock_flag = false ;
		return true;
	}
	return false;

}

bool cci_param_user_data_type::locked() const
{
	std::cout<<"Function "<<__FUNCTION__<<" Called "<<std::endl;
	return lock_flag;
}

/// Creating CCI-parmeter with some default value
namespace cci {
	namespace cnf {

		template <>
		cci::cnf::cci_param_impl_if<user_data_type, cci::cnf::mutable_parameter>	\
		   *create_cci_param<user_data_type, cci::cnf::mutable_parameter>(cci::cnf::cci_param<user_data_type, cci::cnf::mutable_parameter> *owner_par,
		                                                                  const std::string &nam,
		                                                                  const user_data_type & val,
		                                                                  bool is_top_level_name,
		                                                                  cci::cnf::cci_cnf_broker_if* broker_accessor)
		{
			std::cout << "\n\t[PARAM_IMPL] : Creating CCI_PARAM: For user_data_type with cci::cnf::mutable_parameter" << std::endl;

			std::cout << "\t[PARAM_IMPL] : Called with Default Value as reference object" << val << std::endl;
	
			cci_param_user_data_type *param_impl = new cci_param_user_data_type(nam,val);
	
			return param_impl;
		}


		// Creating CCI-parameter
		template <>
		cci::cnf::cci_param_impl_if<user_data_type, cci::cnf::mutable_parameter>	\
			*create_cci_param<user_data_type, cci::cnf::mutable_parameter>(cci::cnf::cci_param<user_data_type, cci::cnf::mutable_parameter> *owner_par,
	  		                                                            const std::string &nam,
	    		                                                            const bool is_top_level_name,
	      		                                                          cci::cnf::cci_cnf_broker_if* broker_accessor)
		{
			std::cout << "\n\t[PARAM_IMPL] : Creating CCI_PARAM: For user_data_type With cci::cnf::mutable_parameter" << std::endl;

			cci_param_user_data_type *param_impl = new cci_param_user_data_type(nam,user_data_type(0,0,0));
	
			return param_impl;

		}


		// With char pointer
		template <>
		cci::cnf::cci_param_impl_if<user_data_type, cci::cnf::mutable_parameter>	\
			*create_cci_param<user_data_type, cci::cnf::mutable_parameter>(cci::cnf::cci_param<user_data_type, cci::cnf::mutable_parameter> *owner_par,
			                                                               const std::string &nam,
	  		                                                             const char * pval,
	    		                                                           const bool is_top_level_name,
	      		                                                         cci::cnf::cci_cnf_broker_if* broker_accessor)
		{
			std::cout << "\n\t[PARAM IMPL] : Creating CCI_PARAM : For user_data_type With cci::cnf::mutable_parameter "<< std::endl;
		
			//std::cout<<"Called with Default Value char string "<<string(pval)<<std::endl;
			//cci_param_user_data_type *param_impl = new cci_param_user_data_type();
			//Get Value from pval string 

			cci_param_user_data_type *param_impl = new cci_param_user_data_type(nam,user_data_type(0,0,0));
	
			return param_impl;

		}


		//This provides an implmentation for initializing CCI_PARAM with an init_value
		template <>
		void init_cci_param<user_data_type, cci::cnf::mutable_parameter>(cci::cnf::cci_param<user_data_type,
 		                                                                 cci::cnf::mutable_parameter> *owner_par)
		{
			std::cout << "\n\t[PARAM IMPL] : 'init_cci_param' : For user_data_type With cci::cnf::mutable_parameter" << std::endl;
		}

		//This provides an implmentation for destroying CCI_PARAM
		template <>
		void destroy_cci_param<user_data_type, cci::cnf::mutable_parameter> (cci::cnf::cci_param<user_data_type, cci::cnf::mutable_parameter>* param)
		{
			std::cout << "\n\t[PARAM IMPL] : 'destroy_cci_param' : For user_data_type With cci::cnf::mutable_parameter" << std::endl;
		}


	}	//End of CNF namespace

} // End of CCI namespace
