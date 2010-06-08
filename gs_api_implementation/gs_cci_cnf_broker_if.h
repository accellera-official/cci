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


#ifndef __GS_CCI_CNF_BROKER_IF_H__
#define __GS_CCI_CNF_BROKER_IF_H__

namespace cci {
  namespace cnf {
  
  class  gs_cci_cnf_broker_if {
  public:

    virtual ~gs_cci_cnf_broker_if() { }
    
    virtual gs::cnf::cnf_api* get_gcnf_api() = 0;
    
  };
  
  } // end namespace
} // end namespace

#endif
