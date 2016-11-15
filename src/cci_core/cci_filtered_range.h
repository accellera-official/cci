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
 * @brief CCI filtered range class
 *
 * The filtered range class allows to iterate over a range skipping some
 * elements lazily. A predicate controls elements which are skipped.
 */
template<typename T, typename Predicate, typename Container = std::vector<T> >
class cci_filtered_range
{
public:
    typedef typename Container::iterator container_iterator;

private:
    class cci_iterator
    {
    public:
        friend class cci_filtered_range;

        cci_iterator():
                m_fr(NULL)
        {};

    private:
        cci_iterator(container_iterator it, cci_filtered_range* fr):
                m_current(it),
                m_fr(fr)
        {};

    public:
        cci_iterator& operator--() {
            if(m_fr) {
                decrement();
            }
            return *this;
        }

        cci_iterator& operator++() {
            if(m_fr) {
                increment();
            }
            return *this;
        }

        T operator*() const {
            return *m_current;
        }

        T operator->() const {
            return *m_current;
        }

        operator container_iterator() const {
            return m_current;
        }

        bool operator==(
                const container_iterator& it) const {
            return m_current == it;
        }

        bool operator!=(
                const container_iterator& it) const {
            return !(operator==(it));
        }

    private:
        void increment() {
            ++m_current;
            while((m_current != m_fr->m_end)
                  && (!m_fr->m_pred(*(m_current)))) {
                ++m_current;
            }
        }

        void decrement() {
            --m_current;
            while((m_current != m_fr->m_begin)
                  && (!m_fr->m_pred(*(m_current)))) {
                --m_current;
            }
        }

        container_iterator m_current;
        cci_filtered_range* m_fr;
    };

public:
    typedef cci_iterator iterator;
    typedef const cci_iterator const_iterator;

    cci_filtered_range(Predicate pred,
                       Container container):
            m_pred(pred),
            m_container(container),
            m_begin(m_container.begin()),
            m_end(m_container.end())
    {
        init();
    }

    Predicate predicate() const {
        return m_pred;
    }

    const Container& container() {
        return m_container;
    }

    iterator begin() {
        return iterator(m_begin, this);
    }

    iterator end() {
        return iterator(m_end, this);
    }

public:
    void init() {
        cci_iterator it(m_begin, this);
        if ((it != m_end) && (!m_pred(*it))) {
            ++it;
        }
        m_begin = it;
    }

    Predicate m_pred;
    Container m_container;
    container_iterator m_begin;
    container_iterator m_end;
};

CCI_CLOSE_NAMESPACE_

#endif // CCI_CORE_CCI_FILTERED_RANGE_H_INCLUDED_
