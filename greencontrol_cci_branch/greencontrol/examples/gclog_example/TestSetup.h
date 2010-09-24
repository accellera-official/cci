//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Michael Ruetz <m.ruetz@tu-bs.de>,
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doxygen comments

#ifndef __TESTSETUP_H__
#define __TESTSETUP_H__

enum TestCase {
  MISC,
  SIMPLE_CONSOLE,
  SIMPLE_TEXT,
  SIMPLE_DEBUG,
  TEXT_FILES,
  TEXT_CONFIG,
  SERVICE_FILTER,
  SERVICE_HEAD,
  CSV_CONFIG_DYNAMIC,
  CSV_CONFIG_STATIC,
  CSV_CONF_PERF,
  CSV_CONF_PERF_OPTIMIZED,
  CSV_HARD_PERF
};

using namespace gs::log;

class TestSetup
{
public:
  TestSetup()
  {
    gclog = NULL;

    passFilter = NULL;
    confFilter = NULL;
    servFilter = NULL;
    servFilterHead = NULL;

    textOut = NULL;
    textOut2 = NULL;
    consoleOut = NULL;
    debugOut = NULL;
    debugOut2 = NULL;
    confTextOut = NULL;
    confTextOut2 = NULL;
    csvOut = NULL;
    csvOut2 = NULL;
    csvOutSimple = NULL;

    logCfg = NULL;
  }

  ~TestSetup()
  {
    if(logCfg)
      delete logCfg;

    if(textOut)
      delete textOut;
    if(textOut2)
      delete textOut2;
    if(consoleOut)
      delete consoleOut;
    if(debugOut)
      delete debugOut;
    if(debugOut2)
      delete debugOut2;
    if(confTextOut)
      delete confTextOut;
    if(confTextOut2)
      delete confTextOut2;
    if(csvOut)
      delete csvOut;
    if(csvOut2)
      delete csvOut2;
    if(csvOutSimple)
      delete csvOutSimple;

    if(passFilter)
      delete passFilter;
    if(confFilter)
      delete confFilter;
    if(servFilter)
      delete servFilter;
    if(servFilterHead)
      delete servFilterHead;

    if(gclog)
      delete gclog;
  }

  void setupTest(TestCase test)
  {
    switch(test)
    {
      // case MISC see default
      case SIMPLE_CONSOLE:
        simpleConsole();
        break;
      case SIMPLE_TEXT:
        simpleText();
        break;
      case SIMPLE_DEBUG:
        simpleDebug();
        break;
      case TEXT_FILES:
        textFiles();
        break;
      case TEXT_CONFIG:
        textConfig();
        break;
      case SERVICE_FILTER:
        serviceFilter();
        break;
      case SERVICE_HEAD:
        serviceHead();
        break;
      case CSV_CONFIG_DYNAMIC:
        csvConfigDynamic();
        break;
      case CSV_CONFIG_STATIC:
        csvConfigStatic();
        break;
      case CSV_CONF_PERF:
        csvConfPerf();
        break;
      case CSV_CONF_PERF_OPTIMIZED:
        csvConfPerfOptimized();
        break;
      case CSV_HARD_PERF:
        csvHardPerf();
        break;
      default:
        SC_REPORT_WARNING("TestSetup", "Testcase not defined!");
        misc();
    }
  }

  void choose_warning() {
    std::cout << std::endl << "*******************************************" << std::endl;
    std::cout << std::endl << "Please choose a command line option (1-12):" << std::endl;
    std::cout << 
    "  1 = SIMPLE_CONSOLE," << std::endl<<
    "  2 = SIMPLE_TEXT," << std::endl<<
    "  3 = SIMPLE_DEBUG," << std::endl<<
    "  4 = TEXT_FILES," << std::endl<<
    "  5 = TEXT_CONFIG," << std::endl<<
    "  6 = SERVICE_FILTER," << std::endl<<
    "  7 = SERVICE_HEAD," << std::endl<<
    "  8 = CSV_CONFIG_DYNAMIC," << std::endl<<
    "  9 = CSV_CONFIG_STATIC," << std::endl<<
    "  10 = CSV_CONF_PERF," << std::endl<<
    "  11 = CSV_CONF_PERF_OPTIMIZED," << std::endl<<
    "  12 = CSV_HARD_PERF" << std::endl<< std::endl;
    std::cout << std::endl << "*******************************************" << std::endl;
  }
  
  void misc()
  {
    std::cout << std::endl << "Choose default (1=SIMPLE_CONSOLE):" << std::endl << std::endl;
    simpleConsole();
  }

  // Test the Console output.
  void simpleConsole()
  {
    gclog = new GC_Logger();
    consoleOut = new Console();

    gclog->setFilter(consoleOut);
  }

  // Test the Textfile output.
  void simpleText()
  {
    gclog = new GC_Logger();
    textOut = new Textfile("simpleText.txt");

    gclog->setFilter(textOut);
  }

  // Test the DebugTextfile output.
  void simpleDebug()
  {
    gclog = new GC_Logger();
    debugOut = new DebugTextfile("simpleDebug.txt");

    gclog->setFilter(debugOut);
  }

  // Use the text-outputs simultaniously without filtering. Therefore the Passthrough filter is needed. The outputs use
  // the same filter port. Since there is no LogConfig involved, the ConfigTextfile and normal Textfile output should
  // be identical.
  void textFiles()
  {
    gclog = new GC_Logger();
    passFilter = new Passthrough();
    textOut = new Textfile("textFiles_textOut.txt");
    confTextOut = new ConfigTextfile("textFiles_confTextOut.txt");
    debugOut = new DebugTextfile("textFiles_debugOut.txt");

    passFilter->addFilter(0, textOut);
    passFilter->addFilter(0, confTextOut);
    passFilter->addFilter(0, debugOut);
    gclog->setFilter(passFilter);
  }

  // Use a ConfigFilter and the text outputs. This shows the difference between an activated LogConfig and no LogConfig
  // while using different output filters. This shows the benefits of using output-filters just for logging and pure filters
  // just for filtering.
  void textConfig()
  {
    gclog = new GC_Logger();
    confFilter = new ConfigFilter();

    textOut = new Textfile("textConfig_textOut.txt");
    confTextOut = new ConfigTextfile("textConfig_confTextOut.txt");
    debugOut = new DebugTextfile("textConfig_debugOut.txt");

    textOut2 = new Textfile("textConfig_textOut_active.txt");
    confTextOut2 = new ConfigTextfile("textConfig_confTextOut_active.txt");
    debugOut2 = new DebugTextfile("textConfig_debugOut_active.txt");

    LogConfig logConf;
    logConf.reset(false);
    logConf.setService(true);
    logConf.setTarget(true);
    logConf.setCmd(true);
    logConf.setID(true);
    logConf.setLogConfig(true);
    logConf.setLogPointer(true);
    confFilter->setLogConfig(logConf);

    confFilter->addFilter(0, textOut);
    confFilter->addFilter(0, confTextOut);
    confFilter->addFilter(0, debugOut);

    confFilter->addFilter(1, textOut2);
    confFilter->addFilter(1, confTextOut2);
    confFilter->addFilter(1, debugOut2);
    gclog->setFilter(confFilter);
  }

  // This test shows the usage of the ServiceFilter. The number of filter ports and thereby the number of services to filter
  // are specified at creation-time. Also the filter ports are used in different ways. One CSV output uses two filter ports.
  // The other outputs are used for transactions belonging to different services.
  void serviceFilter()
  {
    gclog = new GC_Logger();
    servFilter = new ServiceFilter(4);
    consoleOut = new Console();
    csvOut = new CSV_File("serviceFilter_GCnf.csv");
    csvOutSimple = new Simple_CSV_File("serviceFilter_GAV.csv");
    csvOut2 = new CSV_File("serviceFilter_Combined.csv");

    csvOut->setHumanReadable(true);
    csvOut2->setHumanReadable(true);

    servFilter->addFilter(0, consoleOut);
    servFilter->addFilter(1, csvOut);
    servFilter->addFilter(2, csvOutSimple);
    servFilter->addFilter(1, csvOut2);
    servFilter->addFilter(2, csvOut2);

    gclog->setFilter(servFilter);
  }

  // This tests shows the benefits of using Head-Filters.
  void serviceHead()
  {
    gclog = new GC_Logger();
    servFilterHead = new ServiceFilterHead();

    gclog->setFilter(servFilterHead);
  }

  // This test shows how to use special LogConfigs for requesting CTs and the automatic configuration for returning CTs.
  // This can be achieved by using a ConfigFilter for requesting CTs and a Passthrough filter for returning CTs. The CTs
  // are filtered dynamically by using a LogConfig.
  void csvConfigDynamic()
  {
    gclog = new GC_Logger();
    passFilter = new Passthrough();
    confFilter = new ConfigFilter();
    csvOut = new CSV_File("csvConfigDynamic.csv");
    logCfg = new LogConfig();

    logCfg->reset(false);
    logCfg->setCmd(true);
    logCfg->setSpecifier(true);
    logCfg->setService(true);

    csvOut->setHumanReadable(true);

    passFilter->addFilter(0, csvOut);
    confFilter->setLogConfig(*logCfg);
    confFilter->addFilter(1, csvOut);

    gclog->setFilter(confFilter);
    gclog->setReturnFilter(passFilter);
  }

  // This test uses the static configuration of the CSV_File output. It shows the difference between a human readable and a
  // non human readable log.
  void csvConfigStatic()
  {
    gclog = new GC_Logger();
    passFilter = new Passthrough();
    csvOut = new CSV_File("csvConfigStatic_human.csv");
    csvOut2 = new CSV_File("csvConfigStatic.csv");
    logCfg = new LogConfig();

    logCfg->reset(false);
    logCfg->setCmd(true);
    logCfg->setLogPointer(true);

    csvOut->setHumanReadable(true);
    csvOut->setLogConfig(logCfg);

    csvOut2->setHumanReadable(false);
    csvOut2->setLogConfig(logCfg);

    passFilter->addFilter(0, csvOut);
    passFilter->addFilter(0, csvOut2);

    gclog->setFilter(passFilter);
  }

  // This test is used for performance comparison. It uses the writeDynamicTransaction() method of the CSV_File.
  void csvConfPerf()
  {
    gclog = new GC_Logger();
    confFilter = new ConfigFilter();
    csvOut = new CSV_File("csvConfPerf.csv");
    logCfg = new LogConfig();

    logCfg->reset(true);
    logCfg->setCmd(true); // doesn't change the LogConfig, but switches its state to "changed", so the unoptimized path of csvOut is used
//    logCfg->setTarget(false);
//    logCfg->setAnyPointer(false);
//    logCfg->setLogConfig(false);
//    logCfg->setService(false);
    csvOut->setHumanReadable(true);

    confFilter->setLogConfig(*logCfg);
    confFilter->addFilter(1, csvOut);

    gclog->setFilter(confFilter);
  }

  // This test is used for performance comparison. It uses the writeFullTransaction() method of the CSV_File.
  void csvConfPerfOptimized()
  {
    gclog = new GC_Logger();
    confFilter = new ConfigFilter();
    csvOut = new CSV_File("csvConfPerfOptimized.csv");
    logCfg = new LogConfig();

    logCfg->reset(true);
    csvOut->setHumanReadable(true);

    confFilter->setLogConfig(*logCfg);
    confFilter->addFilter(1, csvOut);

    gclog->setFilter(confFilter);
  }

  // This test is used for performance comparison. It uses the hardcoded Simple_CSV_File output.
  void csvHardPerf()
  {
    gclog = new GC_Logger();
    confFilter = new ConfigFilter();
    csvOutSimple = new Simple_CSV_File("csvHardPerf.csv");

    confFilter->addFilter(1, csvOutSimple);
    gclog->setFilter(confFilter);
  }

protected:
  // Logger
  gs::log::GC_Logger* gclog;

  // Filters
  gs::log::Passthrough* passFilter;
  gs::log::ConfigFilter* confFilter;
  gs::log::ServiceFilter* servFilter;
  gs::log::ServiceFilterHead* servFilterHead;

  // Outputs
  gs::log::Textfile* textOut;
  gs::log::Textfile* textOut2;
  gs::log::Console* consoleOut;
  gs::log::DebugTextfile* debugOut;
  gs::log::DebugTextfile* debugOut2;
  gs::log::ConfigTextfile* confTextOut;
  gs::log::ConfigTextfile* confTextOut2;
  gs::log::CSV_File* csvOut;
  gs::log::CSV_File* csvOut2;
  gs::log::Simple_CSV_File* csvOutSimple;

  // Configs
  gs::log::LogConfig* logCfg;
};

#endif
