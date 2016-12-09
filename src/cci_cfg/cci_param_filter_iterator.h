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

#ifndef CCI_CFG_CCI_PARAM_FILTER_ITERATOR_H_INCLUDED_
#define CCI_CFG_CCI_PARAM_FILTER_ITERATOR_H_INCLUDED_

/**
 * @author Guillaume Delbergue, Ericsson / GreenSocs
 */

CCI_OPEN_NAMESPACE_

#include "cci_cfg/cci_param_untyped_handle.h"
#include "cci_cfg/cci_broker_callbacks.h"

/**
 * @class cci_param_filter_iterator
 * @brief CCI parameter filter iterator class
 *
 * The filter iterator class allows to iterate over a range skipping some
 * elements lazily. A predicate controls elements which are skipped.
 */
class cci_param_filter_iterator
{
public:
    typedef std::vector<cci_param_untyped_handle>::iterator iterator;

    cci_param_filter_iterator(cci_param_predicate pred,
                              iterator begin,
                              iterator end):
            m_pred(pred),
            m_begin(begin),
            m_end(end),
            m_current(begin)
    {
        init();
    }

    cci_param_filter_iterator(cci_param_predicate pred,
                              iterator begin,
                              iterator end,
                              iterator current):
            m_pred(pred),
            m_begin(begin),
            m_end(end),
            m_current(current)
    {
        init();
    }

    cci_param_filter_iterator(cci_param_predicate pred,
                              std::vector<cci_param_untyped_handle> handles):
            m_pred(pred),
            m_handles(handles),
            m_begin(m_handles.begin()),
            m_end(m_handles.end()),
            m_current(m_handles.begin())
    {
        init();
    }

    cci_param_filter_iterator& operator++() {
        increment();
        return *this;
    }

    cci_param_untyped_handle operator*() const {
        return *m_current;
    }

    cci_param_untyped_handle operator->() const {
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

    cci_param_predicate predicate() const {
        return m_pred;
    }

    cci_param_filter_iterator begin() {
        return cci_param_filter_iterator(m_pred, m_begin, m_end, m_begin);
    }

    iterator end() {
        return m_end;
    }

    void reset() {
        m_current = m_begin;
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

    cci_param_predicate m_pred;
    std::vector<cci_param_untyped_handle> m_handles;
    iterator m_begin;
    iterator m_end;
    iterator m_current;
};

CCI_CLOSE_NAMESPACE_

#endif // CCI_CFG_CCI_PARAM_FILTER_ITERATOR_H_INCLUDED_
