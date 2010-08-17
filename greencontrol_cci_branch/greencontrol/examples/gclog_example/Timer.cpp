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

// Timer.cpp: Implementierung der Klasse CTimer.
//
//////////////////////////////////////////////////////////////////////

#include "Timer.h"
#include <windows.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CTimer::CTimer()
{
  frequency = 0;
  resolution = 0;
  timer_start = 0;
  timer_elapsed = 0;
  elapsed_time = 0;
  start_time = 0;
  end_time = 0;
  fTimeMultiplier = 1.0f;
}

CTimer::~CTimer()
{
}

void CTimer::Init(void)
{
  QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
  // Get The Current Time And Store It In performance_timer_start
  QueryPerformanceCounter((LARGE_INTEGER*) &timer_start);
  // Calculate The Timer Resolution Using The Timer Frequency
  resolution = ((1.0)/(double)frequency);
  // Set The Elapsed Time To The Current Time
  timer_elapsed = timer_start;
}

double CTimer::GetTime(void)
{
  __int64 time;

  QueryPerformanceCounter((LARGE_INTEGER*) &time);    // Grab The Current Performance Time
  // Return The Current Time Minus The Start Time Multiplied By The Resolution And 1000 (To Get MS)
  return ((double)(time - timer_start) * resolution) * 1000.0;
}

double CTimer::GetTime(int type)
{
  __int64 time;

  QueryPerformanceCounter((LARGE_INTEGER*) &time);    // Grab The Current Performance Time
  // Return The Current Time Minus The Start Time Multiplied By The Resolution And 1000 (To Get MS)
  if(type == SECONDS)
    return ((double)(time - timer_start) * resolution);
  else if(type == MINUTES)
    return ((double)(time - timer_start) * resolution) / 60.0;
  else
    return ((double)(time - timer_start) * resolution) * 1000.0;
}

float CTimer::GetMinutes(void)
{
  __int64  time, tmpTime;
  int    min, sec;

  QueryPerformanceCounter((LARGE_INTEGER*) &time);
  tmpTime = (double)((time - timer_start) * resolution);
  min = tmpTime / 60;
  sec = tmpTime % 60;
  return (float)((float)min + ((float)sec * 0.01f));
}

int CTimer::GetTimeInt(void)
{
  __int64 time;

  QueryPerformanceCounter((LARGE_INTEGER*) &time);    // Grab The Current Performance Time
  // Return The Current Time Minus The Start Time Multiplied By The Resolution And 1000 (To Get MS)
  return ((int)(time - timer_start) * resolution) * 1000.0;
}

int CTimer::GetTimeInt(int type)
{
  __int64 time;

  QueryPerformanceCounter((LARGE_INTEGER*) &time);    // Grab The Current Performance Time
  // Return The Current Time Minus The Start Time Multiplied By The Resolution And 1000 (To Get MS)
  if(type == SECONDS)
    return ((int)(time - timer_start) * resolution);
  else if(type == MINUTES)
    return ((int)(time - timer_start) * resolution) / 60.0;
  else
    return ((int)(time - timer_start) * resolution) * 1000.0;
}

double CTimer::GetElapsedTime(void)
{
  return elapsed_time;
}

void CTimer::StartFrame(void)
{
  QueryPerformanceCounter((LARGE_INTEGER*) &start_time);
}

double CTimer::EndFrame(void)
{
  QueryPerformanceCounter((LARGE_INTEGER*) &end_time);
  if(start_time != end_time)
    elapsed_time = (double)(end_time - start_time) * resolution;
  else
    elapsed_time = 1;
  elapsed_time *= fTimeMultiplier;
  return elapsed_time;
}

void CTimer::SetTimeMultiplier(float fMultiplier)
{
  fTimeMultiplier = fMultiplier;
}
