// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
//       http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   MichaelRuetz <m.ruetz@tu-bs.de>,
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

// Timer.h: Schnittstelle für die Klasse CTimer.
//
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

#define SECONDS    0
#define MILLISECONDS  1
#define MINUTES    2

//////////////////////////////////////////////////////////////////////

class CTimer  
{
public:
  CTimer();
  virtual ~CTimer();
  void Init(void);
  int GetTimeInt(void);
  int GetTimeInt(int type);
  float GetMinutes(void);
  double GetTime(void);
  double GetTime(int type);
  double GetElapsedTime(void);
  void StartFrame(void);
  double EndFrame(void);
  void SetTimeMultiplier(float fMultiplier);

protected:
  double  resolution;    // Timer Resolution
  __int64  frequency;    // Timer Frequency
  __int64  timer_start;  // Performance Timer Start Value
  __int64  timer_elapsed;  // Performance Timer Elapsed Time
  double  elapsed_time;  // Elapsed Time between two Frames
  __int64  start_time;
  __int64  end_time;
  float    fTimeMultiplier;
};
