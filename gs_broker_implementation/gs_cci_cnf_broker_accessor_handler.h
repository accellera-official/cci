// LICENSETEXT
//
//   Copyright (C) 2011 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#define CCI_CNF_SHARE_LIBRARY_MACROS
#include <cci_configuration>
#include "gs_cci_cnf_broker_accessor.h"
#include "gs_cci_broker.h"

#ifndef __GS_CCI_CNF_BROKER_HANDLER_H__
#define __GS_CCI_CNF_BROKER_HANDLER_H__


namespace cci {

  // forward declaration
  class gs_cci_cnf_broker_accessor;
  
  /// Helper class the gs_cci_broker and gs_cci_private_broker derive from
  /**
   * This provides the get_accessor function which returns the appropriate
   * broker accessor for the originator (either newed or from a cache)
   */
  class gs_cci_cnf_broker_accessor_handler
  {
  public:
    
    /// @see cci::cci_cnf_broker_if::get_accessor
    cci_cnf_broker_if& get_accessor(const cci_originator& originator, cci::cci_cnf_broker_if& orig_broker);

  protected:

    /// Map containing all broker accessors for this broker
    std::map<std::string, gs_cci_cnf_broker_accessor> m_broker_accessors;
    
  };
  

} // end namespace

#endif
