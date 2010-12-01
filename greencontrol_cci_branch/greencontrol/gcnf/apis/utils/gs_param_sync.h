//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Marcus Bartholomeu
//     GreenSocs Ltd
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doxygen comments

#ifndef __GS_PARAM_SYNC_H__
#define __GS_PARAM_SYNC_H__

namespace gs {
namespace cnf {

  /// Class that synchronizes two gs_params - can be used for aliasing parameters
  template <class T>
  class gs_param_sync
  {

    gs_param<T>* m_param1;
    gs_param<T>* m_param2;

  public:
    
    GC_HAS_CALLBACKS();


    /// Constructor
    gs_param_sync()
    {
    }


    void bind(const std::string& param1_name, const std::string& param2_name)
    {
      gs::cnf::cnf_api_if* m_cnfApi = gs::cnf::GCnf_Api::getApiInstance(NULL);

      gs_param_base* param1_base = m_cnfApi->getPar(param1_name);
      gs_param_base* param2_base = m_cnfApi->getPar(param2_name);

      bind(param1_base, param2_base);
    }

    void bind(gs_param_base* param1_base, gs_param_base* param2_base)
    {
      // Get param1
      if (!param1_base)
        SC_REPORT_FATAL("gs_param_sync", ("The syncronized parameter should already exist: " + param1_base->getName()).c_str());
      m_param1 = dynamic_cast<gs_param<T>*>(param1_base);
      if (!m_param1)
        SC_REPORT_FATAL("gs_param_sync", ("The syncronized parameter is not the same type as the gs_param_sync: " + param1_base->getName()).c_str());

      // Get param2
      if (!param2_base)
        SC_REPORT_FATAL("gs_param_sync", ("The syncronized parameter should already exist: " + param2_base->getName()).c_str());
      m_param2 = dynamic_cast<gs_param<T>*>(param2_base);
      if (!m_param2)
        SC_REPORT_FATAL("gs_param_sync", ("The syncronized parameter is not the same type as the gs_param_sync: " + param2_base->getName()).c_str());

      // TODO: it would be possible to do sync not-yet-existing params by using the new param callback.
      //       Then there are considerations like what if the new param was initialized with a non-default value

      // Do the first sync
      if (*m_param1 != *m_param2) {
        // if m_param1 has the default value
        if (*m_param1 == T()) {
          // copy from m_param2
          *m_param1 = *m_param2;
        }
        // if m_param2 has the default value
        else if (*m_param2 == T()) {
          // copy from m_param1
          *m_param2 = *m_param1;
        }
        // both are already set, error!
        else {
          SC_REPORT_ERROR("gs_param_sync", ("The syncronized parameters are already set to different non-default values: " +
                                            m_param1->getName() + "=" + m_param1->getString() + ", " +
                                            m_param2->getName() + "=" + m_param2->getString()).c_str());
        }
      }

      // set callbacks
      GC_REGISTER_PARAM_CALLBACK(m_param1, gs_param_sync, callback1);
      GC_REGISTER_PARAM_CALLBACK(m_param2, gs_param_sync, callback2);

    }


    ~gs_param_sync() {
      GC_UNREGISTER_CALLBACKS();
    }


    void callback1(gs::gs_param_base& par) {
      if (!par.is_destructing() && m_param2->getString() != par.getString()) {
        m_param2->setString(par.getString());
      }
    }


    void callback2(gs::gs_param_base& par) {
      if (!par.is_destructing() && m_param1->getString() != par.getString()) {
        m_param1->setString(par.getString());
      }
    }


  };


} // namespace cnf
using cnf::gs_param_sync; // make gs_param_sync available in gs namespace
} // namespace gs

#endif // __GS_PARAM_SYNC_H__
