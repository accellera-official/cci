//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2009 : GreenSocs Ltd
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


#include "AVscvAnalyserTool.h"
#include "greencontrol/analysis_scv_outputplugin.h"

void AVscvAnalyserTool::main_action() {

  wait(1, SC_NS);
  
  gs::gs_param_base *int_par  = m_configAPI->getPar("Owner.int_param");
  gs::gs_param_base *str_par  = m_configAPI->getPar("Owner.str_param");
  gs::gs_param_base *uint_par = m_configAPI->getPar("Owner.uint_param");
  
  m_analysisAPI.add_to_default_output(gs::av::SCV_STREAM_OUT, int_par);
  GAV_REGR_TEST("PASS/FAIL: Passed if 'adding of param Owner.int_param to default Output Plugin SCV_STREAM_OUT successfull'.");

  m_analysisAPI.add_to_default_output(gs::av::SCV_STREAM_OUT, str_par);
  GAV_REGR_TEST("PASS/FAIL: Passed if 'adding of param Owner.str_param to default Output Plugin SCV_STREAM_OUT successfull'.");

  wait (2, SC_NS);

  m_analysisAPI.add_to_default_output(gs::av::SCV_STREAM_OUT, uint_par);
  GAV_REGR_TEST("PASS/FAIL: Passed if 'adding of param Owner.uint_param to default Output Plugin SCV_STREAM_OUT successfull'.");

  uint_par->setString("2000");

  wait (2, SC_NS);
  uint_par->setString("3000");

  str_par->setString("new hello");
  int_par->setString("222");  

  wait(100, SC_NS);

  gs::av::OutputPlugin_if* op = m_analysisAPI.create_OutputPlugin(gs::av::SCV_STREAM_OUT, "secondUserDefinedSCVop");
  GAV_REGR_TEST("PASS/FAIL: Passed if 'adding of param Owner.str_param to default Output Plugin SCV_STREAM_OUT successfull'.");
  op->resume(); // start output
  
  m_analysisAPI.add_to_output(op, int_par);
  GAV_REGR_TEST("PASS/FAIL: Passed if 'adding of param Owner.int_param to Output Plugin secondUserDefinedSCVop successfull and changes occur on the output (file or tool)'.");

  wait(1, SC_NS);
  int_par->setString("133");

  wait(1, SC_NS);
  int_par->setString("42");

  // create local parameter that will be deleted after some tests to test the delete mechanism
  gs::gs_param<int> *tmp_par = new gs::gs_param<int>("tmp_par", 99);
  m_analysisAPI.add_to_output(op, tmp_par);
  wait(5, SC_NS);
  *tmp_par = 77;
  wait(5, SC_NS);
  delete tmp_par;
  GAV_REGR_TEST("PASS/FAIL: Passed if parameter tmp_par is removed from the scv OutputPlugin 'secondUserDefinedSCVop' and the transaction has ended (see database!).");

  int_par->setString("4422");
  
  wait(1, SC_NS);
}
