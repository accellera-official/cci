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

/**
 *  @file   ex00_Subsystem.h
 *  @brief  This file constructs the "param_owner", "param_user" and "param_track" modules.
 *  @author Sonal Poddar, Intel
 */

#ifndef EXAMPLES_EX00_KEY_CONCEPTS_EX00_SUBSYSTEM_H_
#define EXAMPLES_EX00_KEY_CONCEPTS_EX00_SUBSYSTEM_H_

#include <cci_configuration>
#include <string>
#include "ex00_Param_Owner.h"
#include "ex00_Param_User.h"
#include "ex00_Param_Tracker.h"

/**
 *  @class  ex00_Subsystem
 */
SC_MODULE(ex00_Subsystem) {
 public:
  /**
   *  @fn     void ex00_Subsystem()
   *  @brief  The constructor for the class.
   *  @return void
   */

  SC_CTOR(ex00_Subsystem) 
	  : param_owner("param_owner"),
	    param_user("param_user"),
        param_track("param_track")        
  {  
  }
  ex00_Param_Owner param_owner;
  ex00_Param_User param_user;
  ex00_Param_Tracker param_track;
};
// ex00_Subsystem

#endif  // EXAMPLES_EX00_KEY_CONCEPTS_EX00_SUBSYSTEM_H_



