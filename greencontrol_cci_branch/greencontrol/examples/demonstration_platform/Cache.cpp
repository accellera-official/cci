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

#include "Cache.h"

bool Cache::read(std::vector<gs_uint8> &data, const gs_uint32 addr, const gs_uint32 length) {
  if (enable_cache) {
    // read from cache
    for (unsigned int i = 0; i < length; i++) {
      if (!cache.read(data[i], addr+i))
        return false;
    }
  } else {
    // read directly
    return master_port.read(data, addr, length);
  }
  return true;
}
  
bool Cache::write(const std::vector<gs_uint8> &data, const gs_uint32 addr, const gs_uint32 length) {
  if (enable_cache) {
    // write to cache
    for (unsigned int i = 0; i < length; i++) {
      if (!cache.write(data[i], addr+i))
        return false;
    }
  } else {
    // write directly
    return master_port.write(data, addr, length);
  }
  return true;
}

void Cache::parameter_changed_callback(gs::gs_param_base& par) {
  if (! par.is_destructing()) {
    DEMO_TRACE(sc_core::sc_module::name(), "parameter_changed_callback, enable_cache = %u", (unsigned int)enable_cache);
    // notify cache to make write backs and reset.
    cache.create_cache_event.notify();
  } 
  else {
    DEMO_TRACE(sc_core::sc_module::name(), "parameter_changed_callback, enable_cache param is destructing!")
  }
}
