/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

 Modified from
  payload_event_queue.h -- Event Queue Facility Definitions

  Original Author: Ulli Holtmann, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Marcus Bartholomeu, GreenSocs, 2006-05
  Description of Modification: starting from the original sc_event_queue.cpp,
                               a payload was associated to each event trigger.

			Name, Affiliation, Date: Robert G¸nzel, Wolfgang Klingauf, TU Braunschweig, E.I.S., 2007-08
  Description of Modification: own time ordered list implementation,
                               changed output IF to interface method call

 *****************************************************************************/

// DONOTRELICENSE

#ifndef __gc_PAYLOAD_EVENT_QUEUE_H__
#define __gc_PAYLOAD_EVENT_QUEUE_H__

#define USE_DELTA_LISTS // use tuning for delta-cycle payload events

#include "sysc/communication/sc_interface.h"
#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_event.h"

#include <map>

namespace gs {
namespace ctr {

using std::multimap;
using std::pair;


template <typename PAYLOAD>
class timeorderedlist
{
public:
  struct element
  {
    struct element  *next;
    PAYLOAD p;
    sc_core::sc_time t;
    sc_dt::uint64 d;
    element(PAYLOAD& p, sc_core::sc_time t, sc_dt::uint64 d): p(p),t(t),d(d) {}
    element(){}
  };
  
  element *nill;
  element *empties;
  element *list;
  unsigned int size;
    
  timeorderedlist() 
    : nill(new element()),
      empties(NULL),
      list(nill),
      size(0)
  {
  }
  
  ~timeorderedlist() {
    while(size) {
      delete_top();
    }

    while(empties){
      struct element *e=empties->next;
      delete empties;
      empties=e;
    }
    delete nill;
  }
  
  void insert(PAYLOAD& p, sc_core::sc_time t) {
    if (!empties) {
      empties=new struct element();
      empties->next=NULL;
    }
    
    struct element *e=empties;
    empties=empties->next;
    e->p=p;
    e->t=t;
    e->d=sc_core::sc_delta_count();
    
    struct element * ancestor=nill;
    struct element * iterator=list;
    while (iterator!=nill && iterator->t<=t){
      ancestor=iterator;
      iterator=iterator->next;
    }
    if (ancestor==nill){
      e->next=list;
      list=e;
    }
    else {
      e->next=iterator;
      ancestor->next=e;
    }
    size++;
  }
  
  void delete_top(){
    if (list != nill) {
      struct element *e=list;
      list=list->next;
      e->next=empties;
      empties=e;
      size--;
    }
  }
  
  unsigned int getSize()
  {
    return size;
  }
  
  PAYLOAD &top()
  {
    return list->p;
  }
  sc_core::sc_time top_time()
  {
    return list->t;
  }

  sc_dt::uint64& top_delta()
  {
    return list->d;
  }

  sc_core::sc_time next_time()
  {
    return list->next->t;
  }
};

//---------------------------------------------------------------------------
/**
 * Interface to the event queue with payload
 */
//---------------------------------------------------------------------------

/**
 * Special PEQ delay types
 */
typedef enum {
  PEQ_IMMEDIATE = 0, // do direct method call in the target 
  PEQ_YIELD          // use immediate event notification (notify after callee has yielded)
} enumPeqDelay;


  template<typename PAYLOAD>
  class payload_event_queue_if : public virtual sc_core::sc_interface
  {
  public:
//    virtual void notify (PAYLOAD& p) =0;
    virtual void notify (PAYLOAD& p, double when, sc_core::sc_time_unit base) =0;
    virtual void notify (PAYLOAD& p, const sc_core::sc_time& when) =0;
    virtual void notify (PAYLOAD& p, const enumPeqDelay delay=PEQ_YIELD) =0;
    virtual void cancel_all() =0;
  };

  template<typename PAYLOAD>
  class payload_event_queue_output_if : public virtual sc_core::sc_interface
  {
  public:
    virtual void notify (PAYLOAD& p) =0;
  };




//---------------------------------------------------------------------------
/**
 * An event queue that can contain any number of pending
 * notifications. Each notification have an associate payload.
 */
//---------------------------------------------------------------------------
template<typename PAYLOAD>
class payload_event_queue: 
  public payload_event_queue_if<PAYLOAD>,
  public sc_core::sc_module
{

  class delta_list{
  public:
    delta_list(){
      reset();
      entries.resize(100);
    }
    
    inline void insert(PAYLOAD& p){
      if (size==entries.size()){
        entries.resize(entries.size()*2);
      }
      entries[size++]=p;
    }
    
    inline PAYLOAD& get(){
      return entries[out++];
    }
    
    inline bool next(){
      return out<size;
    }
    
    inline void reset(){
      size=0;
      out=0;
    }
  public:
    unsigned int size;
  private:
    std::vector<PAYLOAD> entries;
    unsigned int out;
  };

public:

  //this is a multi_port because there could be multiple targets (e.g. 'real' target and monitor)
  sc_core::sc_port<payload_event_queue_output_if<PAYLOAD>,0> out_port;
  
  SC_HAS_PROCESS( payload_event_queue );
  
  payload_event_queue();
  payload_event_queue( sc_core::sc_module_name name_ );
  virtual ~payload_event_queue();
  
  //virtual void notify (PAYLOAD&);
  virtual void notify (PAYLOAD&, const sc_core::sc_time& when);
  virtual void notify (PAYLOAD&, double when, sc_core::sc_time_unit base);
  virtual void notify (PAYLOAD& p, const enumPeqDelay delay=PEQ_YIELD);
  virtual void cancel_all();
    
  virtual void end_of_elaboration();
  
private:
  
  void fec();  //used for direct call based interface
  timeorderedlist<PAYLOAD> m_ppq;
#ifdef USE_DELTA_LISTS
  delta_list m_unevenDelta;
  delta_list m_evenDelta;
  delta_list m_immediateYield;
#endif
  
  sc_core::sc_event m_e;   // default event
  sc_dt::uint64 m_delta;
  sc_core::sc_time m_time;
};


//---------------------------------------------------------------------------
//
// Implementation (in the header to avoid use of export templates)
//
//---------------------------------------------------------------------------

template<typename PAYLOAD>
void payload_event_queue<PAYLOAD>::notify (PAYLOAD& p, const sc_core::sc_time& when)
{
#ifdef USE_DELTA_LISTS
  if (when==sc_core::SC_ZERO_TIME) {
    if (sc_core::sc_delta_count() & (sc_dt::uint64)0x1) //uneven delta cycle so delta delay is for even cylce
      m_evenDelta.insert(p);
    else
      m_unevenDelta.insert(p); //even delta cycle so delta delay is for uneven delta
    m_e.notify(sc_core::SC_ZERO_TIME);
  }
  else {
#endif
    m_ppq.insert(p,  when + sc_core::sc_time_stamp() );
    m_e.notify(when); // note, this will only over-right the "newest" event.
#ifdef USE_DELTA_LISTS
  }
#endif
}
/*
template<typename PAYLOAD>
void payload_event_queue<PAYLOAD>::notify (PAYLOAD& p)
{
  PAYLOAD q=p; //copy!!
  out_port->notify(q);
}
*/

template<typename PAYLOAD>
void payload_event_queue<PAYLOAD>::notify (PAYLOAD& p, double when, sc_core::sc_time_unit base )
{
  notify( p, sc_core::sc_time(when,base) );
}

// special notify for immediate "yielded" events
template<typename PAYLOAD>
void payload_event_queue<PAYLOAD>::notify (PAYLOAD& p, const enumPeqDelay delay)
{
  switch (delay) {
    case PEQ_IMMEDIATE:{
      PAYLOAD q=p; //copy!!
      out_port->notify(p);
      break;
    }
    case PEQ_YIELD:{
#ifdef USE_DELTA_LISTS
      m_immediateYield.insert(p);
#else
		  m_ppq.insert(p, sc_core::sc_time_stamp() ); // Wolle: doesnt work properly -- inserts a delta cycle due to fec() implementation!
#endif
		  m_e.notify(); // immediate notification
      break;
    }
  }
}

template<typename PAYLOAD>
payload_event_queue<PAYLOAD>::payload_event_queue()
  : sc_core::sc_module( sc_core::sc_gen_unique_name( "payload_event_queue" ) ), m_delta(0),m_time(0-10,sc_core::SC_NS)
{
  end_module();
}

template<typename PAYLOAD>
payload_event_queue<PAYLOAD>::payload_event_queue( sc_core::sc_module_name name_ )
  : sc_core::sc_module( name_ ),m_delta(0),m_time(0-10,sc_core::SC_NS)
{
  end_module();
}

template<typename PAYLOAD>
payload_event_queue<PAYLOAD>::~payload_event_queue()
{
}

template<typename PAYLOAD>
void payload_event_queue<PAYLOAD>::end_of_elaboration(){
// if true, sysc 2.2 from OSCI or anything else
// if false OSCI 2.1v1 or less
#if !(defined SYSTEMC_VERSION & SYSTEMC_VERSION <= 20050714)
  // If a warning occurs here, this is a SystemC 2.1 issue!
  SC_METHOD( fec );
#else
  // workaround for OSCI SystemC 2.1
  declare_method_process( fec_handle, sc_gen_unique_name("gc_fec_renamed"), SC_CURRENT_USER_MODULE, fec );
#endif
  sensitive << m_e;
  dont_initialize();
}

template<typename PAYLOAD>
void payload_event_queue<PAYLOAD>::cancel_all()
{
  while( m_ppq.getSize() > 0 )
    m_ppq.delete_top();
  m_e.cancel();
}

//TODO: call into all connected 'targets' (there could be more than one when using an external monitor)
template<typename PAYLOAD>
void payload_event_queue<PAYLOAD>::fec(){

#ifdef USE_DELTA_LISTS
  //immediate yield notifications
  while(m_immediateYield.next()) out_port->notify(m_immediateYield.get());
  m_immediateYield.reset();
  
  //delta notifications
  if (sc_core::sc_delta_count() & (sc_dt::uint64) 0x1) {//uneven delta so put out all payloads for uneven delta
    while (m_unevenDelta.next()) out_port->notify(m_unevenDelta.get());
    m_unevenDelta.reset();
    if (m_evenDelta.size) m_e.notify(sc_core::SC_ZERO_TIME);
  }
  else {
    while (m_evenDelta.next()) out_port->notify(m_evenDelta.get());
    m_evenDelta.reset();  
    if (m_unevenDelta.size) m_e.notify(sc_core::SC_ZERO_TIME);
  }
  if (!m_ppq.getSize()) return; //there were only delta notification
#endif
  
  //timed notifications
  //TODO:: if delta boost proves to be good and free of bugs remove delta checks below, because they are not needed any more
  assert(m_ppq.getSize()); //there has to be something inside
  const sc_core::sc_time now=sc_core::sc_time_stamp();
  sc_core::sc_time top=m_ppq.top_time();
  const sc_dt::uint64& delta=sc_core::sc_delta_count();
  sc_dt::uint64& top_delta=m_ppq.top_delta();
#ifndef USE_DELTA_LISTS
  assert(top==now); //we must have waken up at the top's time
#endif
  
  while(m_ppq.getSize() && top_delta!=delta && top==now) { // push all active ones into target
    out_port->notify(m_ppq.top());
    m_ppq.delete_top();
    top_delta=m_ppq.top_delta();
    top=m_ppq.top_time();
  }
  if ( m_ppq.getSize()) {
    m_e.notify( top - now) ;
  }
  
}

  
} // end namespace ctr
} // end namespace gs

#endif // PAYLOAD_EVENT_QUEUE_H
