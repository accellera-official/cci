//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#ifndef __CACHE_H__
#define __CACHE_H__

#include "demo_globals.h"

// SimpleBus API
#include "greenbus/api/simplebusAPI.h"

// GreenControl and GreenConfig
#include "greencontrol/config.h"

// Cache
#include "lru_cache.h"

/// Cache for demonstration platform.
/**
 * Has a SimpleBus slave port and implements simplebus_if for 
 * read and write methods to receive transactions from TG.
 *
 * Has a SimpleBus master port to send transactions to the devices.
 *
 * Uses an LRU_cache to read and write word oriented.
 *
 * The parameter enable_cache may be used during runtime to enable
 * and disable the LRU cache. After changing that parameter the callback
 * function is called. It notifies an event in the LRU cache which
 * results in resetting the cache (including write back if needed and
 * creating new cache lines for enabling a new cache). This reset needs 
 * some time if write backs have to be done! Especially when disabling
 * the LRU cache wait some time until the write back is done.
 *
 * Make cache test with module TG (see define MAKE_CACHE_TEST)
 */
class Cache
: public sc_core::sc_module,
  public virtual tlm::simplebus_if
{
public:
  /// SimpleBus slave port
  tlm::simplebusSlavePort slave_port;
  /// SimpleBus master port
  tlm::simplebusMasterPort master_port;

private:
  gs::gs_param<bool> enable_cache;
  
public:

  GC_HAS_CALLBACKS();
  
  SC_HAS_PROCESS(Cache);
  
  /// Constructor
  Cache(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      slave_port("tport"),
      master_port("iport"),
      enable_cache("enable_cache", true),
      cache(sc_core::sc_module_name("LRUCache"), &master_port)
  { 
    slave_port.slave_port(*this);
    slave_port.base_addr = tlm::MAddr(0x00000000);
    slave_port.high_addr = tlm::MAddr(0xFFFFFFFF);

    // register callback
    GC_REGISTER_PARAM_CALLBACK(&enable_cache, Cache, parameter_changed_callback);
  }
  
  ~Cache() {
    GC_UNREGISTER_CALLBACKS();
  }
  
  /// Read data from memory (@see simplebus_if)
  virtual bool read(std::vector<gs_uint8> &data, const gs_uint32 addr, const gs_uint32 length);
  
  /// Write data to memory (@see simplebus_if)
  virtual bool write(const std::vector<gs_uint8> &data, const gs_uint32 addr, const gs_uint32 length);

  /// Callback functions for changed enable_cache-parameter
  /**
   * Write back all lines and disable or enable cache.
   */
  void parameter_changed_callback(gs::gs_param_base &par);

private:
  /// LRU Cache
  LRU_cache cache;

};


#endif
