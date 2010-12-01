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

#ifndef __ModuleTests_h__
#define __ModuleTests_h__

#include "globals.h"

#include "greencontrol/config.h"


/// Module having a private API and testing the private API
class ModuleTests
: public sc_core::sc_module
{
public:
  
  SC_HAS_PROCESS(ModuleTests);
  /// Constructor
  ModuleTests(sc_core::sc_module_name name)
  : sc_core::sc_module(name),
    int_param("mA_int_param", 45),
    int_param2("int_param2", 245),
    pub_par("pub_par")
  {
    mApi = gs::cnf::GCnf_Api::getApiInstance(this);
    SC_THREAD(main_action);
    gs::gs_param<double> tmp_par("tmp_par");
    
  }
  
  void main_action();
  
  void new_param_callback(const std::string parname, const std::string value);
  
  /// Function to be used as dynamic process. Calls the given function when event is notified.
  void spawned_thread();
  
protected:
  /// API
  gs::cnf::cnf_api_if *mApi;
  
  /// Example parameter.
  gs::gs_param<int>   int_param;
  gs::gs_param<int>   int_param2;
  gs::gs_param<int>   pub_par;
};


void ModuleTests::main_action() {
  std::cout << std::endl << "***** BEGIN "<< name() <<" main_action *****" << std::endl<<std::endl;
  
  std::string nam;
  
  // ** Tests **
  // ** For each function test a) the private API b) the forwarding to parent API
  
  // ** deprecated functions
  
  std::cout << "  Test adding public param (deprecated):" << std::endl;
  nam = name(); nam += ".myAddedParam1Pub";
  mApi->addParam(nam);
  std::cout << "  Test adding private param (deprecated):" << std::endl;
  nam = name(); nam += ".myAddedParam2Priv";
  mApi->addParam(nam, "value2");

  std::cout << "  Test setting public param (deprecated):" << std::endl;
  nam = name(); nam += ".myAddedParam1Pub";
  mApi->setParam(nam, "value1b");
  std::cout << "  Test setting private param (deprecated):" << std::endl;
  nam = name(); nam += ".myAddedParam2Priv";
  mApi->setParam(nam, "value2b");

  std::cout << "  Test getting public param (deprecated):" << std::endl;
  nam = name(); nam += ".myAddedParam1Pub";
  std::cout << "    value should be 'value1b': " << mApi->getParam(nam) << std::endl;
  std::cout << "  Test getting private param (deprecated):" << std::endl;
  nam = name(); nam += ".myAddedParam2Priv";
  std::cout << "    value should be 'value2b': " << mApi->getParam(nam) << std::endl;

  // ** normal functions

  // tests mApi->addPar
  nam = name();
  nam += ".myAddTestParamPub";
  mApi->setInitValue(nam, "1000");
  gs::gs_param<int> *myPubParam = new gs::gs_param<int>("myAddTestParamPub"); 
  std::cout << "  Test passed if value of '"<<name()<<".myAddTestParamPub' == '1000': "<<*myPubParam << std::endl;
  
  nam = name();
  nam += ".myAddTestParamPriv";
  mApi->setInitValue(nam, "2000");
  gs::gs_param<unsigned int> *myPrivParam = new gs::gs_param<unsigned int>("myAddTestParamPriv"); 
  std::cout << "  Test passed if value of '"<<name()<<".myAddTestParamPriv' == '2000': "<<*myPrivParam << std::endl;
  
  nam = name();
  nam +=".myAddTestParamPub";
  gs::gs_param_base *par = mApi->getPar(nam);
  if (par != NULL)
    std::cout << "  Passed getPar test!" << std::endl;
  else
    std::cout << "  Failed getPar test!" << std::endl;

  gs::gs_param_base *par2 = mApi->getPar("NotExisting.myAddTestParam");
  if (par2 == NULL)
    std::cout << "  Passed getPar test (passed getting not existing param failed)!" << std::endl;
  else
    std::cout << "  Failed getPar test!" << std::endl;
  
  // exists test within private API
  nam = name();
  nam += ".myAddTestParamPriv";
  if ( mApi->existsParam(nam) ) {
    std::cout << "  Passed existsParam test (private)!" << std::endl;
  } else {
    std::cout << "  Failed existsParam test (private)!" << std::endl;
  }
  nam = name();
  nam += ".notExisting.myAddTestParamPriv";
  if ( mApi->existsParam(nam) ) {
    std::cout << "  Failed existsParam test (private)!" << std::endl;
  } else {
    std::cout << "  Passed existsParam test (private)!" << std::endl;
  }
  
  // exists test within parent API
  if ( mApi->existsParam("ModuleTop.my_top_int_param") ) {
    std::cout << "  Passed existsParam test (parent)!" << std::endl;
  } else {
    std::cout << "  Failed existsParam test (parent)!" << std::endl;
  }
  if ( mApi->existsParam("ModuleA.not_existing_param") ) {
    std::cout << "  Failed existsParam test (parent)!" << std::endl;
  } else {
    std::cout << "  Passed existsParam test (parent)!" << std::endl;
  }
  
  // ** list functions 
  std::vector<std::string> lst;
  lst = mApi->getParamList();
  std::cout << "  "<<name() << " performs getParamList() passed if all public and only own private params are shown:" << std::endl;
  gs::ctr::show_vector(lst);
    
  lst = mApi->getParamList("ModuleTopA.ModuleTests");
  std::cout << "  "<<name() << " performs getParamList('ModuleTopA.ModuleTests'):"<< std::endl;
  std::cout << "    if this is ModuleTopA.ModuleTests:     passed if all my params are shown:" << std::endl;
  std::cout << "    if this is NOT ModuleTopA.ModuleTests: passed if only the public params are shown:" << std::endl;
  gs::ctr::show_vector(lst);

  lst = mApi->getParamList("ModuleTopA", true);
  std::cout << "  "<<name() << " performs getParamList('ModuleTopA', true) passed if all public and only own private params are shown:" << std::endl;
  std::cout << "    if this is ModuleTopA.ModuleTests:     passed if all (even private) params of ModuleTopA are shown:" << std::endl;
  std::cout << "    if this is NOT ModuleTopA.ModuleTests: passed if only the public params of ModuleTopA are shown:" << std::endl;
  gs::ctr::show_vector(lst);
  
  // ** Event and Callback functions
  
  std::cout << "  Test new param event" << std::endl;
  sc_core::sc_event &ev = mApi->getNewParamEvent();
  sc_core::sc_spawn_options opt;
   opt.spawn_method();
   opt.dont_initialize();
   opt.set_sensitivity(&ev);
  sc_core::sc_spawn(/*void*/sc_bind(&ModuleTests::spawned_thread, this), "TestSpawnedThread", &opt); 
  
  mApi->REGISTER_NEW_PARAM_CALLBACK(ModuleTests, new_param_callback);

  wait(sc_core::SC_ZERO_TIME);
  gs::gs_param<std::string> tmp_new_par_test("tmp_new_par_test");
  gs::gs_param<std::string> tmp_new_par_test2("tmp_new_par_test2");

  // tests mApi->removePar
  delete myPubParam; myPubParam = NULL;
  delete myPrivParam; myPrivParam = NULL;
  
  
  std::cout <<std::endl<< "***** END   "<< name() <<" main_action *****" << std::endl<<std::endl;
}

void ModuleTests::new_param_callback(const std::string parname, const std::string value) {
  std::cout << "  "<<name()<<": new param callback" <<std::endl;
  std::cout << "    parname = " << parname << std::endl;
  std::cout << "    value   = " << value   << std::endl;
  if (mApi->getPar(parname) == NULL) {
    std::cout << "    implicit" << std::endl;
  } else {
    std::cout << "    explicit" << std::endl;
  }
}

void ModuleTests::spawned_thread() {
  std::cout << "  " << name() << ": spawned method called: one or more new parameter(s) added" << std::endl;
}

#endif
