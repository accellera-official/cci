#include "cci_cfg/cci_cfg_broker.h"

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
cci_broker_if& cci_get_global_broker() {
  if (!singleton_broker)
    singleton_broker = new cci_cfg_broker(__CCI_DEFAULT_BROKER_STRING__);
  return *singleton_broker;
}

CCI_CLOSE_NAMESPACE_
