//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>,
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

/*
 DOXYGEN usage in this project:
 
 All comments: 
 - Begin with capital letter,
 - End with '.'
 
 classes and methods:
 /// Brief description.
 / **
 *   Detailled description.
 * /
 
 Special tags in detailled description of methods:
 * @param paraname Description of parameter.
 * @return Description of Return value.
 
 - Additional comments in code (not for doxygen): //
 - Optical separators (not for doxygen):
 // /////////////////// Description //////////////////
 
 */



// /**
//  * @mainpage GreenSocs GreenControl Framework
//  * This is the GreenSocs GreenControl Model-Middleware-Framework.<br>
//  *
//  * Developed by: Christian Schroeder<br>
//  * and Wolfgang Klingauf<br>
//  * with Robert Guenzel, Mark Burton, Marcus Bartholomeu, Eric Roesler, Michael Ruetz and others.
//  */


#ifndef __GC_CORE_H__
#define __GC_CORE_H__


#include "gc_globals.h"

#include <list>
#include <vector>

#include "gc_port_if.h"
#include "controladdressmap.h"
#include "callbackdispatcher.h"
#include "initialize_if.h"
#include "helpfunctions.h"

#include "greencontrol/gclog/gc_logger_if.h"


namespace gs {
namespace ctr {

// forward declarations for typedef
template <typename gc_port_T> class GC_Core_T;
class gc_port;

/// GC_Core
typedef GC_Core_T<gc_port> GC_Core;

/// Central routing core of the GreenControl framework.
/**
 * GreenControl Core which routes the transactions between the IPs 
 * (controlled modules or controlling tools) and the plugins
 *
 * The Core routes the transactions immediately.
 */
template <typename gc_port_T> // gc_port uses GC_Core, this template resolves cyclic includes
class GC_Core_T
  : public gc_port_if
{
protected: 

  // Friend class to allow it to get access to the core (to register at callback dispatcher)
  friend class initialize_if_T<GC_Core>;
  
  /// target address map 
  ControlAddressMap m_AddressMap;

  /// String containing the name of the core
  const char* m_name;

  /// Callback Dispatcher to inform the APIs about the SystemC callbacks
  CallbackDispatcher m_callbackDisp;

  /// Direct pointer to the GC_Logger instance
  gs::log::GC_Logger_if* m_pLogger;


  /// Returns and sets the Core singleton pointer
  static GC_Core* access_core_singleton(GC_Core* core = NULL, bool set = false) {
    /// Pointer to the Core singleton
    static GC_Core* core_singleton = NULL;
    if (set) {
      assert(!access_core_instantiated() || core == NULL);
      assert(core_singleton == NULL || core == NULL);
      core_singleton = core;
      access_core_instantiated(true);
    }
    return core_singleton;
  }

  /// If the Core has been instantiated, stays true when Core is destroyed
  static bool access_core_instantiated(bool set = false) {
    /// Instantiated status
    static bool core_instantiated = false;
    if (set) {
      core_instantiated = set;
    }
    return core_instantiated;
  }

  /// Provide a name method, since GC_Core no longer derives from sc_module
  const char* name() const
  {
    return m_name;
  }
  
public:

  /// Static function to get access to the Core singleton
  static GC_Core& get_instance() {
    if (access_core_singleton() == NULL) {
      assert(!access_core_instantiated() && "Core has already been destroyed. Make sure you destroy the core after everything else.");
      GC_DUMP_N("static GC_Core::get_instance()", "Create not yet existing Core.");      
      new GC_Core(); // will register itself with core_singleton 
    }
    return *access_core_singleton();
  }
  
  SC_HAS_PROCESS(GC_Core_T);

  /// Constructor
  GC_Core_T()
  : m_name("GC_Core")
  , m_pLogger(NULL)
  {
    GC_DUMP_N(name(), "I am a GC Core.");
    
    // ensure singleton
    if (access_core_singleton() != NULL) {
      SC_REPORT_FATAL(name(), "Core has already been instantiated! Make sure a manual construction is done before a possible automatic construction.");
    }
    assert(access_core_singleton() == NULL && "Core has already been instantiated! Make sure a manual construction is done before a possible automatic construction.");
    assert(access_core_instantiated() == false && "Core has already been instantiated and destroyed!");
    access_core_singleton(this, true);
  }

  /// DEPRECATED Constructor
  GC_Core_T(const char* n)
  : m_name("GC_Core")
  , m_pLogger(NULL)
  {
    DEPRECATED_WARNING("GC_Core", "DEPRECATED: GC_Core Constructor with name is deprecated, use without name instead!");
    GC_DUMP_N(name(), "I am a GC Core.");
    
    // ensure singleton
    if (access_core_singleton() != NULL) {
      SC_REPORT_FATAL(name(), "Core has already been instantiated! Make sure a manual construction is done before a possible automatic construction.");
    }
    assert(access_core_singleton() == NULL && "Core has already been instantiated! Make sure a manual construction is done before a possible automatic construction.");
    assert(access_core_instantiated() == false && "Core has already been instantiated and destroyed!");
    access_core_singleton(this, true);
  }
  
  
  /// Destructor warns if there are still Plugins or APIs existing
  ~GC_Core_T()
  {
    GC_DUMP_N(name(), "Make test for still existing gc_ports.");

    // This is only a convenience security check to prevent user errors
    // TODO: Should be SC_REPORT_FATAL, if there are still any APIs/Plugins connected to the core, but GCnf_Api_t isn't destroyed,
    // so the m_AddressMap won't be empty and the Error/Warning gets thrown every time.
    //if(!m_AddressMap.isEmpty())
    //  SC_REPORT_WARNING(name(), "GC_Core destructed before other GreenControl elements! To asure that no GreenControl element uses the Core, take care of the coding style to construct first the Core, then the Plugins and destruct them in reverse order.");

    access_core_singleton(NULL, true);
  }
  
  // ///////////////////////////////////////////////////////////////////////

  /// Binds a gc_port to the GC_Core.
  /**
   * The gc_port gets added to the AddressMap of the Core. If the port belongs to an API, it is added to the CallbackDispatcher.
   * If the port belongs to the Logger-Plugin, it gets handled accordingly.
   */
  void bind(gc_port_T* pPort)
  {
    assert(pPort != NULL && "gc_port must not be NULL!");
    assert(pPort->getSupportedControlService() != EXTENDED_SERVICE && "Use other constructor and provide the extended service ID!");
    assert(pPort->getSupportedControlService() != NO_SERVICE && "Provide a valid service ID!");

    m_AddressMap.addPort(pPort);

    initialize_if* pIF;
    // add only APIs to the CallbackDispatcher
    if(!pPort->isPlugin())
    {
      // check if the API actually implements initialize_if
      pIF = dynamic_cast<initialize_if*>(pPort->getParent());
      if(pIF)
        m_callbackDisp.add(pIF);
      else
      {
        GC_DUMP_N(name(), "API " << pPort->getParentName().c_str() << " doesn't implement the initialize_if interface.");
      }
    }
    else
    {
      // check if Plugin is GC_Logger
      if(pPort->getSupportedControlService() == LOG_SERVICE)
        m_pLogger = dynamic_cast<gs::log::GC_Logger_if*>(pPort->getParent());
    }
  }

  /// Unbinds a gc_port from the Core.
  /**
   * If an API or Plugin is destroyed before the Core, it has to unbind itself, so the Core won't call an invalid port.
   */
  void unbind(gc_port_T* pPort)
  {
    m_AddressMap.removePort(pPort);

    // remove plugins from the CallbackDispatcher
    initialize_if* pIF;
    if(!pPort->isPlugin())
    {
      pIF = dynamic_cast<initialize_if*>(pPort->getParent());
      if(pIF)
        m_callbackDisp.remove(pIF);
    }
    else
    {
      // disable logger if necessary
      if(pPort->getSupportedControlService() == LOG_SERVICE)
        m_pLogger = NULL;
    }
  }

  /// Gets incoming transaction from gc_ports.
  /**
   * The incoming transaction is forwarded to the correct destination module.
   * If the mTarget field is '0' the routing is done with the mService field.
   */
  void transport(ControlTransactionHandle& tr)
  {
    std::vector<cport_address_type> addrVec;
    cport_address_type targetAddr;

    GC_DUMP_N(name(), "got transaction atom from master"); 
#ifdef GC_VERBOSE
    show_pure_vector(m_AddressMap.getName( tr->get_mID(), tr->get_mService()), 1);
    std::cout << std::endl;
#endif
    GC_DUMP_N(name(), "  received transaction: "<<tr->toString().c_str());      
    // TODO:
    // Check Design Rules:
    // e.g.: CMD_ADD_PARAM may only be used by parameter owning module (/API)

    // forward the transaction to the target
    GC_DUMP_N(name(), "forward transaction to slave"); 
#ifdef GC_VERBOSE
    show_pure_vector(m_AddressMap.getName( tr->get_mTarget(), tr->get_mService()), 1);
    std::cout << std::endl;
#endif


    // log the transaction
    if(m_pLogger)
      m_pLogger->log(tr);

    // transport transaction to the target
    targetAddr = tr->get_mTarget();
    if(targetAddr) // use address for direct communication
    {
      // check if target is (still) existing (could be destroyed in the meantime)
      if(m_AddressMap.is_in_maps(targetAddr))
        targetAddr->transport(tr);
      else
      {
        std::stringstream tmpStr;
        tmpStr << "Sender (" << tr->getSenderName() << ") is trying to reach a non existing target.";
        SC_REPORT_WARNING(name(), tmpStr.str().c_str());
        tr->set_mError(666); // GreenControl Core Error
      }

    }
    else // got no target address, use plugin id with map
    {
      addrVec = m_AddressMap.decode(tr->get_mService());
      if (addrVec.size() == 0) // set error if no target (e.g. plugin not available)
        tr->set_mError(666); // GreenControl Core Error
      for(unsigned int i=0;i<addrVec.size();i++)
        addrVec[i]->transport(tr);
    }

    // log the returning transaction
    if(m_pLogger)
      m_pLogger->logReturn(tr);
  }
  
  /// Returns the callback dispatcher so that any initialize_if can register for being called
  /*
   * Important: Make sure an object being added is removed again before being destroyed
   *
   * @return Callback dispatcher
   */
  CallbackDispatcher& get_CallbackDispatcher() {
    return m_callbackDisp;
  }
  
};


} // end namespace ctr
} // end namespace gs

#include "gc_port.h"

#endif
