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

#ifndef __OUTPUT_BASE_H__
#define __OUTPUT_BASE_H__

#include <time.h>
#include "greencontrol/gclog/filter_base.h"

namespace gs {
namespace log {


/// Abstract base class for output modules used by the GC_Logger.
/**
 * This class specializes the FilterBase class for output modules. It adds helper methods for unique filename and timestamp generation.
 * It may be used by output modules instead of the FilterBase class.
 */
class OutputBase : public FilterBase
{
protected:

  /// Helper function that appends the current timecode to the given filename.
  std::string generateUniqueFilename(std::string filename)
  {
    std::stringstream buf;
    std::string filetype = "";
    unsigned int i;

    // preserve filename extension
    i = filename.find_last_of('.');
    if(i != std::string::npos)
    {
      filetype = filename.substr(i);
      filename.erase(i);
    }

    buf << filename << "_" << getTimeStringShort() << filetype;

    return buf.str();
  }

  /// Helper function that generates a short string representation of the current time.
  std::string getTimeStringShort(void)
  {
    char timeStr[256];
    time_t startTime;
    tm* pTime;

    startTime = time(NULL);
    pTime = localtime(&startTime);
    strftime(timeStr, 256, "%Y%m%d_%H%M%S", pTime);

    return std::string(timeStr);
  }

  /// Helper function that generates a detailed string representation of the current time.
  std::string getTimeString(void)
  {
    char timeStr[256];
    time_t startTime;
    tm* pTime;

    startTime = time(NULL);
    pTime = localtime(&startTime);
    strftime(timeStr, 256, "%A, %d %B %Y - %X", pTime);

    return std::string(timeStr);
  }
};

  
} // end namespace log
} // end namespace gs

#endif
