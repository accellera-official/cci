//   OSCI CCI WG
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// 
// ENDLICENSETEXT


#include "ParameterOwnerModule.h"
#include <systemc.h>

void ParameterOwnerModule::main_action() {

  cci::cci_cnf_api* mApi = cci::get_cnf_api_instance(this);
  assert(mApi != NULL && "get_cnf_api_instance returned is NULL");
  
  // regression tests params
  std::string str = int_param.json_serialize();
  cout << name() << ": "<<int_param.get_name() << ".json_serialize() = "<< int_param.json_serialize()<<endl;
  
  int_param.json_deserialize("555000");
  bool succ = int_param.get_string(str);
  assert(succ);
  cout << name() << ": "<<int_param.get_name() << ".json_deserialize(555000) results in "<< str <<endl;
  
  wait(1, SC_NS);

  wait(1, SC_US);

  
  wait(1, SC_NS);
  cout << name() << ": set int_param = 100" << endl;
  int_param = 100;
  cout << endl;
  wait(SC_ZERO_TIME);

  cout << name() << ": set str_param = 'Hello World!'" << endl;
  str_param = "Hello World!";
  cout << endl;
  wait(SC_ZERO_TIME);
  
  
  wait(1, SC_NS);
  cout << name() << ": set int_param = 200" << endl;
  int_param = 200;
  cout << endl;
  wait(SC_ZERO_TIME);
  
  cout << name() << ": set str_param = 'Hello Germany!'" << endl;
  str_param = "Hello Germany!";
  cout << endl;
  wait(SC_ZERO_TIME);
  
  
  cout << name() << ": Create two new parameters which can be observed with new param callbacks/events and set them" << endl;

  cout << name() << ": unnamed param" << endl;
  cci::gs_cci_param<unsigned char> uchar_param;
  cout << endl;
  wait(SC_ZERO_TIME);
  cout << name() << ": Set the new parameter uchar_param" << endl;
  uchar_param = 'u';
  cout << endl;

  cout << endl << "**** Parameter list: " << endl;
  std::vector<std::string> vec = mApi->get_param_list();
  // Show vector
  std::vector<std::string>::iterator iter;
  std::stringstream ss_show;
  for (iter = vec.begin() ; iter < vec.end(); iter++) {
    if (iter != vec.begin()) ss_show << ", ";
    ss_show << *iter;
  }
  std::cout << "   " << ss_show.str() << std::endl<<std::endl<<std::endl;
  
  wait(SC_ZERO_TIME);
  cci::gs_cci_param<unsigned long long> ull_param("ull_param", 1000ULL);
  std::cout << std::endl;
  wait(SC_ZERO_TIME);
  std::cout << name() << ": Set the new parameter ull_param" << std::endl;
  ull_param = 2000LL;
  std::cout << std::endl;
 
  wait(100, SC_US);

  std::cout << "----------------------------" << std::endl;

}
