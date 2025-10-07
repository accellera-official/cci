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

#include "cci_inspection.h"

#include <string>
#include <unordered_map>

class discovery_impl : public cci::experimental::inspection::discovery_if {
  using item_if = cci::experimental::inspection::item_if;

public:
  discovery_impl() : m_items(), m_name("CCI Discovery Interface") {};

  virtual ~discovery_impl() = default;

  virtual std::string_view get_name() const override { return m_name; }

  virtual std::vector<std::shared_ptr<item_if>>
  get_items(std::string_view k) const override {
    auto p = m_items.find(std::string(k));
    if (p == m_items.end()) {
      return std::vector<std::shared_ptr<item_if>>();
    } else {
      return std::vector<std::shared_ptr<item_if>>(p->second);
    }
  }

  virtual bool register_item(std::shared_ptr<item_if> item,
                             std::string_view k) override {
    auto p = m_items.find(std::string{k});
    if (p == m_items.end()) {
      std::vector<std::shared_ptr<item_if>> in;
      in.emplace_back(item);
      m_items.emplace(k, in);
    } else {
      p->second.emplace_back(item);
    }
    return true;
  }

  virtual bool unregister_item(std::shared_ptr<item_if> item,
                               std::string_view k) override {
    auto p = m_items.find(std::string{k});
    if (p == m_items.end())
      return false;
    for (auto it = p->second.begin(); it != p->second.end(); ++it) {
      if (*it == item) {
        p->second.erase(it);
        return true;
      }
    }
    return false;
  }

private:
  std::unordered_map<std::string, std::vector<std::shared_ptr<item_if>>>
      m_items;
  std::string m_name;
};

cci::experimental::inspection::discovery_if *
cci::experimental::inspection::get_discovery_if(uint64_t api_version) {
  if (api_version != CCI_INSPECTION_API_VERSION) {
    return nullptr;
  }
  static discovery_impl disc_impl;
  return &disc_impl;
}
