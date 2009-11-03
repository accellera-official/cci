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
//   This program is free software.
// 
//   If you have no applicable agreement with GreenSocs Ltd, this software
//   is licensed to you, and you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   If you have a applicable agreement with GreenSocs Ltd, the terms of that
//   agreement prevail.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//   02110-1301  USA 
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
//  * @mainpage GreenSocs GreenControl framework
//  * This is the GreenSocs GreenControl framework.
//  */


#ifndef __GC_CORE_H__
#define __GC_CORE_H__


#include "gc_globals.h"

#include <list>
#include <vector>

#include "controlports.h"
#include "controladdressmap.h"
#include "gc_core_if.h"
#include "initialize_if.h"
#include "helpfunctions.h"


namespace gs {
namespace ctr {

using std::list;
using std::vector;

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
: public sc_core::sc_module,
  public GC_Core_if   // @see gc_port.h
{
  
  typedef sc_export<payload_event_queue_if<ControlTransactionContainer> > * SourcePort;
  typedef sc_export<payload_event_queue_if<ControlTransactionContainer> > * DestinationPort;
  
  /// Connector to receive master calls from peq.
  /**
   * The master_connector's notify is called by the peq (where it is registered in GC_Core).
   * This makes the sensitivity of masterAccess dispensable and is FASTER than an event and
   * may be used before start of simulation because it avoids the usage of events.
   */
  class master_connector:public payload_event_queue_output_if<ControlTransactionContainer>{
    public:
      master_connector(GC_Core* boss):m_boss(boss){}
      
      virtual void notify(ControlTransactionContainer& tp){
        m_boss->masterAccess(tp);
      }
      
    protected:
      GC_Core* m_boss;
  };

  /// Connector to receive slave calls from peq.
  /**
   * The slave_connector's notify is called by the peq (where it is registered in GC_Core).
   * This makes the sensitivity of slaveAccess dispensable  is FASTER than an event and
   * may be used before start of simulation because it avoids the usage of events.
   */
  class slave_connector:public payload_event_queue_output_if<ControlTransactionContainer>{
    public: 
      slave_connector(GC_Core* boss):m_boss(boss){}
      
      virtual void notify(ControlTransactionContainer& tp){
        m_boss->slaveAccess(tp);
      }
      
    protected:
      GC_Core* m_boss;
  };
 

public:

  /// control target multiport
  control_target_multi_port     target_multi_port;
  
  /// control initiator multiport
  control_initiator_multi_port  init_multi_port;

protected: 

  /// target address map 
  ControlAddressMap mAddressMap;

  /// list containing pointers to each object which inherits from gc_configurable
  list<sc_object*> mConfigurableList;

  /// list containing pointers to each object which is a gc_port
  list<sc_object*> mGcPortList;

  /// list containing pointers to each object control_target_port or control_initiator_port
  list<sc_object*> mGcControlPortList;

  /// list containing pointers to each object which implements cnf::initialize_if
  list<sc_object*> mGcApiIfList;

  /// Connectors which are called by the peqs of the ports
  master_connector mMConn;
  slave_connector mSConn;
  
  // /**
  // * \brief master <=> port-num map.
  // * This map is needed by the protocol to identify the master destination port of a slave's reply.
  // */
  // std::map<unsigned int, unsigned int> mMasterMap;

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
  
public:

  /// Static function to get access to the Core singleton
  static GC_Core& get_instance() {
    if (access_core_singleton() == NULL) {
      assert(!access_core_instantiated() && "Core has already been destroyed. Make sure you destroy the core after everything else.");
      GC_DUMP_N("static GC_Core::get_instance()", "Create not yet existing Core.");      
      new GC_Core("ControlCore"); // will register itself with core_singleton 
    }
    return *access_core_singleton();
  }
  
  SC_HAS_PROCESS(GC_Core_T);
  
  /// Constructor; bind ports
  GC_Core_T(sc_core::sc_module_name name_)
    : sc_core::sc_module(name_),
      target_multi_port("tport"),
      init_multi_port("iport"),
      mMConn(this),
      mSConn(this)
  {
    GC_DUMP_N(name(), "I am a GC Core.");
    
    // ensure singleton
    if (access_core_singleton() != NULL) {
      SC_REPORT_FATAL(sc_core::sc_module::name(), "Core has already been instantiated! Make sure a manual construction is done before a possible automatic construction.");
    }
    assert(access_core_singleton() == NULL && "Core has already been instantiated! Make sure a manual construction is done before a possible automatic construction.");
    assert(access_core_instantiated() == false && "Core has already been instantiated and destroyed!");
    access_core_singleton(this, true);

    target_multi_port.peq.out_port(mMConn);
    init_multi_port.peq.out_port(mSConn);    
  }
  
  /// Destructor warns if there are still Plugins or APIs existing
  ~GC_Core_T() {
    GC_DUMP_N(name(), "Make test for still existing gc_ports.");      
    // This is only a convenience security check to prevent user errors
    if (  search_for_objects<gc_port>(NULL, this)  ) {
      SC_REPORT_WARNING(name(), "GC_Core destructed before other GreenControl elements! To asure that no GreenControl element uses the Core, take care of the coding style to construct first the Core, then the Plugins and destruct them in reverse order.");
    }
    access_core_singleton(NULL, true);
  }
  
  // ///////////////////////////////////////////////////////////////////////

  /// Gets incoming transaction from masters.
  /**
   * This method starts whenever a master's peq triggers a payload-event
   * over the master_connector.
   *
   * The incoming transaction is forwarded to the correct destination port.
   * If the mTarget field is '0' the routing is done with the mService field.
   */
  void masterAccess(ControlTransactionContainer& t_p)
  {
    ControlTransactionHandle tr = t_p.first;
    ControlPhase ph = t_p.second;
    DestinationPort destinationForward;
    std::vector<unsigned int> int_vec;

    // needs to change, as getName and decode now returns vector
    GC_DUMP_N(name(), "got "<<ph.toString().c_str()<<" atom from master at port number(s) [Name] :-"); 
#ifdef GC_VERBOSE
    show_vector_gc_trace(mAddressMap.decode ( tr->get_mID(), tr->get_mService()), mAddressMap.getName( tr->get_mID(), tr->get_mService()));
#endif
    GC_DUMP_N(name(), "  received transaction: "<<tr->toString().c_str());      
    // TODO:
    // Check Design Rules:
    // e.g.: CMD_ADD_PARAM may only be used by parameter owning module (/API)

    // forward the transaction to the target
    // needs to change, as getName and decode now returns vector
    GC_DUMP_N(name(), "forward transaction to slave at port number(s) [Name] :-"); 
#ifdef GC_VERBOSE
    show_vector_gc_trace( mAddressMap.decode ( tr->get_mTarget(), tr->get_mService() ), mAddressMap.getName( tr->get_mTarget(), tr->get_mService() ));
#endif

    int_vec = mAddressMap.decode(tr->get_mTarget(), tr->get_mService());
    for(unsigned int i=0; i < int_vec.size(); i++ )
    {	
       // this is a master - forward the data to the slave
       destinationForward = init_multi_port.connected_in_ports[int_vec[i]]; // forward a transaction to the mTarget
       (*destinationForward)->notify(t_p, PEQ_IMMEDIATE);
    }

    // Test
    /*
      ControlPhase p2(ControlPhase::CONTROL);
      GC_DUMP_N(name(), "send "<<p2.toString().c_str()<<" atom to slave "<<0);      
      DestinationPort destination2 = init_multi_port.connected_in_ports[0];
      (*destination2)->notify(ControlTransactionContainer(tr, p2), sc_time(2,SC_NS));//SC_ZERO_TIME);
      
      GC_DUMP_N(name(), "send "<<p2.toString().c_str()<<" atom to slave "<<1);  
      destination2 = init_multi_port.connected_in_ports[1];
      (*destination2)->notify(ControlTransactionContainer(tr, p2), sc_time(2,SC_NS));
    */
  }
  
  /// Gets incoming transaction from slaves.
  /**
   * This method starts whenever a slave's peq triggers a payload-event
   * over the slave_connector.
   */
  void slaveAccess(ControlTransactionContainer& t_p)
  {  
    ControlTransactionHandle tr = t_p.first;
    ControlPhase ph = t_p.second;
    DestinationPort destinationForward;
    std::vector<unsigned int> int_vec;

    // needs to change, as getName and decode now returns vector
    GC_DUMP_N(name(), "got "<<ph.toString().c_str()<<" atom from slave at port number(s) [Name] :-");
#ifdef GC_VERBOSE
    show_vector_gc_trace(mAddressMap.decode(tr->get_mTarget(), tr->get_mService()), mAddressMap.getName( tr->get_mTarget(), tr->get_mService() ));
#endif
    GC_DUMP_N(name(), "  received transaction: "<<tr->toString().c_str());      

    // this is direct forwarding to the Master, WITHOUT remarking the Service field!!!
    // needs to change, as getName and decode now returns vector
    GC_DUMP_N(name(), "forward transaction to master at port number(s) [Name] :-"); 
#ifdef GC_VERBOSE
    show_vector_gc_trace(mAddressMap.decode ( tr->get_mID(), tr->get_mService() ), mAddressMap.getName( tr->get_mID(), tr->get_mService() ));
#endif

    int_vec = mAddressMap.decode(tr->get_mID(), tr->get_mService());
    for(unsigned int i=0; i < int_vec.size(); i++ )
    {
       // this is a master - forward the data to the slave
       destinationForward = target_multi_port.connected_in_ports[int_vec[i]]; // forward a transaction to the mID
       (*destinationForward)->notify(t_p, PEQ_IMMEDIATE);
    }

    // TODO: process transaction
    
    //(*(init_multi_port.connected_in_ports[0]))->notify(ControlTransactionContainer(t_p.first,p));

  }


  // ///////////////////////////////////////////////////////////////////////
  
  /// Kernel callback: bind unbound ports and create map.
  /**
   * Before end of elaboration simulation kernel callback
   * - Bind the ports of APIs (of each module which inherits gc_port)
   * - Create the addres map
   */
  virtual void before_end_of_elaboration() {
    GC_PRINT_SPACE;
    GC_DUMP_N(name(), "before_end_of_elaboration called.");
    process_ports();
  }

  /// Bind unbound ports and create map.
  /**
   * - Bind the ports of APIs (of each module which inherits gc_port)
   * - Create the addres map
   *
   * May be called multiple (to bind and add to map immediately).
   *
   * Searches in the simulation context for the ports (and other 
   * classes and interfaces) and adds them to the concerning map
   * if they are not yet included.
   * Then in the port map is searched for unbound ports to bind 
   * them.
   */
  virtual void process_ports()
  {
    // 1. Go through the entire module hierarchie (starting with simulation context)
    //    and find each object which is a control_initiator_port or a control_target_port
    //    (and others)
    // 2. Check if the ports are already bound to the GreenControl Core.
    // 3. If they are not yet bound, bind them!

    // 1. Depth-First Search
    GC_DUMP_N(name(), "Search for classes in sim context tree.");
    depthFirstSearch(NULL);

    // 2.
    GC_DUMP_N(name(), "Iterate through list of objects which are gc_port ("<<(unsigned int)mGcPortList.size()<<" items).");
    for (list<sc_object*>::iterator iter = mGcControlPortList.begin(); iter!=mGcControlPortList.end(); ++iter) {
      // 2. Check whether the ports are already bound to the GreenControl Core.
      sc_object *node = *iter;//childs->at(i);
      // If this object is a port, check whether it is bound
      control_target_port *tport = dynamic_cast<control_target_port*>(node);
      control_initiator_port *iport = dynamic_cast<control_initiator_port*>(node);
      if (tport) {
        if (tport->is_Bound()) {
          GC_DUMP_N(name(), " Target port    "<<tport->name()<<" is already bound.");
        } else {
          // 3. If the port is not yet bound, bind it!
          GC_DUMP_N(name(), " Target port    "<<tport->name()<<" is not yet bound: Bind it!");
          init_multi_port(*tport);
        }    
      }
      else if (iport) {
        if (iport->is_Bound()) {
          GC_DUMP_N(name(), " Initiator port "<<iport->name()<<" is already bound.");
        } else {
          // 3. If the port is not yet bound, bind it!
          GC_DUMP_N(name(), " Initiator port "<<iport->name()<<" is not yet bound: Bind it!");
          iport->operator()(target_multi_port);
        }
      }
      
    }

    GC_PRINT_SPACE;

    // Create the address map

    GC_DUMP_N(name(), "Creating address map...");
    GC_DUMP_N(name(), "target_multi_port.connected_in_ports.size() = "<<(unsigned int)target_multi_port.connected_in_ports.size());
    GC_DUMP_N(name(), "init_multi_port.connected_in_ports.size()   = "<<(unsigned int)init_multi_port.connected_in_ports.size());

    //#ifdef GC_VERBOSE
    // create the master <=> port-num map
    //for (unsigned int i=0; i<target_multi_port.connected_in_ports.size(); i++) {
    //  unsigned int id = (unsigned int)(target_multi_port.connected_in_ports[i]);
    //  // create (port pointer <=> master number) pairs
    //  mMasterMap[id] = i;
    //  GC_DUMP_N(name(), "At port slot <<i<<" there is the master "<<getModuleName(id)<<" with id=0x"<<std::hex<<id<<std::dec);      
    //}
    //#endif

    // create the address map 
    mAddressMap.generateMap(&init_multi_port);
#ifdef GC_VERBOSE
    mAddressMap.dumpMap();
#endif

  }


  /// Kernel callback: start and stop initialize-mode.
  /**
   * start_of_simulation kernel callback
   *
   * Inform all APIs to start their initial configuration.
   * After that inform all APIs to leave the initialize-mod
   *
   * TODO: (Order is NOT ensured!) The mGcApiIfList contains the object in the reverse depth first search tree order
   * This is to achieve that the APIs do their actions in the right order
   */
  virtual void start_of_simulation()
  {
    depthFirstSearch(NULL, true); // check for removed objects
    GC_PRINT_SPACE;
    GC_DUMP_N(name(), "start_of_simulation, inform all APIs to start initial configuration: ");
    for (list<sc_object*>::iterator iter = mGcApiIfList.begin(); iter!=mGcApiIfList.end(); ++iter) {
      sc_object *node = *iter;
      cnf::initialize_if *apiif = dynamic_cast<cnf::initialize_if*>(node);
      if (apiif) {
        GC_DUMP_N(name(), "                   start_initial_configuration() "<<node->name());
        apiif->start_initial_configuration();
      }
      else {
        GC_DUMP_N(name(), "start_of_simulation: Node is of type "<<node->kind()<<"!");
        SC_REPORT_ERROR(name(), "start_of_simulation: Node is not of type gs::cnf::initialize_if!");
      }
    }    
    GC_DUMP_N(name(), "Inform all APIs to leave initialize-mode: ");
    for (list<sc_object*>::iterator iter = mGcApiIfList.begin(); iter!=mGcApiIfList.end(); ++iter) {
      sc_object* node = *iter;
      cnf::initialize_if *apiif = dynamic_cast<cnf::initialize_if*>(node);
      if (apiif) {
        GC_DUMP_N(name(), "                   end_initialize_mode() "<<node->name());
        apiif->end_initialize_mode();
      }
      else {
        SC_REPORT_ERROR(name(), "start_of_simulation: Node is not of type gs::cnf::initialize_if!");
      }
    }    
    GC_DUMP_N(name(), "        ------------------------------------------------------------------------");
    GC_DUMP_N(name(), "        ------------------------------------------------------------------------");
    GC_DUMP_N(name(), "        -----------------------   start_of_simulation   ------------------------");
    GC_DUMP_N(name(), "        ------------------------------------------------------------------------");
    GC_DUMP_N(name(), "        ------------------------------------------------------------------------");
  }
 

  //#ifdef GC_VERBOSE
  // /// Helper function to get connected module name of a port.
  // /**
  // * Debug helper function.
  // * Returns the name of the module which has the port with the MasterID id.
  // */
  //inline const char *getModuleName(unsigned int id) {
  //  SourcePort port = target_multi_port.connected_in_ports[mMasterMap[id]];
  //  return (dynamic_cast<sc_object*>(port->get_parent()->get_parent()))->name();
  //}
  //#endif


protected:

//#define DFS_DEBUG_ENABLE
#ifdef DFS_DEBUG_ENABLE
#  define DFS(par) par
#else
#  define DFS(par)
#endif
  /// Depth-First Search for sim context.
  /**
   *  Depth-First Search (Tiefensuche)
   *  to go through the module-tree.
   *
   *  Saves all found objects which are gc_port in the 
   *   param variable mGcPortList
   *  and all found objects which inherit gc_configurable in the
   *   member variable configurableList.
   *
   *  Do not add to lists, if already in it. So this function may
   *  be called multiple without generation wrong lists. 
   *
   *  @param node: NULL = start with simulation context (root)
   *               Pointer to sc_object = start with this object
   *  @param report_removed_objects : If to report if the current list contains pointers
   *                                  that are NOT found in this search! If this is the case
   *                                  it may lead to a seg fault due to wrong usage.
   */
  void depthFirstSearch(sc_object *node, bool report_removed_objects = false) {
    DFS(if (node != NULL) cout << "depthFirstSearch(obj "<< node->name() << " " << node << ")"<<flush;
    //DFS(if (node != NULL) cout << "depthFirstSearch(obj "<< node << ")"<<endl<<flush;
        else              cout << "depthFirstSearch(obj "<< node << ")" << flush;);
    if (report_removed_objects) {
      unsigned int mGcPortList_size = mGcPortList.size(); mGcPortList.clear();
      unsigned int mGcApiIfList_size = mGcApiIfList.size(); mGcApiIfList.clear();
      unsigned int mGcControlPortList_size = mGcControlPortList.size(); mGcControlPortList.clear();
      depthFirstSearch_recursive(node, "");
      if (mGcPortList_size != mGcPortList.size()) {
        SC_REPORT_FATAL(name(), "A gc_port has been removed since last search. This may lead to a seg fault when accessing it! Please never create temporary APIs or Plugins!");
      }
      if (mGcApiIfList_size != mGcApiIfList.size()) {
        SC_REPORT_FATAL(name(), "A initialize_if has been removed since last search. This may lead to a seg fault when accessing it! Please never create temporary APIs or Plugins!");
      }
      if (mGcControlPortList_size != mGcControlPortList.size()) {
        SC_REPORT_FATAL(name(), "A control port has been removed since last search. This may lead to a seg fault when accessing it! Please never create temporary APIs or Plugins!");
      }
      assert(mGcPortList_size == mGcPortList.size() 
             && mGcApiIfList_size == mGcApiIfList.size() 
             && mGcControlPortList_size == mGcControlPortList.size()
             && "A gc_port or initialize_if or control port has been removed since last search. This may lead to a seg fault when accessing it! Please never create temporary APIs or Plugins!");
    }
    else
      depthFirstSearch_recursive(node, "");
  }
  void depthFirstSearch_recursive(sc_object *node, std::string prefix)
  {
    // read out all childs and scan them
    {
      const std::vector<sc_object *> *childs_stack = NULL;
      if (node) {
        // get child vector
        DFS(cout << prefix  << "+-- "<<node->name() <<flush;);
//#if SYSTEMC_API == 210
        // SystemC2.1 cannot get childs of an object, so only modules!
        // And SystemC2.2 produces memory leak if node is sc_object
        sc_core::sc_module* node_as_module = dynamic_cast< sc_core::sc_module* >(node);
        if (node_as_module)
          childs_stack = &(node_as_module->get_child_objects());
//#else
//        // memory leak if node is an sc_object: the returned vector is newed!
//        childs_stack = &node->get_child_objects();
//#endif
      } else { // If NULL, then top level
        // get child vector of sim context
        DFS(cout << prefix  << "+-- "<<"root" <<flush;);
#if SYSTEMC_API == 210
        sc_simcontext *sim; // deprecated with SystemC-2.2
        sim = sc_get_curr_simcontext(); // deprecated with SystemC-2.2
        childs_stack = &(sim->get_child_objects());
#else
        childs_stack = &sc_get_top_level_objects();
#endif
      }
      // If node implements the interface impl., store it
      // TODO: Check if this should be shown _after_ the node has been printed out (this way it stands behind the wrong object!!!!!!)
      DFS(cout << "  (";);
      DFS(sc_core::sc_module *current_module = dynamic_cast<sc_module*>(node);
          if (current_module) cout << "sc_module  ";);
      // identify gc_port
      gc_port *gcport = dynamic_cast<gc_port_T*>(node);
      if (gcport) {
        DFS(cout << "gc_port  ";);
        if (!is_in_list(&mGcPortList, node))
            mGcPortList.push_back(node);
      }
      cnf::initialize_if *gcapiif = dynamic_cast<cnf::initialize_if*>(node);
      if (gcapiif) {
        DFS(cout << "initialize_if  ";);
        if (!is_in_list(&mGcApiIfList, node)) {
          mGcApiIfList.push_front(node); // push_front(!!) for correct order (not ordered if called mor often)
        }
      }
      control_target_port *tport = dynamic_cast<control_target_port*>(node);
      if (tport) {
        DFS(cout << "control_target_port  ";);
        if (!is_in_list(&mGcControlPortList, node))
            mGcControlPortList.push_front(node);
      }
      control_initiator_port *iport = dynamic_cast<control_initiator_port*>(node);
      if (iport) {
        DFS(cout << "control_initiator_port  ";);
        if (!is_in_list(&mGcControlPortList, node))
            mGcControlPortList.push_front(node);
      }
      DFS(cout << ")" << endl<<flush;);
      
      // go through childs
      if (childs_stack) {
        for (unsigned int i = 0; i < childs_stack->size(); i++) {
          sc_object *chnode = childs_stack->at(i);
          //DFS(cout << "    start depth search in child "<< chnode << endl<<endl<<flush;);
          std::string prefix_new = prefix;
          DFS(prefix_new += "|  ";);
          depthFirstSearch_recursive(chnode, prefix_new);
        }
        //DFS(cout << endl<< "  return to depthFirstSearch_recursive(obj "<< node <<",...)" << endl<<flush;);
      } else {
#if SYSTEMC_API == 210
        DFS(cout << "  -  SystemC 2.1v1: node not a module. ";); 
#endif
      }
    }
  }
  
  /// Deep copies the vector source into the vector target
  /**
   * @param target  Pointer to a empty Vector which should be written to
   * @param source  Pointer to the source vector
   **/
  static void vectorDeepCopy( vector<sc_object*> *target, const vector<sc_object*> *source) {
    if (target == NULL || !target->empty()) {
      //target = new vector<sc_object*>();
      SC_REPORT_ERROR("static vectorDeepCopy in GC_Core", "vectorDeepCopy(): Got NULL or not empty target vector!");
    }
    for (unsigned int i = 0; i < source->size(); i++) {
      DFS(cout << "    vectorDeepCopy "<< i <<": " << source->at(i)->name()<< " "<< source->at(i) << endl;);
      target->push_back(source->at(i));
    }
  }

  /// Returns if the object pointer <obj> is in the <list>.
  /**
   * @param list  List to be checked.
   * @param obj   Object which should be searched in the list.
   */
  bool is_in_list(list<sc_object*> *list, sc_object* obj) {
    std::list<sc_object*>::iterator iter;
    iter = list->begin();
    while( iter != list->end() ) {
      if (*iter == obj)
        return true;
      iter++;
    }
    return false;
  }

};
  

} // end namespace ctr
} // end namespace gs

#include "gc_port.h"

#endif
