//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2009 : GreenSocs Ltd
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

#ifndef __Stdout_OutputPlugin_h__
#define __Stdout_OutputPlugin_h__

#include <time.h>
#include <stdio.h>
#include "OutputPlugin_base.h"
#include "gav_plugin.h"

namespace gs {
namespace av {
      
 
  GAV_REGISTER_PLUGIN(3, STDOUT_OUT, Stdout_OutputPlugin)
 
  /// Output Plugin which observes a GreenConfig gs_param parameter and prints out changes to the standard output stdout
  /**
   *
   */
  class Stdout_OutputPlugin 
  : public OutputPlugin_base {
    
  public:
    
    /// Constructor
    /**
     * @param unused unused
     * @param ev_listn  Pointer to event listener that may be used by this.
     */
    Stdout_OutputPlugin(const char* unused, event_listener<OutputPlugin_base> *ev_listn)
    : OutputPlugin_base(unused, ev_listn, STDOUT_OUT)
    {
      GAV_DUMP_N(name(), "Constructor");
    }
    
  protected:
    /// Init function called on first usage, not called during construction!
    /** Here nothing needs to be initialized */
    void init() {
    }
  public:
    
    /// Destructor
    ~Stdout_OutputPlugin() {
    }

    /// Callback function
    void config_callback(gs_param_base &par) {
      // check for destructing parameters: remove them from callback list if removed
      if (par.is_destructing()) {
        remove(par);
      }
      // only perform output action when not paused (or stopped)
      else if (is_running) {
        printf("@%s /%d (%s): ", sc_core::sc_time_stamp().to_string().c_str(), (unsigned)sc_core::sc_delta_count(), name());
        printf("%s = %s", par.getName().c_str(), par.getString().c_str()); 
        printf("\n");
      }
    }
    
  protected:
    
  };
  
  
} // end namespace av
} // end namespace gs

#endif
