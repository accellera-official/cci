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

#ifndef PARAM_TRACKER_H_
#define PARAM_TRACKER_H_

#include <sstream>
#include "cci_cfg/cci_param_untyped_handle.h"

/**
 * @class param_tracker
   @brief Reports value changes for parameters of interest; used in examples.
 */
class param_tracker
{
public:
    /**
     * @fn       param_tracker
     * @brief    The class constructor
     * @param h  Handle for parameter to be tracked
     */
    explicit param_tracker(const cci::cci_param_handle h)
    : phandle(h) // take a copy of handle to control its lifetime
    {
        // Ensure handle validity
        sc_assert(phandle.is_valid());

        // Report parameter/handle details
        std::cout << prefix()
                  << "Tracking " << phandle.name() << std::endl
                  << "  handle originator: " << phandle.get_originator().name() << std::endl;
        if (!h.get_description().empty())
            std::cout << "  description: " << phandle.get_description() << std::endl;
        
        // Report value details
        report_value_details();
        report_value_orginator();

        // Register post-write callback
        phandle.register_post_write_callback(&param_tracker::post_write_cb, this);
    }
protected:

    /**
     * @fn    report_value_details
     * @brief Print param value & its characteristics
     */
    void report_value_details()
    {
        std::cout << "  value: " << phandle.get_cci_value().to_json();
        if (phandle.is_locked())
            std::cout << "; locked";
        if (phandle.is_default_value())
            std::cout << "; is default";
        if (phandle.is_preset_value())
            std::cout << "; is preset";
        std::cout << std::endl;
    }

    /**
    * @fn    report_value_orginator
    * @brief Print param value originator 
    */
    void report_value_orginator()
    {
        value_origin = phandle.get_value_origin().name();
        std::cout << "  value origin: " << value_origin << std::endl;
    }

    /**
    * @fn    post_write_cb
    * @brief Print new value when callback triggered
    */
    void post_write_cb(const cci::cci_param_write_event<> &)
    {
        std::cout << prefix()
                  << phandle.name() << " value updated:" << std::endl;
        report_value_details();
 std::string new_value_originator = phandle.get_value_origin().name();
        if (value_origin != phandle.get_value_origin().name())
            report_value_orginator();
    }

    static std::string prefix() 
    { 
        static std::ostringstream ss;
        ss.str("");
        ss << "[PTRK @ " << sc_core::sc_time_stamp() << "] ";  
        return ss.str();
    }
    cci::cci_param_handle phandle;
    std::string value_origin;
};

#endif  // PARAM_TRACKER_H_
