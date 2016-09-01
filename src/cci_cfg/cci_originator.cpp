/*****************************************************************************
  Copyright 2006-2015 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2009-2011 GreenSocs Ltd
  All rights reserved.

  Copyright 2006-2015 Intel Corporation
  All rights reserved.

  Copyright 2016 Ericsson
  All rights reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 *****************************************************************************/

#include "cci_cfg/cci_originator.h"
#include "cci_report_handler.h"

CCI_OPEN_NAMESPACE_

cci_originator::cci_originator()
        : m_originator_obj(current_originator_object()),
          m_originator_str(NULL) {
    if (!m_originator_obj) {
        // Caller needs to catch this exception & add identifying parameter info.
        SC_REPORT_ERROR(__CCI_CNF_SC_REPORT_MSG_TYPE_PREFIX__,
                        "Unable to determine parameter's owner.");
    }
}

cci_originator::cci_originator(const cci_originator& originator)
        : m_originator_obj(originator.m_originator_obj) {
    m_originator_str = originator.m_originator_str ?
                       new std::string(*(originator.m_originator_str)) : NULL;
}

cci_originator::cci_originator(const std::string &originator_name,
                               bool systemc_hierarchy) {
    if (!systemc_hierarchy) {
        m_originator_obj = NULL;
        m_originator_str = new std::string(originator_name);
    } else {
        m_originator_obj = current_originator_object();
        if (!m_originator_obj) {
            m_originator_str = new std::string(originator_name);
        }
    }
}

cci_originator::cci_originator(const char *originator_name)
        : m_originator_obj(NULL),
          m_originator_str(new std::string(originator_name)) {
}

const sc_core::sc_object *cci_originator::get_object() const {
    return m_originator_obj;
}

const char *cci_originator::name() const {
    if (m_originator_obj) return m_originator_obj->name();
    return m_originator_str->c_str();
}

cci_originator &cci_originator::operator=(cci_originator originator) {
    std::swap(m_originator_obj, originator.m_originator_obj);
    std::swap(m_originator_str, originator.m_originator_str);
    return *this;
}

sc_core::sc_object *cci_originator::current_originator_object() {
    sc_core::sc_object *originator_obj = sc_core::sc_get_current_object();
    for (sc_core::sc_process_handle current_proc(originator_obj);
         current_proc.valid(); // is this a process?
         current_proc = sc_core::sc_process_handle(originator_obj)) {
        originator_obj = current_proc.get_parent_object();
    }
    return originator_obj;
}

cci_originator::~cci_originator() {
    delete m_originator_str;
}

CCI_CLOSE_NAMESPACE_
