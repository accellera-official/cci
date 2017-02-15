


#ifndef CCI_CCI_PRIVATE_BROKER_H_INCLUDED_
#define CCI_CCI_PRIVATE_BROKER_H_INCLUDED_

#include "cci_cfg/cci_cfg_broker.h"
#include "cci_cfg/cci_broker_manager.h"
#include <set>
CCI_OPEN_NAMESPACE_

//  class cci_param_untyped_handle;
//  class cci_param_if;
  
  /// (Non-private) broker implementation
  /**
   * 
   * Global default Broker Implementation
   * See cci_broker_if for details of the implemented API.
   * 
   */
  class cci_cfg_private_broker :
    public cci_cfg_broker,
    public cci_broker_manager
  {
  public:
// a set of perameters that should be exposed up the broker stack
    std::set<std::string> expose;

  public:

    cci_broker_handle create_broker_handle(
            const cci_originator& originator);

    cci_originator get_latest_write_originator(
            const std::string &parname) const;

    /// Constructor
    cci_cfg_private_broker(const std::string& name);

    /// Destructor
    ~cci_cfg_private_broker();

    /// Return the name of the broker
    const cci_value get_initial_cci_value(const std::string &parname);

   /// Return the name of the broker
    void set_initial_cci_value(const std::string &parname,
                               const cci_value &cci_value,
                               const cci_originator& originator);

    /// Lock parameter
    void lock_initial_value(const std::string &parname);

    /// Get current cci_value
    const cci_value get_cci_value(const std::string &parname);
    
    /// return a handle with which to access a parameter
    cci_param_untyped_handle get_param_handle(const std::string &parname,
                                              const cci_originator& originator);

    bool param_exists(const std::string &parname);
    
    bool is_used(const std::string &parname);

    void add_param(cci_param_if* par);

    void remove_param(cci_param_if* par);
    
  };

CCI_CLOSE_NAMESPACE_
#endif

