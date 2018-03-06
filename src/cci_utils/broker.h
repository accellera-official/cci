#ifndef CCI_UTILS_BROKER_H_INCLUDED_
#define CCI_UTILS_BROKER_H_INCLUDED_

#include "cci_utils/consuming_broker.h"
#include "cci_cfg/cci_broker_manager.h"
#include <set>

namespace cci_utils 
{

  /// (Non-private) broker implementation
  /**
   * 
   * Global default Broker Implementation
   * See cci_broker_if for details of the implemented API.
   * 
   */
  class broker : public consuming_broker
  {
  public:
// a set of perameters that should be exposed up the broker stack
    std::set<std::string> expose;

  private:
    /// for the public broker, this will be useless, but if people re-use this
    /// broker, then it will help
    bool has_parent;
    cci::cci_broker_if &m_parent;

    // convenience function for constructor
    cci::cci_broker_if &get_parent_broker()
    {
        if (sc_core::sc_get_current_object()) {
            has_parent=true;
            return unwrap_broker(cci::cci_get_broker());
        } else {
          // We ARE the global broker
          has_parent=false;
          return *this;
        }
    }

    bool sendToParent(const std::string &parname) const;

  public:

    cci::cci_originator get_latest_write_originator(
            const std::string &parname) const;

    /// Constructor
    broker(const std::string& name);

    /// Destructor
    ~broker();

    /// Return the preset value of a parameter (by name)
    cci::cci_value get_preset_cci_value(const std::string &parname) const;

    /// Set the preset value of a parameter (by name, requires originator)
    void set_preset_cci_value(const std::string &parname,
                              const cci::cci_value &cci_value,
                              const cci::cci_originator& originator);

    /// Lock parameter
    void lock_preset_value(const std::string &parname);

    /// Get current cci_value
    cci::cci_value get_cci_value(const std::string &parname) const;
    
    /// return a handle with which to access a parameter
    cci::cci_param_untyped_handle get_param_handle(const std::string &parname,
                                                   const cci::cci_originator& originator) const;

    /// return a list of all the params that the originator can see from either
    /// the private broker, or from up the broker stack.
    std::vector<cci::cci_param_untyped_handle> get_param_handles(const cci::cci_originator& originator) const;

    void add_param(cci::cci_param_if* par);

    void remove_param(cci::cci_param_if* par);

    bool is_global_broker() const;
    
  };

}
#endif

