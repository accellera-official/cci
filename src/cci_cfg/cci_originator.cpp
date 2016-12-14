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

#include <cstring>

#include "cci_cfg/cci_originator.h"
#include "cci_report_handler.h"

CCI_OPEN_NAMESPACE_

cci_originator::cci_originator(const std::string& originator_name)
        : m_originator_obj(current_originator_object()),
          m_originator_str(new std::string(originator_name)) {
    if(is_unknown()) {
        m_originator_obj = NULL;
    }
    check_is_valid();
}

cci_originator::cci_originator(const char* originator_name)
        : m_originator_obj(current_originator_object()),
          m_originator_str(new std::string(originator_name)) {
    if(is_unknown()) {
        m_originator_obj = NULL;
    }
    check_is_valid();
}

cci_originator::cci_originator(const cci_originator& originator)
        : m_originator_obj(originator.m_originator_obj) {
    m_originator_str = originator.m_originator_str ?
                       new std::string(*(originator.m_originator_str)) : NULL;
}

const sc_core::sc_object *cci_originator::get_object() const {
    return m_originator_obj;
}

cci_originator cci_originator::get_parent_originator() const {
    if (m_originator_obj != NULL) {
        const sc_core::sc_object *parent_object =
                m_originator_obj->get_parent_object();
        if (parent_object) {
            return cci_originator(*parent_object);
        }
    }
    return cci_originator(__CCI_UNKNOWN_ORIGINATOR_STRING__);
}

const char* cci_originator::name() const {
    static const char* default_name = __CCI_UNKNOWN_ORIGINATOR_STRING__;
    if (m_originator_obj) {
        return m_originator_obj->name();
    } else if(m_originator_str) {
        return m_originator_str->c_str();
    }
    return default_name;
}

const char* cci_originator::string_name() const {
    if(m_originator_str)
        return m_originator_str->c_str();
    else
        return NULL;
}

void cci_originator::swap(cci_originator& that) {
    std::swap(m_originator_obj, that.m_originator_obj);
    std::swap(m_originator_str, that.m_originator_str);
}

cci_originator &cci_originator::operator=(cci_originator copy) {
    copy.swap(*this);
    return *this;
}

bool cci_originator::operator==( const cci_originator& originator ) const {
    if(this->get_object() && originator.get_object()) {
        return this->get_object() == originator.get_object();
    }
    return !std::strcmp(name(), originator.name());
}

bool cci_originator::operator<(const cci_originator& originator) const {
    return std::strcmp(name(), originator.name()) < 0;
}

sc_core::sc_object *cci_originator::current_originator_object() {
    return sc_core::sc_get_current_object();
}

void cci_originator::check_is_valid() const {
    if (!m_originator_obj && !m_originator_str) {
        CCI_REPORT_ERROR("cci_originator/check_is_valid",
                         "It is forbidden to build an originator without "
                         "information (no SystemC hierarchy or empty name)!");
    }
}

bool cci_originator::is_unknown() const {
    if(m_originator_str
       && (!m_originator_str->compare(__CCI_UNKNOWN_ORIGINATOR_STRING__))) {
        return true;
    } else {
        return false;
    }
}

cci_originator::~cci_originator() {
    delete m_originator_str;
}

CCI_CLOSE_NAMESPACE_
