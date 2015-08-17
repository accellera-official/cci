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


#include "ParameterOwnerModule.h"
#include <systemc.h>

void ParameterOwnerModule::main_action() {

  std::cout << "----------------------------" << std::endl;

  // get the config broker which is responsible for this module
  cci::cci_broker_if* mBroker = &cci::cci_broker_manager::get_current_broker(cci::cci_originator(*this));
  assert(mBroker != NULL && "get_cnf_broker_instance returned is NULL");

  // demonstrate is_default_value
  cout << name() << ": uint_param get_default_value()=" << (dec) << uint_param.get_default_value()<<endl;
  cout << name() << ": uint_param="<<(dec)<< uint_param <<std::endl;
  cout << name() << ": uint_param is_default_value()=" << uint_param.is_default_value()<<endl;
  uint_param = 12000;
  cout << name() << ": set uint_param="<<(dec)<< uint_param <<std::endl;
  cout << name() << ": uint_param is_default_value()=" << uint_param.is_default_value()<<endl;
  uint_param = 10;
  cout << name() << ": set uint_param="<<(dec)<< uint_param <<std::endl;
  cout << name() << ": uint_param is_default_value()=" << uint_param.is_default_value()<<endl<<endl;

  // demonstrate has_default_value
  cout << name() << ": uint_param get_default_value()=" << (dec) << uint_param.get_default_value()<<endl;

  wait(1, SC_NS);

  std::cout << "----------------------------" << std::endl;

  // get json string from parameter
  std::string str = int_param.json_serialize();
  cout << name() << ": "<<int_param.get_name() << ".json_serialize() = "<< int_param.json_serialize()<<endl;
  
  // set parameter using json string
  int_param.json_deserialize("555000");
  str = int_param.json_serialize();
  cout << name() << ": "<<int_param.get_name() << ".json_deserialize(555000) results in "<< str <<endl;

  // set parameter directly using operator=
  cout << name() << ": set int_param = 100" << endl;
  int_param = 100;

  // set parameter directly using set function
  cout << name() << ": set int_param.set(200)" << endl;
  int_param.set(200);

  // create and access a local parameter
  cout << name() << ": create parameter " << endl;
  cci::cci_param<unsigned char> uchar_param("uchar_param", 0);
  cout << "  created " << uchar_param.get_name() << endl;
  cout << name() << ": Set the new parameter uchar_param" << endl;
  uchar_param = 'u';
  cout << endl;

  // copy operator
  cout << name() << ": copy ";
  uint_param = uint_param2;
  
  wait(10, SC_SEC);
  
  std::cout << "----------------------------" << std::endl;

  // show a parameter list
  cout << endl << "**** Parameter list: " << endl;
  std::vector<std::string> vec = mBroker->get_param_list();
  std::vector<std::string>::iterator iter;
  std::stringstream ss_show;
  for (iter = vec.begin() ; iter < vec.end(); iter++) {
    if (iter != vec.begin()) ss_show << ", ";
    ss_show << *iter;
  }
  std::cout << "   " << ss_show.str() << std::endl<<std::endl;

}
