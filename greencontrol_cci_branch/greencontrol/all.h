//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#ifndef __all_h__
#define __all_h__

//
// This files includes many GreenControl files and
// many service classes that are located in the GreenControl
// namespaces gs::ctr, gs::cnf, gs::gc.
// The user simply needs to #include greencontrol/all.h
//
// TODO: Add here all includes.
//


// GreenControl
#include "greencontrol/core.h"    // Core

// GreenConfig
#include "greencontrol/config.h"  // configurable parameters gs_params
//#include "greencontrol/gcnf/plugin/scml_database_adaptor.h" // Alternative database
#include "greencontrol/gcnf/apis/configFileApi/configfile_tool.h"  // API Tool to read configuration file(s)
#include "greencontrol/gcnf/apis/commandLineConfigParserApi/commandLineConfigParser.h" // API Tool to process config command line parameters
//#include "greencontrol/gcnf/apis/commandLine_Tool/CommandLine_Tool.h" // Command Line Tool
#include "greencontrol/gcnf/apis/scmlApi/scml_property.h" // GreenConfig SCML property implementation
#include "greencontrol/gcnf/apis/toolApi/tool_gcnf_api.h" // tool API

#ifdef USE_LUA
#include "greencontrol/gcnf/apis/luaFileApi/luafile_tool.h" // lua file tool
#endif

// GreenAV
#include "greencontrol/gav/plugin/gav_plugin.h"
#include "greencontrol/gav/apis/gav_api/GAV_Api.h"
#include "greencontrol/gav/plugin/StatCalc.h"
#include "greencontrol/gav/plugin/Stdout_OutputPlugin.h"


#endif
