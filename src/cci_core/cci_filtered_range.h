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

#ifndef CCI_CORE_CCI_FILTERED_RANGE_H_INCLUDED_
#define CCI_CORE_CCI_FILTERED_RANGE_H_INCLUDED_

/**
 * @author Guillaume Delbergue, Ericsson / GreenSocs
 */

#include <vector>

#include "cci_cfg/cci_config_macros.h"

CCI_OPEN_NAMESPACE_

/**
 * @class cci_filtered_range
 * @brief CCI filter iterator class
 *
 * The filter iterator class allows to iterate over a range skipping some
 * elements lazily. A predicate controls elements which are skipped.
 */
template<typename T, typename P, typename Container = std::vector<T> >
class cci_filtered_range
{
public:
    typedef typename Container::iterator iterator;

    cci_filtered_range(P pred, iterator begin, iterator end):
            m_pred(pred),
            m_begin(begin),
            m_end(end),
            m_current(begin)
    {
        init();
    }

    cci_filtered_range(P pred, iterator begin, iterator end, iterator current):
            m_pred(pred),
            m_begin(begin),
            m_end(end),
            m_current(current)
    {
        init();
    }

    cci_filtered_range(P pred, Container values):
            m_pred(pred),
            m_values(values),
            m_begin(m_values.begin()),
            m_end(m_values.end()),
            m_current(m_values.begin())
    {
        init();
    }

    cci_filtered_range& operator++() {
        increment();
        return *this;
    }

    T operator*() const {
        return *m_current;
    }

    T operator->() const {
        return *m_current;
    }

    operator iterator() const {
        return m_current;
    }

    bool operator==(
            const iterator& it) const {
        return m_current == it;
    }

    bool operator!=(
            const iterator& it) const {
        return !(operator==(it));
    }

    P predicate() const {
        return m_pred;
    }

    cci_filtered_range begin() {
        return cci_filtered_range(m_pred, m_begin, m_end, m_begin);
    }

    iterator end() {
        return m_end;
    }

private:
    void init() {
        if ((m_current != m_end) && (!m_pred(*(m_current)))) {
            increment();
        }
        m_begin = m_current;
    }

    void increment() {
        ++m_current;
        while((m_current != m_end) && (!m_pred(*(m_current)))) {
            ++m_current;
        }
    }

    P m_pred;
    Container m_values;
    iterator m_begin;
    iterator m_end;
    iterator m_current;
};

CCI_CLOSE_NAMESPACE_

#endif // CCI_CORE_CCI_FILTERED_RANGE_H_INCLUDED_
