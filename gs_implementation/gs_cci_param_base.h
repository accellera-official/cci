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


#ifndef __GS_CCI_PARAM_BASE_H__
#define __GS_CCI_PARAM_BASE_H__


#include <string>
#include <iostream>
#include <map>
#include <set>
#include <boost/shared_ptr.hpp>

#include "cci.h"

namespace cci {

  class gs_cci_param_base
  : public cci_param_base
  {

  public:

    explicit gs_cci_param_base(const std::string& n, const bool register_at_db = true,
                               const bool force_top_level_name = false);
    
    ~gs_cci_param_base();
    
    bool set_string(const std::string& str);
    
    const std::string& get_string() const;
    
    const Param_type get_type() const;
    
    const std::string get_type_string() const;
    
    const std::string& get_name() const;
   
    const bool is_destructing() const;

    boost::shared_ptr<callb_adapt_b> register_callback(boost::shared_ptr< callb_adapt_b> callb);
    
    void unregister_all_callbacks(void* observer);
    
    bool unregisterParamCallback(callb_adapt_b* callb);
    
    bool has_callbacks();
    
  protected:
    std::string parname;
  };

} // namespace cci

#endif
