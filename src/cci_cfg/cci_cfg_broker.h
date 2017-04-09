
#ifndef CCI_CCI_BROKER_H_INCLUDED_
#define CCI_CCI_BROKER_H_INCLUDED_

#include "cci_cfg/cci_broker_if.h"
#include "cci_cfg/cci_broker_handle.h"
#include <map>
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
  class cci_cfg_broker: public cci_broker_if
  {
  public:
    cci_broker_handle create_broker_handle(
            const cci_originator& originator);

    cci_originator get_latest_write_originator(
            const std::string &parname) const;

    /// Constructor
    explicit cci_cfg_broker(const std::string& name);

    /// Destructor
    ~cci_cfg_broker();

    /// Return the name of the broker
    const std::string &name() const;

    /// Return the name of the broker
    const cci_value get_initial_cci_value(const std::string &parname);

   /// Return the name of the broker
    void set_initial_cci_value(const std::string &parname,
                               const cci_value &cci_value,
                               const cci_originator& originator);

    /// Lock parameter
    void lock_initial_value(const std::string &parname);

    /// Get a full list of unconsumed initial values.
    std::vector<cci_name_value_pair> get_unconsumed_initial_values();

    /// get vector of initial values
    cci_initial_value_range get_unconsumed_initial_values(
            const cci_initial_value_predicate &pred);

    void ignore_unconsumed_initial_values(
            const cci_initial_value_predicate &pred);

    /// Get current cci_value
    const cci_value get_cci_value(const std::string &parname);
    
    /// return a handle with which to access a parameter
    cci_param_untyped_handle get_param_handle(const std::string &parname,
                                              const cci_originator& originator);

    const std::vector<cci_param_untyped_handle> get_param_handles(
            const cci_originator& originator);

    cci_param_range get_param_handles(cci_param_predicate& pred,
                                      const cci_originator& originator);
    
    bool param_exists(const std::string &parname);
    
    bool is_used(const std::string &parname);

    void add_param(cci_param_if* par);

    void remove_param(cci_param_if* par);
    
    bool is_global_broker() const;
    bool is_private_broker() const;

    cci_param_create_callback_handle
    register_create_callback(const cci_param_create_callback& cb,
                             const cci_originator& orig);

    bool
    unregister_create_callback(const cci_param_create_callback_handle& cb,
                               const cci_originator& orig);

    cci_param_destroy_callback_handle
    register_destroy_callback(const cci_param_destroy_callback& cb,
                              const cci_originator& orig);

    bool
    unregister_destroy_callback(const cci_param_destroy_callback_handle& cb,
                                const cci_originator& orig);

    bool unregister_all_callbacks(const cci_originator& orig);

    bool has_callbacks() const;
    
  protected:
    /// Get original parameter (internal method)
    cci_param_if* get_orig_param(const std::string &parname) const;

    /// Get parameter handle (internal method)
    cci_param_untyped_handle get_param_handle(
            cci_param_if& orig_param,
            const cci_originator& originator) const;

    std::string m_name;
    const cci_originator m_originator;    

    std::map<std::string, cci_param_if*> m_param_registry;
    std::map<std::string, cci_value> m_unused_value_registry;
    std::map<std::string, cci_value> m_used_value_registry;
    
    // store the list of locked param's (guessing there are many fewer of these
    // than actual params, so hopefully this is more efficient than adding a
    // boolean above)
    std::set<std::string> locked;

    /// Map to save the latest write originator for implicit param sets
    /// Typedef for internal implicit param originator map
    std::map<std::string, cci_originator> m_implicit_originator_map;

    template<class T>
    struct callback_obj {
      callback_obj(T cb, const cci_originator& orig):
              callback(cb), originator(orig) {}
      T callback;
      cci_originator originator;
    };

    typedef callback_obj<typename cci_param_create_callback_handle::type>
          create_callback_obj_t;

    /// Create callbacks
    std::vector<create_callback_obj_t> m_create_callbacks;

    typedef callback_obj<typename cci_param_destroy_callback_handle::type>
          destroy_callback_obj_t;

    /// Destroy callbacks
    std::vector<destroy_callback_obj_t> m_destroy_callbacks;

    /// Ignored unconsumed initial cci values
    std::vector<std::string> m_ignored_unconsumed_initial_cci_values;
  };

CCI_CLOSE_NAMESPACE_
#endif
