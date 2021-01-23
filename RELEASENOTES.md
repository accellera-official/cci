Release Notes for SystemC CCI 1.0.1
===================================

**CONTENTS**
 * [Overview][overview]
 * [What's new in this release?][new]
 * [Known problems][problem]
 * [Reporting Bugs and Suggestions][bugs]


Overview
--------

This package contains the reference implementation for the SystemC 
Configuration, Control and Inspection (CCI) standard. It works optimally with
SystemC 2.3.3 utilizing its hierarchical name registry to detect and mitigate 
parameter name collisions. On prior versions of SystemC, it doesn't address name
collisions but otherwise works fine.


What's new in this release?
---------------------------
  
Compared to the previous release, 1.0.1 is a minor release containing the 
following enhancements and bug fixes:

 * Introduced Automake configuration and build flow (configure, make, 
   make install) for the CCI reference implementation and the examples

 * RapidJSON has become an external package dependency and needs to be added 
   as part of the configuration or using environment variable RAPIDJSON_HOME.

 * Improved build flow using Microsoft Visual C++ IDE, compiler and linker 
   for Windows, for the CCI reference implementation and examples.
   
 * Improved checking of the CCI examples.
 
 * Updated platform support, see the README.md.
 
 * Updated documentation.


Known problems
--------------

 * Convenience functions have been added to cci_broker_handle to register
   callbacks based on an object and a member function. This is now consistent 
   with parameter handle callback registration. This is not documented in the 
   CCI 1.0 LRM but will be added in the next revision.

 * Parameter names may be registered in the future when a preset value is
   provided to the broker. This would allow earlier detection of name
   collisions.  


Reporting Bugs and Suggestions
------------------------------

Please report bugs and suggestions about this library using the Accellera
forums:

  https://forums.accellera.org/forum/41-systemc-cci-configuration-control-inspection/

  --------------------------------------------------------------------------


[overview]: #overview
[new]: #whats-new-in-this-release
[problem]: #known-problems
[bugs]: #reporting-bugs-and-suggestions