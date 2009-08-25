/*****************************************************************************
 
 The following code is derived, directly or indirectly, from the SystemC
 source code Copyright (c) 1996-2009 by all Contributors.
 All Rights reserved.
 
 Developed by GreenSocs : http://www.greensocs.com/
 Christian Schroeder, schroeder@eis.cs.tu-bs.de
 Mark Burton, mark@greensocs.com
 
 The contents of this file are subject to the restrictions and limitations
 set forth in the SystemC Open Source License Version 3.0 (the "License");
 You may not use this file except in compliance with such restrictions and
 limitations. You may obtain instructions on how to receive a copy of the
 License at http://www.systemc.org/. Software distributed by Contributors
 under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 ANY KIND, either express or implied. See the License for the specific
 language governing rights and limitations under the License.
 
 *****************************************************************************/

#ifndef __GS_CCI_BASE_PARAM_H__
#define __GS_CCI_BASE_PARAM_H__

#include "cci.h"

namespace cci {
  
  
  class gs_cci_base_param
  : public gs_cci_param<std::string>
  {
    
  public:
    
    explicit gs_cci_base_param(const std::string& name,
                               const std::string& value,
                               const bool force_top_level_name = false) 
    : gs_cci_param<std::string>(name, value, force_top_level_name)
    {      
    }
    
    virtual ~gs_cci_base_param() { }
    
  };
  
  
} // end namespace cci

#endif
