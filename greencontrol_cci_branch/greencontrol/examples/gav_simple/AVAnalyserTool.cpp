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
//   This program is free software.
// 
//   If you have no applicable agreement with GreenSocs Ltd, this software
//   is licensed to you, and you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   If you have a applicable agreement with GreenSocs Ltd, the terms of that
//   agreement prevail.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//   02110-1301  USA 
// 
// ENDLICENSETEXT


#include "AVAnalyserTool.h"
#include "greencontrol/analysis_file_outputplugin.h"
#include "greencontrol/analysis_csv_outputplugin.h"

using namespace std;
using namespace sc_core;

void AVAnalyserTool::main_action() {

  gs::gs_param_base *int_par  = m_configAPI->getPar("Owner.int_param");
  gs::gs_param_base *str_par  = m_configAPI->getPar("Owner.str_param");
  gs::gs_param_base *uint_par = m_configAPI->getPar("Owner.uint_param");

  gs::av::OutputPlugin_if* fileDefaultOP = m_analysisAPI.add_to_default_output(gs::av::TXT_FILE_OUT, int_par);
  GAV_REGR_TEST("PASS/FAIL: Passed if 'adding of param Owner.int_param to default Output Plugin TXT_FILE_OUT successfull'.");

  m_analysisAPI.add_to_default_output(gs::av::TXT_FILE_OUT, uint_par);
  GAV_REGR_TEST("PASS/FAIL: Passed if 'adding of param Owner.uint_param to default Output Plugin TXT_FILE_OUT successfull'.");
  
  m_analysisAPI.add_to_default_output(gs::av::TXT_FILE_OUT, uint_par);
  GAV_REGR_TEST("PASS/FAIL: Passed if 'adding of param Owner.uint_param to default Output Plugin TXT_FILE_OUT successfull BUT NOT adding it twice (the file must still only contain uint_par once per change)!'.");

  // test default type
  gs::gs_param<bool> param_for_default_op("param_for_default_op", true);
  gs::av::OutputPlugin_if* overallDefaultOP = m_analysisAPI.add_to_default_output(gs::av::DEFAULT_OUT, param_for_default_op);
  param_for_default_op = false;
  param_for_default_op = true;
  GAV_REGR_TEST("PASS/FAIL: Passed if 'adding of param AVTool.param_for_default_op to default Output Plugin DEFAULT_OUT successfull' and value changes occured there (e.g. on stdout if this is the default).");

  gs::gs_param<bool> param_for_default_opB("param_for_default_opB", true);
  m_analysisAPI.add_to_default_output(param_for_default_opB);
  GAV_REGR_TEST("PASS/FAIL: Passed if 'adding of param AVTool.param_for_default_opB to default Output Plugin DEFAULT_OUT successfull'.");

  gs::av::OutputPlugin_if* defaultOP = m_analysisAPI.create_OutputPlugin(gs::av::DEFAULT_OUT, "any");
  GAV_REGR_TEST("PASS/FAIL: Passed if 'created OutputPlugin of type DEFAULT_OUT (constructor param 'any') successfully'.");
  defaultOP = defaultOP;

  gs::av::OutputPlugin_if* overallDefaultOP2 = m_analysisAPI.get_default_output();
  if (overallDefaultOP == overallDefaultOP2) {
    GAV_REGR_TEST("PASSED: Passed get_default_output() test! (Returned the default output plugin)");
  } else {
    GAV_REGR_TEST("FAILED: Failes get_default_output() test! (Returned different default output plugin)");
    assert(false);
  }
  
  gs::av::OutputPlugin_if* fileDefaultOP2 = m_analysisAPI.get_default_output(gs::av::TXT_FILE_OUT);
  if (fileDefaultOP == fileDefaultOP2) {
    GAV_REGR_TEST("PASSED: Passed get_default_output(TXT_FILE_OUT) test! (Returned the default txt output plugin)");
  } else {
    GAV_REGR_TEST("FAILED: Failes get_default_output(TXT_FILE_OUT) test! (Returned different default txt output plugin)");
    assert(false);
  }
  
  // test different output types
  gs::av::OutputPlugin_if* csvFileOP = m_analysisAPI.create_OutputPlugin(gs::av::CSV_FILE_OUT, "CSVexample.log");
  GAV_REGR_TEST("PASS/FAIL: Passed if 'created OutputPlugin of type CSV_FILE_OUT (constructor param 'CSVexample.log.csv') successfully'.");
  csvFileOP->observe_all(*m_configAPI);
  GAV_REGR_TEST("PASS/FAIL: Call observe_all: Passed if file 'CSVexample.log.csv' contains all parameters existing at this point of time.");
  
  gs::av::OutputPlugin_if* fileOP = m_analysisAPI.create_OutputPlugin(gs::av::TXT_FILE_OUT, "secondFile.log");
  GAV_REGR_TEST("PASS/FAIL: Passed if 'created OutputPlugin of type TXT_FILE_OUT (constructor param 'secondFile.log') successfully'.");
  m_analysisAPI.add_to_output(fileOP, str_par);
  GAV_REGR_TEST("PASS/FAIL: Passed if 'adding of param Owner.str_param to Output Plugin with id 0x... successfull'.");
  m_analysisAPI.add_to_output(fileOP, uint_par);
  GAV_REGR_TEST("PASS/FAIL: Passed if 'adding of param Owner.uint_param to Output Plugin with id 0x... successfull'.");
  GAV_REGR_TEST("PASS/FAIL: Passed if file scondFile.log contains changed parameters str_param and uint_param)!");

  gs::gs_param<int> *par_to_be_deleted = new gs::gs_param<int>("par_to_be_deleted");
  m_analysisAPI.add_to_output(fileOP, par_to_be_deleted);
  *par_to_be_deleted = 10;
  wait(5, SC_NS);
  *par_to_be_deleted = 11;
  delete par_to_be_deleted; par_to_be_deleted = NULL;
  GAV_REGR_TEST("PASS/FAIL: Passed if no error occurs because of deleted observes parameter par_to_be_deleted.");
  
  m_analysisAPI.add_to_output(fileOP, int_par);
  GAV_REGR_TEST("PASS/FAIL: Passed if file scondFile.log contains parameter int_param beginning after 5 ns)!");

  wait(102, SC_NS);

  csvFileOP->pause(2, SC_NS);
  GAV_REGR_TEST("PASS/FAIL: Pause Output for 2 ns: Passed if file 'CSVexample.log.csv' does NOT contain parameter int_param at time point 107 ns!");
  
  fileOP->remove(*int_par);
  GAV_REGR_TEST("PASS/FAIL: Removed int_par: Passed if file scondFile.log contains parameter int_param only up to time point 106 ns)!");
  fileOP->remove(*int_par);
  GAV_REGR_TEST("PASS/FAIL: Removed int_par twice: Passed if file scondFile.log contains parameter int_param only up to time point 106 ns)!");

  wait (3, SC_NS);
  sc_event ev;
  csvFileOP->pause(ev);
  wait(100, SC_NS);
  ev.notify();
  wait(SC_ZERO_TIME);
  int_par->setString("10000");
  GAV_REGR_TEST("PASS/FAIL: Pause Output until event notified: Passed if file 'CSVexample.log.csv' does contain parameter int_param at time point 210 ns!");
  
  // test observe functions getting a vector or string
  fileOP->observe(m_configAPI->getParams("Owner"));
  GAV_REGR_TEST("PASS/FAIL: Passed output plugin observes all direct child parameters of 'Owner'.");

  fileOP->observe("Owner.*");
  GAV_REGR_TEST("PASS/FAIL: Passed output plugin observes all parameters of 'Owner.*' (including recursive childs). The sub-params were added.");
  
}
