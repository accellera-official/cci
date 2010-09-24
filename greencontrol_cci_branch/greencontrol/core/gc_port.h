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

#ifndef __GC_PORT_H__
#define __GC_PORT_H__

#include "gc_globals.h"        // global settings

#include "transport/gc_memoryutils.h"

#include "gc_port_if.h"
#include "gc_core.h"

namespace gs {
namespace ctr {


/// Port to the GreenControl Core.
/**
 * Class gc_port is the port to the GreenControl Core.
 *
 * Modules using it will have to implement gc_port_if to handle 
 * incoming transactions. Outgoing transactions can be sent with 
 * transport(transaction).
 * 
 * The port will be bound automatically immediately by obtaining 
 * the Control Core via its get_instance() method and calling the 
 * bind() method.
 */
class gc_port
{
public:

  /// Port constructor for the standard services.
  /**
   * Constructor with information to be saved and to be read of core.
   *
   * Initiates automatic binding of the port by calling bind() in the Control Core (which is obtained by get_instance()).
   *
   * @param supportedService    Used by the plugin to announce the supported service.
   * @param api_OR_plugin_name  Name of the connected plugin or API, used for debug and future use.
   * @param isPlugin            Whether the connected module is a plugin.
   * @param pParent             Address to the parent API or plugin.
   */
  gc_port(ControlService_enum supportedService, const std::string api_OR_plugin_name, bool isPlugin, gc_port_if* pParent)
    : m_pParent(pParent)
    , m_pCommand(NULL)
    , m_supportedControlService(supportedService)
    , m_parentName(api_OR_plugin_name)
    , m_isPlugin(isPlugin)
  {
    assert(supportedService != EXTENDED_SERVICE && "Use other constructor and provide the extended service ID!");
    assert(supportedService != NO_SERVICE && "Provide a valid service ID!");

    // set command_if* to parent if available
    m_pCommand = dynamic_cast<command_if*>(pParent);

    // ensure Core is existing
    m_pCore = &GC_Core::get_instance();

    // bind the port to the core
    if(m_pCore)
      m_pCore->bind(this);
    else
      SC_REPORT_ERROR(m_parentName.c_str(), "Cannot locate GC_Core instance in simulation context.");
  }

  /// Port constructor for extended services.
  /**
   * Constructor with information to be saved and to be read of core.
   *
   * Initiates automatic binding of the port by calling bind() in the Control Core (which is obtained by get_instance()).
   *
   * @param supportedService    Used by the plugin to announce the supported service. For Extended Services.
   * @param api_OR_plugin_name  Name of the connected plugin or API, used for debug and future use.
   * @param isPlugin            Whether the connected module is a plugin.
   * @param pParent             Address to the parent API or plugin.
   */
  gc_port(gc_service_ID_extension_base& supportedService, const std::string api_OR_plugin_name, bool isPlugin, gc_port_if* pParent)
    : m_pParent(pParent)
    , m_pCommand(NULL)
    , m_supportedControlService(supportedService.get_Service_ID())
    , m_parentName(api_OR_plugin_name)
    , m_isPlugin(isPlugin)
  {
    // set command_if* to parent if available
    m_pCommand = dynamic_cast<command_if*>(pParent);
    
    // ensure Core is existing
    m_pCore = &GC_Core::get_instance();
    
    // bind the port to the core
    if(m_pCore)
      m_pCore->bind(this);
    else
      SC_REPORT_ERROR(m_parentName.c_str(), "Cannot locate GC_Core instance in simulation context.");
  }
  
  /// Destructor unbinds the port automatically from the Core.
  ~gc_port()
  {
    m_pCore->unbind(this);
  }

  /// Create a transaction.
  /**
   * Create a transaction out of pool.
   */
  ControlTransactionHandle createTransaction()
  {
    ControlTransactionHandle sp_tmp = m_transactionFactory.create();
    sp_tmp->set_mID(m_pParent);
    sp_tmp->set_mCmdIf(m_pCommand);
    return sp_tmp;
  }

  /// Provides access to the Core.
  gc_port_if* operator->()
  {
    return m_pCore;
  }

  /// Returns a pointer to the parent API or Plugin.
  gc_port_if* getParent()
  {
    return m_pParent;
  }

  /// Returns the supported ControlService.
  ControlService getSupportedControlService()
  {
    return m_supportedControlService;
  }

  /// Get the name of the parent.
  std::string getParentName()
  {
    return m_parentName;
  }

  /// Returns whether the port belongs to an API or a plugin.
  bool isPlugin()
  {
    return m_isPlugin;
  }

protected:

  /// Pointer to the parent API or Plugin.
  gc_port_if* m_pParent;

  /// Pointer to the parent API or Plugin, if the owner implements command_if, so a newly created transaction can be set accordingly.
  command_if* m_pCommand;

  /// Pointer to the GC_Core.
  GC_Core* m_pCore;

  /// Supported ControlService.
  ControlService m_supportedControlService; // needed for binding to GreenControl Core

  /// Name of the parent API or Plugin (for debugging).
  std::string m_parentName;

  /// If the owner is a plugin (to be read of GC Core).
  bool m_isPlugin;

  /// Factory to create ControlTransactionHandles.
  ObjectFactory<ControlTransactionHandle> m_transactionFactory; // faster factory from GreenBus
};


} // end namespace ctr
} // end namespace gs

#endif
