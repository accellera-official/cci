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

#ifndef CCI_INSPECTION_H
#define CCI_INSPECTION_H

#include <memory>
#include <vector>
#include <cstdint>
#include <string_view>

/// API version identifier for CCI Inspection.
#define CCI_INSPECTION_API_VERSION 20241210ull

namespace cci {
namespace experimental {
namespace inspection {

/**
 * @brief Enum defining capability flags for inspection items.
 */
enum capabilities {
    CAP_NONE = 0x0,
    CAP_READ = 0x1,
    CAP_WRITE = 0x2,
    CAP_READWRITE = CAP_READ | CAP_WRITE
};

/**
 * @brief Interface for an inspectable item.
 */
class item_if {
public:
    virtual ~item_if() = default;
    item_if() = default;

    /**
     * @brief Get the name of the item.
     * @return A string view representing the item's name.
     */
    virtual std::string_view get_name() const = 0;

    /**
     * @brief Get the capabilities of the item.
     * @return The capabilities associated with the item.
     */
    virtual capabilities get_capabilities() const { return CAP_NONE; }

    /**
     * @brief Get the size of the item.
     * @return The size of the item in bytes.
     */
    virtual size_t get_size() const = 0;

    /**
     * @brief Read data from the item.
     * @param data A vector to store the read data, needs to be size of data to be read in bytes.
     * @param offset The offset in bytes from which to start reading. If offset is larger than the size of the item,
     *               no data is returned.
     * @return The number of bytes read.
     */
    virtual size_t peek(std::vector<uint8_t>& data, size_t offset) const = 0;

    /**
     * @brief Write data to the item.
     * @param data A vector containing the data to write. If the vector is larger than what can be written to the item,
                   only the valid subset will be written.
     * @param offset The offset at which to start writing. If the offset is larger than the size of the item, no data is
                     written. If the data is larger than what can be written at the offset, only the valid subset will
                     be written.
     * @return The number of bytes written.
     */
    virtual size_t poke(const std::vector<uint8_t>& data, size_t offset) = 0;
};

/**
 * @brief Interface for discovering and managing inspectable items.
 */
class discovery_if
{
public:
    virtual ~discovery_if() = default;
    discovery_if() = default;

    /**
     * @brief Get the name of the discovery interface.
     * @return A string view representing the name.
     */
    virtual std::string_view get_name() const = 0;

    /**
     * @brief Get a description of the discovery interface.
     * @return A string view representing the description.
     */
    virtual std::string_view get_description() const { return ""; };

    /**
     * @brief Retrieve a list of items associated with a given key.
     * @param key A string view representing the key.
     * @return A vector of shared pointers to `item_if` objects.
     */
    virtual std::vector<std::shared_ptr<item_if>> get_items(std::string_view key) const = 0;

    /**
     * @brief Register an item with a key.
     * @param m Shared pointer to the item interface.
     * @param key A string view representing the key.
     * @return True if registration was successful, false otherwise.
     */
    virtual bool register_item(std::shared_ptr<item_if> m, std::string_view key) = 0;

    /**
     * @brief Unregister an item using a key.
     * @param m Shared pointer to the item interface.
     * @param key A string view representing the key.
     * @return True if unregistration was successful, false otherwise.
     */
    virtual bool unregister_item(std::shared_ptr<item_if> m, std::string_view key) = 0;
};

/**
 * @brief Retrieve the discovery interface for the given API version.
 * @param api_version The API version to use.
 * @return A pointer to the discovery interface if available, otherwise nullptr.
 */
discovery_if* get_discovery_if(uint64_t api_version);

} // namespace inspection
} // namespace experimental
} // namespace cci


#endif // CCI_INSPECTION_H
