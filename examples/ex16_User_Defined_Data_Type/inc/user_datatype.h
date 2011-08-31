/* The following code is derived, directly or indirectly, from the SystemC
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
 ********************************************************************************/

/**
 * @file    parameter_owner.h
 * @brief   This header instantiates various CCI parameters with default values
 * @author  Girish Verma, CircuitSutra Technologies    <girish@circuitsutra.com>
 *          P V S Phaneendra, CircuitSutra Technologies    <pvs@circuitsutra.com>
 * @date    31st August, 2011 (Wednesday)
 */
#ifndef   _USER_DATATYPE_H_
#define   _USER_DATATYPE_H_

#include <iostream>
#include <cci.h>
#include <core/cci_shared_ptr.h>
#include <systemc.h>
#include <string>

#include "json_spirit.h"

#include "cci_params.h"

/** 
 * @brief   route_table_ut structure takes in the f
 */
struct route_table_ut
{
	/// Default Constructor
	route_table_ut()
	: s_address(0x0)	//Source Address
	, d_address(0x0)	//Destination Address
	, index(0x0)	    //Index
	{
		// Nothing to implement
	} 

	/// Overloaded Constructor
  route_table_ut(int saddr, int daddr, int idx)
  : s_address(saddr)
  , d_address(daddr)
  , index(idx)
  {
		// Nothing to implement
	}
    
  int    s_address;
  int    d_address;
  int    index;

};// End of STRUCT  


template <cci::cnf::param_mutable_type TM = cci::cnf::mutable_parameter>
struct cci_route_table_ut_param : public __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__::gs_cci_param<route_table_ut,TM> 
{
  /// Typedef for the value
  typedef route_table_ut val_type;

  /// Typedef for the param itself
  typedef __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__::gs_cci_param<val_type, TM> my_type;

  /// Typedef for base type
  typedef __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__::gs_cci_param_t<val_type, TM> base_type;


  cci_route_table_ut_param (cci::cnf::cci_param<val_type, TM>& owner_par, 
	                           const char* nam, const char* val, 
	                           const bool is_top_level_name, 
	                           cci::cnf::cci_cnf_broker_if* broker_accessor)
	: __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__::gs_cci_param<val_type, TM>( owner_par, nam, val, is_top_level_name, broker_accessor )
	{ 

	}

	cci_route_table_ut_param (cci::cnf::cci_param<val_type, TM>& owner_par, 
	                           const char* nam, const val_type& val, 
	                           const bool is_top_level_name, 
	                           cci::cnf::cci_cnf_broker_if* broker_accessor)
	: __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__::gs_cci_param<val_type, TM>( owner_par, nam, val, is_top_level_name, broker_accessor )
	{ 

	}

	cci_route_table_ut_param (cci::cnf::cci_param<val_type, TM>& owner_par, 
	                           const char* nam,  
	                           const bool is_top_level_name, 
	                           cci::cnf::cci_cnf_broker_if* broker_accessor)
	: __NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__::gs_cci_param<val_type, TM>( owner_par, nam,  is_top_level_name, broker_accessor )
	{ 

	}

	/// Destructor
	~cci_route_table_ut_param()
	{
		// Nothing to destruct 
	}

};// End of SC_MODULE


std::ostream& operator << (std::ostream& os, const route_table_ut& ud)
{

  json_spirit::Object user_dt_obj;
  std::ostringstream json_ostream;
  std::string json_str;
  user_dt_obj.push_back( json_spirit::Pair( "s_address", (int)ud.s_address ) );
  user_dt_obj.push_back( json_spirit::Pair( "d_address", (int)ud.d_address ) );
  user_dt_obj.push_back( json_spirit::Pair( "index",     (int)ud.index ) );


  json_spirit::write( user_dt_obj, json_ostream );
  json_str = json_ostream.str();

  os<<json_ostream.str();

  return os;
}


std::istream& operator >> (std::istream& is, route_table_ut& ud)
{

    json_spirit::Value jvalue;
    read( is, jvalue );
    json_spirit::Object  usr_dt_obj = jvalue.get_value<json_spirit::Object>();
    for(unsigned int i=0; i< usr_dt_obj.size(); i++) {
      if(usr_dt_obj[i].name_.compare("s_address")==0)
        ud.s_address = usr_dt_obj[i].value_.get_value<int>();

      if(usr_dt_obj[i].name_.compare("d_address")==0)
        ud.d_address = usr_dt_obj[i].value_.get_value<int>();

      if(usr_dt_obj[i].name_.compare("index")==0)
        ud.index = usr_dt_obj[i].value_.get_value<int>();
    }

  /* json_spirit lib modify  error flag  */
  is.clear();
  is.setstate(ifstream::eofbit );

  return is;
}

#endif /* _USER_DATATYPE_H_ */

