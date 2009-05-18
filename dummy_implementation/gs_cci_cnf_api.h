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


#ifndef __GS_CCI_CNF_API_H__
#define __GS_CCI_CNF_API_H__


#include <cci.h>


namespace cci {


  class gs_cci_cnf_api
  : public cci_cnf_api
  {
    
  public:
    
    ~gs_cci_cnf_api();

    bool set_init_value(const std::string &parname, const std::string &value);
    
    const std::string get_json_string(const std::string &parname);

    cci_param_base* get_param(const std::string &parname);
    
    bool exists_param(const std::string &parname);
    
    const std::vector<std::string> get_param_list();
    
    boost::shared_ptr< callb_adapt_b> register_callback(const std::string& parname, boost::shared_ptr< callb_adapt_b> callb);

    void unregister_all_callbacks(void* observer);
    
    bool unregister_param_callback(callb_adapt_b* callb);
    
    bool has_callbacks(const std::string& parname);
    
    bool add_param(cci_param_base* par);
    
    bool remove_param(cci_param_base* par);
    
    void set_alias(std::string& orig_parname, std::string& alias_parname);
        
    const std::vector<std::string> get_param_list(const std::string& pattern);
    
    const std::vector<cci_param_base*> get_params(const std::string& pattern = "");

  };

      
} // end namespace cci

#endif
