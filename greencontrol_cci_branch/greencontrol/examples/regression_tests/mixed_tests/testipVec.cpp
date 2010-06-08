//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2008 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#include "testipVec.h"

#define IPVecTest(a) std::cout << "IPVec test: " << a <<std::endl
#ifndef SPACE
# define SPACE std::cout << std::endl
#endif

void TestIPVec::main_action() {

  DEMO_PRINT_SPACE;
  DEMO_TRACE(name(), "This is the main action in TestIPVec");
  SPACE;
  IPVecTest("show vec1: "<< vec1.getString());
  gs::ctr::show_vector<std::string>(vec1);
  SPACE;
  IPVecTest("show vec2: "<< vec2.getString());
  gs::ctr::show_vector<std::string>(vec2);
  SPACE;
  IPVecTest("show vec3: "<< vec3.getString());
  gs::ctr::show_vector<std::string>(vec3);
  SPACE;
  IPVecTest("show vec4: "<< vec4.getString());
  gs::ctr::show_vector<std::string>(vec4);
  SPACE;
  IPVecTest("show vec5: "<< vec5.getString());
  gs::ctr::show_vector<std::string>(vec5);
  SPACE;
  std::string setStr;
  
  // simple example
  setStr = "{\"This\", \"is the\", \"simplest\", \"example\"}";
  gs::gs_param< std::vector<std::string> >    vecLocal;
  IPVecTest("set vector param to: '"<< setStr << "'");
  vecLocal.setString(setStr);
  IPVecTest(" show vecLocal: "<< vecLocal.getString());
  gs::ctr::show_vector<std::string>(vecLocal);
  SPACE;
  
  // variants of spaces and tabs
  setStr = "{ \"This\",\"is the\"   ,   \"simplest\"    ,    \"example\"}";
  IPVecTest("set vector param to: '"<< setStr << "'");
  vecLocal.setString(setStr);
  IPVecTest(" show vecLocal: "<< vecLocal.getString());
  gs::ctr::show_vector<std::string>(vecLocal);
  SPACE;
  
  // example with quotes within one of the strings
  setStr = "{\"example\", \"with\", \"quotes \\\"here\\\" inside and comma, here\", \"and single quote 'here\" }";
  IPVecTest("set vector param to: '"<< setStr << "'");
  vecLocal.setString(setStr);
  IPVecTest(" show vecLocal: "<< vecLocal.getString());
  gs::ctr::show_vector<std::string>(vecLocal);
  SPACE;

  // deserialize -> serialize -> deserialize test
  IPVecTest("deserialize -> serialize -> deserialize test: '"<<setStr<<"'");
  vecLocal.setString(vecLocal.getString());
  IPVecTest("result:");
  IPVecTest(" show vecLocal: "<< vecLocal.getString());
  gs::ctr::show_vector<std::string>(vecLocal);
  SPACE;

}

void TestIPVec::config_callback(gs::gs_param_base& par) {
  if (!par.is_destructing()) {
    printf("TestIPVec::config_callback: Callback for changed parameter %s. New value: %s\n", par.getName().c_str(), par.getString().c_str());
  }
  else {
    printf("TestIPVec::config_callback: Parameter %s is destructing.\n", par.getName().c_str());
  }
}

void TestIPVec::new_param_callback(const std::string parname, const std::string value) {
  printf("TestIPVec::new_param_callback: Callback for NEW PARAMETERS: Parameter %s, value: %s\n", parname.c_str(), value.c_str());
}
