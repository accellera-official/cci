//   GreenConfig framework
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#ifndef __CONFIG_H__
#define __CONFIG_H__

//
// This file is the recommended file to include the basic
// GreenConfig service which is located in the namespace
// gs::cnf.
//
// The user simply needs to #include greencontrol/config.h
//
#define BEGIN_GS_CONFIG_NAMESPACE namespace gs { namespace cnf {

#define END_GS_CONFIG_NAMESPACE   }  }


#include "greencontrol/core/gc_core.h"                  // GreenControl middleware

#include "greencontrol/gcnf/plugin/config_globals.h"    // configuration globals
#include "greencontrol/gcnf/plugin/gcnf_datatypes.h"    // Command enumeration
#include "greencontrol/gcnf/plugin/param_db_if.h"       // allows connection of database through SystemC port
//#include "greencontrol/gcnf/plugin/plugin_if.h"         // interface for the configuration plugin
#include "greencontrol/gcnf/plugin/config_paramcallbadapt.h"  // callback adapters
#include "greencontrol/gcnf/plugin/config_callbadapt.h"       // callback adapters

#include "greencontrol/gcnf/apis/GCnf_Api/cnf_api_if.h"    // Configuration API interface (implemented by normal and private API)

BEGIN_GS_CONFIG_NAMESPACE
class ConfigPlugin;
END_GS_CONFIG_NAMESPACE
   
#include "greencontrol/gcnf/apis/GCnf_Api/GCnf_Api_t.h"                   // normal config API (without typedef)
#include "greencontrol/gcnf/apis/GCnf_private_Api/gcnf_private_api_t.h"   // private config API (without typedef)

BEGIN_GS_CONFIG_NAMESPACE
/// normal config API typedef
typedef GCnf_Api_t<gs_param_base, GCnf_private_Api_T<gs_param_base, gs_param, ConfigPlugin>, gs_param, ConfigPlugin> GCnf_Api; 
/// private config API typedef
typedef GCnf_private_Api_T<gs_param_base, gs_param, ConfigPlugin> GCnf_private_Api;
END_GS_CONFIG_NAMESPACE

#include "greencontrol/gcnf/apis/gs_param/gs_param_array.h"     // parameter array
// TODO: remove check
#define __ONLY_INCLUDED_BY_CONFIG_H__
#include "greencontrol/gcnf/apis/gs_param/gs_param_base.h"      // parameter base class
#undef __ONLY_INCLUDED_BY_CONFIG_H__
#include "greencontrol/gcnf/apis/gs_param/gs_param_class.h"     // parameters
#include "greencontrol/gcnf/apis/gs_param/gs_param_attribute_classes.hpp"     // parameters attributes

#include "greencontrol/gcnf/plugin/configplugin_t.h"                         // Config Plugin
  
#include "greencontrol/gcnf/apis/utils/show_database.h"

#endif
