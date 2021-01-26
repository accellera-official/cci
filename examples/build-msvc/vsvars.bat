@echo off
rem ***************************************************************************
rem
rem  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
rem  more contributor license agreements.  See the NOTICE file distributed
rem  with this work for additional information regarding copyright ownership.
rem  Accellera licenses this file to you under the Apache License, Version 2.0
rem  (the "License"); you may not use this file except in compliance with the
rem  License.  You may obtain a copy of the License at
rem
rem    http://www.apache.org/licenses/LICENSE-2.0
rem
rem  Unless required by applicable law or agreed to in writing, software
rem  distributed under the License is distributed on an "AS IS" BASIS,
rem  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
rem  implied.  See the License for the specific language governing
rem  permissions and limitations under the License.
rem
rem ***************************************************************************
rem
rem  vsvars.bat --
rem  Run this file in a command prompt to load MS Visual C++ environment
rem
rem  Original Author: Philipp A. Hartmann, OFFIS, 2013-12-09
rem
rem ***************************************************************************
rem
rem  You can specify the version and the platform to use via additional
rem  command-line options given to vsvars.bat, e.g.
rem
rem    vsvars.bat [arch]               # load MSVC 2010 for [arch]
rem    vsvars.bat [version]            # load MSVC [version] for x86
rem    vsvars.bat [version] [platform] # load MSVC [version] for [platform]
rem
rem    vsvars.bat 2010 amd64  # load 64-bit tools for MSVC 10.0
rem    vsvars.bat 11.0        # load default (x86) tools for MSVC 2012
rem    vsvars.vat x86_amd64   # load x64 cross-tools for MSVC 2010
rem
rem ***************************************************************************

set MSVC_VERSION=
set MSVC_PLATFORM=

if "%1" == "8.0"   goto check_MSVC80
if "%1" == "2005"  goto check_MSVC80
if "%1" == "9.0"   goto check_MSVC90
if "%1" == "2008"  goto check_MSVC90
if "%1" == "10.0"  goto check_MSVC100
if "%1" == "2010"  goto check_MSVC100
if "%1" == "11.0"  goto check_MSVC110
if "%1" == "2011"  goto check_MSVC110
if "%1" == "2012"  goto check_MSVC110
if "%1" == "12.0"  goto check_MSVC120
if "%1" == "2013"  goto check_MSVC120
if "%1" == "2014"  goto check_MSVC120
if "%1" == "14.0"  goto check_MSVC140
if "%1" == "2015"  goto check_MSVC140

if not "%1" == "" set MSVC_PLATFORM=%1
if     "%1" == "" set MSVC_PLATFORM=x86
goto check_MSVC100

rem We rely on the variables VSxxxCOMNTOOLS to be set by the MSVC
rem installation.  This should be usually the case by default.

:check_MSVC80
set MSVC_VERSION=8.0 (2005)
set VSINSTALLDIR=%VS80COMNTOOLS%..\..\
if "%SYSTEMC_MSVC%" == "" set SYSTEMC_MSVC=msvc80
if "%CCI_MSVC%" == "" set CCI_MSVC=msvc80
goto load_MSVC

:check_MSVC90
set MSVC_VERSION=9.0 (2008)
set VSINSTALLDIR=%VS90COMNTOOLS%..\..\
if "%SYSTEMC_MSVC%" == "" set SYSTEMC_MSVC=msvc90
if "%CCI_MSVC%" == "" set CCI_MSVC=msvc90
goto load_MSVC

:check_MSVC100
set MSVC_VERSION=10.0 (2010)
set VSINSTALLDIR=%VS100COMNTOOLS%..\..\
if "%SYSTEMC_MSVC%" == "" set SYSTEMC_MSVC=msvc10
if "%CCI_MSVC%" == "" set CCI_MSVC=msvc10
goto load_MSVC

:check_MSVC110
set MSVC_VERSION=11.0 (2012)
set VSINSTALLDIR=%VS110COMNTOOLS%..\..\
if "%SYSTEMC_MSVC%" == "" set SYSTEMC_MSVC=msvc11
if "%CCI_MSVC%" == "" set CCI_MSVC=msvc11
goto load_MSVC

:check_MSVC120
set MSVC_VERSION=12.0 (2013)
set VSINSTALLDIR=%VS120COMNTOOLS%..\..\
if "%SYSTEMC_MSVC%" == "" set SYSTEMC_MSVC=msvc12
if "%CCI_MSVC%" == "" set CCI_MSVC=msvc12
goto load_MSVC

:check_MSVC140
set MSVC_VERSION=14.0 (2015)
set VSINSTALLDIR=%VS140COMNTOOLS%..\..\
if "%SYSTEMC_MSVC%" == "" set SYSTEMC_MSVC=msvc14
if "%CCI_MSVC%" == "" set CCI_MSVC=msvc14
goto load_MSVC

:load_MSVC
if "%MSVC_PLATFORM%" == "" set MSVC_PLATFORM=%2
if "%MSVC_PLATFORM%" == "" set MSVC_PLATFORM=x86
if not exist "%SYSTEMC_HOME%\%SYSTEMC_MSVC%" goto error_no_SYSTEMC_MSVC_LIB
if not exist "%CCI_HOME%\%CCI_MSVC%" goto error_no_CCI_MSVC_LIB
if not exist "%VSINSTALLDIR%" goto error_no_MSVC_VERSION
set VCINSTALLDIR=%VSINSTALLDIR%VC\
if not exist "%VCINSTALLDIR%vcvarsall.bat" goto error_no_MSVC_VERSION
echo Loading settings for MS Visual C++ %MSVC_VERSION% (%MSVC_PLATFORM% platform)
call "%VCINSTALLDIR%vcvarsall.bat" %MSVC_PLATFORM%
goto end

:error_no_SYSTEMC_MSVC_LIB
echo SystemC directory %SYSTEMC_HOME%\%SYSTEMC_MSVC% not found.
echo Check your SystemC installation or set the SYSTEMC_MSVC environment variable manually.
goto end

:error_no_CCI_MSVC_LIB
echo SystemC CCI directory %CCI_HOME%\%CCI_MSVC% not found.
echo Check your SystemC installation or set the CCI_MSVC environment variable manually.
goto end

:error_no_MSVC_VERSION
echo MS Visual C++ %MSVC_VERSION% not found.
echo Could not load compiler environment.
echo Check your MS Visual C++ %MSVC_VERSION% installation.
goto end

:end
