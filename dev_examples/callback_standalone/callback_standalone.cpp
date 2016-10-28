/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 ****************************************************************************/

#include "cci_cfg/cci_param_callbacks.h"
#include "cci_cfg/cci_broker_callbacks.h"
#include "cci_cfg/cci_param_untyped_handle.h"

using namespace sc_core;
using namespace CCI_NAMESPACE;

#ifndef SC_INCLUDE_DYNAMIC_PROCESSES
# define SC_INCLUDE_DYNAMIC_PROCESSES
#endif
#include <systemc> // sc_bind

//
// Some CCI mock definitions
//

cci_originator mock_originator("MockOriginator");

struct mock_param_impl
  : public cci_param_callback_if
{
  // just for demonstration
  friend class mock_param_untyped_handle;
  template<typename U>
  friend class mock_param_handle;

  virtual void trigger(const void * ev) = 0;
  virtual ~mock_param_impl(){}
};

/* ------------------------------------------------------------------------ */

template<typename T>
class mock_param_typed_impl
  : public mock_param_impl
{
  virtual cci_callback_untyped_handle
  register_post_write_callback( const cci_callback_untyped_handle& cb
                              , const cci_originator& orig )
  {
    // restore type information
    typename cci_param_post_write_callback_handle<T>::type typed_cb(cb);
    if ( typed_cb.valid() ) // validate callback type
      m_post_write_callbacks.push_back( typed_cb );
    return typed_cb;
  }

  virtual bool
  unregister_post_write_callback( const cci_callback_untyped_handle& cb
                                , const cci_originator& orig )
  {
    // remove from m_post_write_callbacks again
    return false;
  }

  virtual cci_callback_untyped_handle
  register_pre_write_callback( const cci_callback_untyped_handle& cb
                             , const cci_originator& orig )
  {
    // restore type information
    typename cci_param_pre_write_callback_handle<T>::type typed_cb(cb);
    if ( typed_cb.valid() ) // validate callback type
      m_pre_write_callbacks.push_back( typed_cb );
    return typed_cb;
  }


  virtual bool
  unregister_pre_write_callback( const cci_callback_untyped_handle& cb
                               , const cci_originator& orig )
  {
    // remove from m_pre_write_callbacks again
    return false;
  }

  virtual cci_callback_untyped_handle
  register_pre_read_callback( const cci_callback_untyped_handle& cb
                            , const cci_originator& orig )
  {
    // Unimplemented
    return cci_callback_untyped_handle();
  }

  virtual bool
  unregister_pre_read_callback( const cci_callback_untyped_handle& cb
                              , const cci_originator& orig )
  {
    // Unimplemented
    return false;
  }

  virtual cci_callback_untyped_handle
  register_post_read_callback( const cci_callback_untyped_handle& cb
                             , const cci_originator& orig )
  {
    // Unimplemented
    return cci_callback_untyped_handle();
  }


  virtual bool
  unregister_post_read_callback( const cci_callback_untyped_handle& cb
                               , const cci_originator& orig )
  {
    // Unimplemented
    return false;
  }

  virtual bool unregister_all_callbacks(const cci_originator& orig)
  {
    // remove from m_post_write_callbacks, m_pre_write_callbacks
    // (requires storing the originator somewhere
    return false;
  }

  virtual bool has_callbacks() const
  {
    return !m_post_write_callbacks.empty() || !m_pre_write_callbacks.empty();
  }

  virtual void trigger(const void * ev) {
    for(unsigned i=0; i<m_post_write_callbacks.size(); ++i)
    {
      m_post_write_callbacks[i].invoke(
              *static_cast<const cci_param_write_event<T>*>(ev) );
    }
  }

private:
  std::vector< typename cci_param_post_write_callback_handle<T>::type >
          m_post_write_callbacks;
  std::vector< typename cci_param_pre_write_callback_handle<T>::type >
          m_pre_write_callbacks;
};

/* ------------------------------------------------------------------------ */

class mock_param_untyped_handle
{
public:

  cci_callback_untyped_handle
  register_post_write_callback( const cci_param_post_write_callback_untyped& cb
                              , cci_untyped_tag = cci_untyped_tag() )
    { return m_impl->register_post_write_callback(cb, m_orig); }

  template<typename C>
  cci_callback_untyped_handle
  register_post_write_callback( cci_param_post_write_callback_untyped::signature
                               (C::*cb), C* obj,
                               cci_untyped_tag = cci_untyped_tag() )
    { return register_post_write_callback( sc_bind(cb, obj, sc_unnamed::_1) ); }

  ~mock_param_untyped_handle()
    { delete m_impl; }

protected:
  mock_param_untyped_handle( mock_param_impl* impl )
    : m_impl(impl)
    , m_orig(mock_originator)
  {}

protected:
  mock_param_impl* m_impl;
  cci_originator   m_orig;
};

/* ------------------------------------------------------------------------ */

template<typename T>
struct mock_param_handle : public mock_param_untyped_handle
{
  typedef mock_param_untyped_handle base_type;
public:

  typedef typename cci_param_post_write_callback<T>::type
    cci_param_post_write_callback_typed;

  mock_param_handle()
    : mock_param_untyped_handle( new mock_param_typed_impl<T>() )
  {}

  cci_callback_untyped_handle
  register_post_write_callback( const cci_param_post_write_callback_untyped& cb
                              , cci_untyped_tag )
    { return base_type::register_post_write_callback( cb ); }

  template<typename C>
  cci_callback_untyped_handle
  register_post_write_callback( cci_param_post_write_callback_untyped::signature
                               (C::*cb), C* obj, cci_untyped_tag )
    { return base_type::register_post_write_callback( cb, obj ); }

  virtual cci_callback_untyped_handle
  register_post_write_callback( const cci_param_post_write_callback_typed& cb
                              , cci_typed_tag<T> = cci_typed_tag<T>() )
    { return this->m_impl->register_post_write_callback(cb, this->m_orig); }

  template<typename C>
  cci_callback_untyped_handle
  register_post_write_callback(
          typename cci_param_post_write_callback_typed::signature (C::*cb),
          C* obj, cci_typed_tag<T> = cci_typed_tag<T>() )
    { return register_post_write_callback( sc_bind(cb, obj, sc_unnamed::_1) ); }


  void trigger( const cci_param_write_event<T>& ev )
  {
    (this)->m_impl->trigger(&ev);
  }
};

/* ------------------------------------------------------------------------ */

template<typename T>
void write_callback(const cci_param_write_event<T> & ev)
{
    std::cout << "write_callback: "
              << ev.old_value << " -> " << ev.new_value
              << std::endl;
}

void untyped_write_callback(const cci_param_write_event<> & ev)
{
    std::cout << "untyped_write_callback: "
              << ev.old_value << " -> " << ev.new_value
              << std::endl;
}

class MyClass
{
public:
  template<typename T>
  void typed_write(const cci_param_write_event<sc_time>& ev)
  {
    std::cout << "MyClass::typed_write: "
              << ev.old_value << " -> " << ev.new_value
              << std::endl;
  }

  void untyped_write(const cci_param_write_event<> & ev)
  {
      std::cout << "MyClass::untyped_write: "
                << ev.old_value << " -> " << ev.new_value
                << std::endl;
  }
};

int sc_main(int, char*[])
{
  cci_param_untyped_handle dummy_handle(cci_originator("sc_main"));
  int old_v = 17, new_v = 42;
  cci_param_write_event<int> ev(old_v,new_v,cci_originator("sc_main"),dummy_handle);

  cci_param_post_write_callback<int>::type cb( write_callback<int> );
  std::cout << "Direct invocation: " << std::endl;
  cb( ev );

  {
    std::cout << "Typed handle invocation: " << std::endl;
    cci_param_post_write_callback_handle<int>::type hnd = cb;
    hnd.invoke(ev);
  }
  {
    std::cout << "Explicit untyped handle invocation: " << std::endl;
    cci_callback_untyped_handle hnd = cb;
    hnd.invoke<void,const cci_param_write_event<int>&>(ev);
  }

  {
    mock_param_handle<sc_time> p;
    p.register_post_write_callback( write_callback<sc_time> );
    p.register_post_write_callback( untyped_write_callback, cci_untyped_tag() );

    MyClass c;
    p.register_post_write_callback( &MyClass::typed_write<sc_time>, &c );
    p.register_post_write_callback( &MyClass::untyped_write, &c,
                                   cci_untyped_tag() );

    std::cout << "Trigger registered callbacks: " << std::endl;
    cci_param_write_event<sc_time> tev
    (
        sc_time(17, SC_NS)
      , sc_time(42, SC_US)
      , mock_originator
      , dummy_handle
    );
    p.trigger( tev );
  }

  return 0;
}
