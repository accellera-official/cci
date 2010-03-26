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

// SystemC included in globals!

/*
 * This is an example for how to use the GreenAV Statstics Calculator 
 * and its Trigger and Caculator.
 *
 * At the same time this example performs regression tests:
 * - AVnewStatCalc tests the trigger abilities 
 *   and the sliding window of the calculator
 * - AVnewStatCalcFormulaTest tests the Calculator abilities (formula)
 */

#include "analysis_statcalc_globals.h"

// GreenConfig
#include "greencontrol/config.h" 

// GreenAV
#include "greencontrol/gav/plugin/gav_plugin.h"

// Example
#include "AVnewStCalc.h"
#include "AVnewStatCalcFormulaTest.h"

//#include "ShowSCObjects.h"

/// Average calculator function to be added to the Calculator<int>
static int average(int a, int b, bool sloppy) {
  return (a + b) / 2;
}

/// Test class surrounding the static calculation function
class Surrounding_Class {
public:
  /// Calculator function to be added to the Calculator<double>
  static double divide_by_three(const double a, const double b, bool sloppy) {
    return (a + b) / 3;
  }
};


/// Testbench for the GreenControl analysis (GreenAV) example
int sc_main(int argc, char *argv[]) {
  
  // GreenControl Core instance
  gs::ctr::GC_Core       core;

  // GreenConfig Plugin
  gs::cnf::ConfigDatabase cnfdatabase("ConfigDatabase");
  gs::cnf::ConfigPlugin configPlugin(&cnfdatabase);

  // GreenAV Plugin
  gs::av::GAV_Plugin analysisPlugin("AnalysisPlugin");
  
  AVnewStatCalc             tool("AVnewStatCalc");
  AVnewStatCalcFormulaTest  formulaTest("AVnewStatCalcFormulaTest");

  // Add user defined calculation functions to Calculator class
  // This function will be usable by any Calculator<int> and Calculator_bit<int>
  gs::av::Calculator<int>::addFunc(&average,"average"); 
  // This function will be usable by any Calculator<double> and Calculator_bit<double>
  gs::av::Calculator<double>::addFunc(&Surrounding_Class::divide_by_three,"div3"); 

  std::cout << "-------------------- SIMULATION START -----------------------" << std::endl;

  sc_core::sc_start();
  
  std::cout << "-------------------- SIMULATION ENDED -----------------------" << std::endl;
  
  //ShowSCObjects::showSCObjects();

  return EXIT_SUCCESS; 
  
}
