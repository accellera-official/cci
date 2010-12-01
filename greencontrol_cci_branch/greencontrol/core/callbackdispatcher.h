//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Michael Ruetz <m.ruetz@tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doxygen comments

#ifndef __CALLBACKDISPATCHER_H__
#define __CALLBACKDISPATCHER_H__

#include "initialize_if.h"

namespace gs {
namespace ctr {

/// This class handles the SystemC callbacks and passes them to the connected objects.
class CallbackDispatcher : public sc_module
{
public:

  /// Constructor.
  CallbackDispatcher()
  : sc_module(sc_module_name("CallbackDispatcher"))
  {
  }

  /// Destructor.
  ~CallbackDispatcher() {}

  /// Add a module to the CallbackDispatcher.
  void add(initialize_if* callbMod)
  {
    initVec.push_back(callbMod);
  }

  /// Remove a module from the CallbackDispatcher.
  void remove(initialize_if* callbMod)
  {
    // iterator is increased by hand in for-loop, because erase() invalidates the iterator, but returns an iterator for the next element
    for(vector<initialize_if*>::iterator it = initVec.begin(); it != initVec.end(); )
    {
      if(*it == callbMod)
        it = initVec.erase(it);
      else
        ++it;
    }
  }

  /// Kernel callback: start and stop initialize-mode.
  /**
   * start_of_simulation kernel callback
   *
   * Inform all APIs to start their initial configuration.
   * After that inform all APIs to leave the initialize-mode
   */
  virtual void start_of_simulation()
  {
    initialize_if* pIF;
    sc_object* pObj;

    GC_PRINT_SPACE;
    GC_DUMP_N(name(), "start_of_simulation, inform all APIs to start initial configuration: ");

    for(unsigned int i=0;i<initVec.size();i++)
    {
      pIF = initVec[i];
      pObj = dynamic_cast<sc_object*>(pIF);
      if(pObj) {
        GC_DUMP_N(name(), "                   start_initialize_mode() "<<pObj->name());
      }
      pIF->start_initialize_mode();
      if(pObj) {
        GC_DUMP_N(name(), "                   gc_start_of_simulation() "<<pObj->name());
      }
      pIF->gc_start_of_simulation();
    }

    GC_DUMP_N(name(), "Inform all APIs to leave initialize-mode: ");
    for(unsigned int i=0;i<initVec.size();i++)
    {
      pIF = initVec[i];
      pObj = dynamic_cast<sc_object*>(pIF);
      if(pObj) {
        GC_DUMP_N(name(), "                   end_initialize_mode() "<<pObj->name());
      }
      pIF->end_initialize_mode();
    }

    GC_DUMP_N(name(), "        ------------------------------------------------------------------------");
    GC_DUMP_N(name(), "        ------------------------------------------------------------------------");
    GC_DUMP_N(name(), "        -----------------------   start_of_simulation   ------------------------");
    GC_DUMP_N(name(), "        ------------------------------------------------------------------------");
    GC_DUMP_N(name(), "        ------------------------------------------------------------------------");
  }
  
  /// Kernel callback: just forward
  /**
   * end_of_elaboration kernel callback
   *
   * Just forwarded to the implementing classes.
   */
  virtual void end_of_elaboration() {
    initialize_if* pIF;
    sc_object* pObj;

    GC_DUMP_N(name(), "Call gc_end_of_elaboration on all APIs: ");
    for(unsigned int i=0;i<initVec.size();i++)
    {
      pIF = initVec[i];
      pObj = dynamic_cast<sc_object*>(pIF);
      if(pObj) {
        GC_DUMP_N(name(), "                   gc_end_of_elaboration() "<<pObj->name());
      }
      pIF->gc_end_of_elaboration();
    }
  } 

  /// Kernel callback: just forward
  /**
   * before_end_of_elaboration kernel callback
   *
   * Just forwarded to the implementing classes.
   */
  virtual void before_end_of_elaboration() {
    initialize_if* pIF;
    sc_object* pObj;
    
    GC_DUMP_N(name(), "Call gc_before_end_of_elaboration on all APIs: ");
    for(unsigned int i=0;i<initVec.size();i++)
    {
      pIF = initVec[i];
      pObj = dynamic_cast<sc_object*>(pIF);
      if(pObj) {
        GC_DUMP_N(name(), "                   gc_before_end_of_elaboration() "<<pObj->name());
      }
      pIF->gc_before_end_of_elaboration();
    }
  } 
  
  /// Kernel callback: just forward
  /**
   * end_of_simulation kernel callback
   *
   * Just forwarded to the implementing classes.
   */
  virtual void end_of_simulation() {
    initialize_if* pIF;
    sc_object* pObj;
    
    GC_DUMP_N(name(), "Call gc_end_of_simulation on all APIs: ");
    for(unsigned int i=0;i<initVec.size();i++)
    {
      pIF = initVec[i];
      pObj = dynamic_cast<sc_object*>(pIF);
      if(pObj) {
        GC_DUMP_N(name(), "                   gc_end_of_simulation() "<<pObj->name());
      }
      pIF->gc_end_of_simulation();
    }
  } 

protected:
  /// Vector with pointers to the APIs.
  vector<initialize_if*> initVec;
};


} // end namespace ctr
} // end namespace gs

#endif
