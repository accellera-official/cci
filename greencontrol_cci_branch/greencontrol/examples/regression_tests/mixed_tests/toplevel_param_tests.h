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

// doxygen comments

#ifndef __toplevel_param_tests_h__
#define __toplevel_param_tests_h__


//
// These are tests for top-level gs_param that 
// are not located within the SystemC-hierarchy
//


/// Top-level class NOT being a SystemC class
class Toplevel_Param_Tests {
public:

  void main() {
    
    std::cout << std::endl<< std::endl<< "** Test top-level parameters **" << std::endl;

    //gs::gs_param<int> topLevelParam("topLevelParam", 42);
    //std::cout << "create top-level parameter '"<< topLevelParam.getName() <<"'" << std::endl;

    gs::gs_param<int> topLevelParam("Toplevel_Param_Tests", 42);
    std::cout << "create top-level parameter '"<< topLevelParam.getName() <<"'" << std::endl;

    gs::gs_param<int> topLevelParam2("toplevel.sub.topLevelParam2", 42);
    std::cout << "create top-level parameter '"<< topLevelParam2.getName() <<"'" << std::endl;
    
    // ////////////////////////////////////////////////////////////////////
    // //////// test all API function calls with the top-level params /////
    
    gs::cnf::cnf_api_if *mApi = gs::cnf::GCnf_Api::getApiInstance(NULL);

    // test addParam
    mApi->addParam("addedTopLevelParam", "30");
    
    // test removePar
    gs::gs_param<int> *tmpTopLevelParam = new gs::gs_param<int>("tmpTopLevelParam");
    delete tmpTopLevelParam; tmpTopLevelParam = NULL;
    
    // test setParam
    mApi->setParam("Toplevel_Param_Tests", "40");
    
    // test setInitValue
    mApi->setInitValue("notYetExisting", "intiVal");

    // test getParam
    std::cout << "value of Toplevel_Param_Tests should be 40 == " << mApi->getParam("Toplevel_Param_Tests") << std::endl;
    
    // test getPar
    gs::gs_param_base* p = mApi->getPar("Toplevel_Param_Tests");
    p = p;
    
    // test existsPar
    if (mApi->existsParam("Toplevel_Param_Tests")) std::cout << "existsPar test passed" << std::endl;
    else std::cout << "existsPar test failed" << std::endl;
    if (mApi->existsParam("notYetExisting")) std::cout << "existsPar test passed" << std::endl;
    else std::cout << "existsPar test failed" << std::endl;
    if (!mApi->existsParam("notExisting")) std::cout << "existsPar test passed" << std::endl;
    else std::cout << "existsPar test failed" << std::endl;
    
    // test deprecated getUpdateEvent
    sc_core::sc_event &e = mApi->getUpdateEvent("Toplevel_Param_Tests");
    e.cancel(); // to avoid "unused variable" warning
    
    // test deprecated registerCallback
    mApi->REGISTER_CALLBACK(Toplevel_Param_Tests, config_callback, "Toplevel_Param_Tests")
    
    std::cout << std::endl<< "** Test top-level parameters passed if no errors **" << std::endl<<std::endl;
    
  }
  
  void config_callback(const std::string parname, const std::string value) {
  }
  
};


#endif
