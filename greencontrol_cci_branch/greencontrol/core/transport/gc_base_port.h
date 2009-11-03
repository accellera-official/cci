// LICENSETEXT
// 
//   Copyright (C) 2007-2008 : GreenSocs Ltd
//       http://www.greensocs.com/ , email: info@greensocs.com
// 
//   Developed by :
// 
//   Wolfgang Klingauf, Robert Guenzel, Christian Schroeder
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
// 
//   Mark Burton, Marcus Bartholomeu
//     GreenSocs Ltd
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



#include <systemc>

#ifndef __gc_base_port_h__
#define __gc_base_port_h__

// TODO: remove this completely
#include "payload_event_queue_only_call_delta_boost.h"


namespace gs {
namespace ctr {

  
//forward declaration
  template <class PV, class PVT>
  class gc_multi_base_port;

  //template <class PV, class PVT>  
  //class gc_base_port_forwarder_base;
  

  //---------------------------------------------------------------------------
  /**
   * Base for all TLM ports. The communication uses events for both
   * sending and receiving.
   */
  //---------------------------------------------------------------------------
  template <class PV, class PVT>
  class gc_base_port // (GreenBus equivalent: tlm_port)
  : public sc_core::sc_module
  {

  public:

    typedef PV b_if_type;
    typedef PVT if_type;
    typedef payload_event_queue_if<if_type> peq_if_type;
    
    sc_port<b_if_type> b_out;
    sc_export<b_if_type> b_in;

    sc_port<peq_if_type> out;
    sc_export<peq_if_type> in;

    payload_event_queue<if_type> peq;

    SC_HAS_PROCESS(gc_base_port);

    gc_base_port( sc_core::sc_module_name port_name ) :
      sc_core::sc_module(port_name),
      b_out(sc_gen_unique_name("b_out")),
      b_in(sc_gen_unique_name("b_in")),
      out(sc_gen_unique_name("out")),
      in(sc_gen_unique_name("in"))
    {
      in(peq);
    }

    virtual ~gc_base_port() {
    }

    void operator() (gc_base_port<b_if_type,if_type>& other) {
      b_out(other.b_in);
      other.b_out(b_in);
      out(other.in);
      other.out(in);
    }

    void bind_b_if(b_if_type& other) {
      b_in(other);
    }

    void operator() (gc_multi_base_port<b_if_type,if_type>& other) {
      b_out(other.b_in);
      other.b_bind_out_port(b_in);
      out(other.in);
      other.bind_out_port(in);
    }

    /*void operator() (gc_base_port_forwarder_base<b_if_type,if_type>& port_fw_) {
      port_fw_.forward(*this);
    }*/
    
  };



  //---------------------------------------------------------------------------
  /**
   * A TLM multi port.
   */
  //---------------------------------------------------------------------------
  template <class PV, class PVT>
  class gc_multi_base_port // (GreenBus equivalent: tlm_multi_port)
  : public sc_core::sc_module
  {

  public:

    typedef PVT if_type;
    typedef PV b_if_type;
    typedef payload_event_queue_if<if_type> peq_if_type;

    sc_port<peq_if_type, 0> out;
    sc_export<peq_if_type> in;

    sc_port<b_if_type, 0> b_out;
    sc_export<b_if_type> b_in;

    std::vector< sc_export<b_if_type>* > connected_b_in_ports;

    payload_event_queue<if_type> peq;
    std::vector< sc_export<peq_if_type>* > connected_in_ports;
    
    gc_multi_base_port( sc_core::sc_module_name port_name ) :
      sc_core::sc_module(port_name),
      out(sc_gen_unique_name("out")),
      in(sc_gen_unique_name("in")),
      b_out(sc_gen_unique_name("b_out")),
      b_in(sc_gen_unique_name("b_in"))
    {
      in(peq);
    }

    virtual ~gc_multi_base_port() {
    }

    void operator() (gc_base_port<b_if_type,if_type>& other) {
      b_bind_out_port(other.b_in);
      other.b_out(b_in);
      bind_out_port(other.in);
      other.out(in);
    }

    void bind_b_if(b_if_type& other) {
      b_in(other);
    }

    void operator() (gc_multi_base_port<b_if_type,if_type>& other) {
      //SC_REPORT_FATAL(sc_core::SC_ID_NOT_IMPLEMENTED_, "multi-to-multi-port binding not supported (yet).");
      b_bind_out_port(other.b_in);
      other.b_bind_out_port(b_in);
      bind_out_port(other.in);
      other.bind_out_port(in);
    }

    /*void operator() (gc_base_port_forwarder_base<b_if_type,if_type>& port_fw_) {
      port_fw_.forward(*this);
    }*/

    void bind_out_port(sc_export<peq_if_type>& in){
      for (unsigned int i=0; i<connected_in_ports.size(); i++){
        if(connected_in_ports[i]==(&in)){
          std::cout<<"Double binding of PVT ports between "<<this->name()<<" and "<<dynamic_cast<sc_object*>(&in)->get_parent()->name()<<std::endl<<std::flush;
          return;
        }
      }
      connected_in_ports.push_back(&in);
      out(in);
    }
    
    void b_bind_out_port(sc_export<b_if_type>& b_in){
      for (unsigned int i=0; i<connected_b_in_ports.size(); i++){
        if(connected_b_in_ports[i]==(&b_in)){
          std::cout<<"Double binding of PV ports between "<<this->name()<<" and "<<dynamic_cast<sc_object*>(&b_in)->get_parent()->name()<<std::endl<<std::flush;
          return;
        }
      }
      connected_b_in_ports.push_back(&b_in);
      b_out(b_in);
    }

  };


/*  template <class PV, class PVT>
  class gc_base_port_forwarder_base : public sc_core::sc_module {  // (GreenBus equivalent: paramless_tlm_port_forwarder_base)
    public:
    
      gc_base_port_forwarder_base(sc_core::sc_module_name name): sc_core::sc_module(name), singleNmulti(false),bound(false),forwarding(false){}
  
      //bind forward port to tlm_port
      void bind(gc_base_port<PV, PVT>& port_){
        if(bound) SC_REPORT_ERROR(name(), "Port is already bound.");
        bound=true;
        if (singleNmulti) (*m_port)(port_);
        else (*m_multi_port)(port_);
      }

      //bind forward port to tlm_multi_port
      void bind( gc_multi_base_port<PV, PVT>& port_ )
      { 
        if(bound) SC_REPORT_ERROR(name(), "Port is already bound.");
        bound=true;
        if (singleNmulti) (*m_port)(port_); 
        else (*m_multi_port)(port_);
      }

      //this method binds a forward port to another forward port
      //unfortunately there can be no short notation for this as the () operator is already used to forward a forward port... Dang!
      void bind(gc_base_port_forwarder_base<PV, PVT>& port_)
      {
        if(bound) SC_REPORT_ERROR(name(), "Port is already bound.");
        bound=true;
        port_.setBound(); 
        if(singleNmulti){
          if(port_.getSNM())
            (*m_port)(*(port_.getTlmPort()));
          else
            (*m_port)(*(port_.getTlmMultiPort()));
        }
        else{
          if(port_.getSNM())
            (*m_multi_port)(*(port_.getTlmPort()));
          else
            (*m_multi_port)(*(port_.getTlmMultiPort()));
        }
      }
      
      //forward a forward port
      void forward( gc_base_port_forwarder_base<PV, PVT>& port_ )
      { 
        //from the point of view of the fw port, it's bound when it is being forwarded to another fw port
        //that's why bound is checked within a forward method
        if(bound) SC_REPORT_ERROR(name(), "Port is already bound a port.");
        bound=true;
        if (singleNmulti) port_.forward(*m_port); 
        else port_.forward(*m_multi_port);
      }
      
      //this method gets called when a forward port is bound to a tlm_port
      //this could either mean to forward this port or to bind it
      //the default implemtation assumes forwarding
      virtual void forward(gc_base_port<PV, PVT>& port_)
      {
        if(forwarding) SC_REPORT_ERROR(name(), "Port is already forwarding a port.");
        forwarding=true;
        m_port=&port_;
        singleNmulti=true;
      }

      //this method gets called when a forward port is bound to a tlm_multi_port
      //this could either mean to forward this port or to bind it
      //the default implemtation assumes forwarding
      virtual void forward(gc_multi_base_port<PV, PVT>& port_)
      {
        if(forwarding) SC_REPORT_ERROR(name(), "Port is already forwarding a port.");
        forwarding=true;       
        m_multi_port=&port_;
        singleNmulti=false;
      }
      
      virtual void start_of_simulation(){
        if (!bound)
          SC_REPORT_ERROR(name(), "Port is not bound.");
        if (!forwarding)
          SC_REPORT_ERROR(name(), "Port is not forwarding a port.");
      }
      
      //some getters
      bool getSNM(){return singleNmulti;}
      gc_base_port<PV, PVT>* getTlmPort(){return m_port;}
      gc_multi_base_port<PV, PVT>* getTlmMultiPort(){return m_multi_port;}
      void setBound(){bound=true;}
        
    private:
      gc_base_port<PV, PVT>* m_port;
      gc_multi_base_port<PV, PVT>* m_multi_port;
      bool singleNmulti;
      bool bound;
      bool forwarding;
  };
  */
  
  
} // end of namespace ctr
} // end of namespace gs

#endif
