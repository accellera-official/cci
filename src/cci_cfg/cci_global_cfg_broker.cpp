#include "cci_cfg/cci_cfg_broker.h"
#include "cci_cfg/cci_config_macros.h"
#include "cci_cfg/cci_originator.h"

CCI_OPEN_NAMESPACE_

/// Internal pointer to the one global default fallback broker instance 
/// used in the case the appropriate broker is not known (or is this one)
cci_broker_if* singleton_broker = NULL;
// We could send this name through cci_gen_unique_name, but if it's not unique,
// somebody is playing games, and we should fail anyway!

/**
 * Creates or returns the one non-private global config broker provided
 * by the broker implementation. Called by the header function
 * get_broker. NEVER call as a user!
 *
 * This returns the raw broker, not a handle, thus this
 * shall not be returned directy to the user!
 * @return The one non-private global config broker (not wrapped with a
 * handle)
 */

/* Not in a header file, not available to be used*/

/* anonymous */ namespace {
class cci_cfg_private_global_broker : public cci_cfg_broker
{
  friend cci_broker_handle cci::cci_get_global_broker(const cci_originator &);
  explicit cci_cfg_private_global_broker(const std::string& nm) : cci_cfg_broker(nm) {}

  virtual bool is_global_broker() const {return true; }
};
} // anonymous namespace


cci_broker_handle cci_get_global_broker(const cci_originator &originator) {
  if (!singleton_broker)
    singleton_broker = new cci_cfg_private_global_broker(CCI_DEFAULT_BROKER_STRING_);
  return singleton_broker->create_broker_handle(originator);
}

CCI_CLOSE_NAMESPACE_
