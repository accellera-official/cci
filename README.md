![SystemC][logo]
SystemC Configuration, Control and Inspection (CCI) Library
===========================================================

About SystemC CCI
-----------------

  The Configuration, Control and Inspection (CCI) standard enables greater 
  interoperability of SystemC models within the SystemC ecosystem. 
  It allows tools and models to work together to provide valuable user-level
  capabilities. The first phase of this standard addresses model configuration.
  The roadmap includes checkpointing and register introspection.

  The SystemC CCI Working Group addressed several key concerns while defining
  the 1.0 standard. Of critical importance is the ability to interact with 
  tools for which a portable value representation has been provided, along 
  with information about configuration parameters, including a description 
  and flexible metadata. Among the fundamental features in the standard are 
  avoiding name clashes with elements of the SystemC object hierarchy, 
  looking up and accessing configuration parameters, restricting access to 
  parameters, preloading configuration settings, callbacks for parameter 
  creation/destruction as well as value access, traceability of parameter 
  value updates, and support for user-defined parameter value types.

  This SystemC CCI package is the *reference implementation* provided by
  the [Accellera Systems Initiative][1] and is being developed by the 
  Accellera SystemC CCI Working Group.
  
  In the event of discrepancies between the behavior of this implementation and
  statements made in the [CCI 1.0 Language Reference Manual](https://www.accellera.org/images/downloads/standards/systemc/SystemC_CCI_1_0_LRM.pdf),
  the LRM standard shall be taken to be definitive.

  The library is divided into three sub-libraries: [`configuration`](configuration), [`control`](control), and
  [`inspection`](inspection). These sub-libraries can be found in the corresponding subfolders.

-------------------------------------------------------------------------------

### Licensing and Copyright

  See the separate [LICENSE](LICENSE) and [NOTICE](NOTICE) files to determine
  your rights and responsiblities for using SystemC.

### SystemC Community

  * SystemC community website  
    https://accellera.org/community/systemc/

  * SystemC Community discussion forum  
    https://forums.accellera.org/forum/9-systemc/

  * SystemC CCI discussion forum  
    https://forums.accellera.org/forum/41-systemc-cci-configuration-control-inspection/

About Accellera SystemC Working Groups
--------------------------------------

  Accellera hosts different SystemC Working Groups to address the various
  aspects of Electronic System-Level (ESL) modeling and simulation:
  
  * SystemC [Language Working Group][3]: Working Group responsible for the 
    definition and development of the SystemC and TLM core languages, the
    foundation on which all other SystemC libraries and functionality are built.
  
  * SystemC [CCI Working Group][6]: Working Group responsible for the 
    Configuration, Control and Inspection (CCI) extensions of SystemC, to
    enable tools to interact with models in order to perform activities 
    such as setup, debug and analysis.
  
  * SystemC [AMS Working Group][7]: Working Group responsible for the 
    Analog/Mixed-Signal extensions of SystemC, to enable abstract modeling 
    approaches for analog, mixed-signal and radio-frequency functionality.

  * SystemC [Synthesis Working Group][8]: Working Group responsible for the
    synthesizable subset of SystemC to enable High-Level Synthesis (HLS).
    
  * SystemC [Verification Working Group][9]: Working Group responsible for the
    verification extensions of SystemC, introducing UVM in SystemC library and 
    the SystemC Verification library (SCV).


About Accellera Systems Initiative
----------------------------------

  Accellera Systems Initiative is an independent, not-for profit organization
  dedicated to create, support, promote and advance system-level design,
  modeling and verification standards for use by the worldwide electronics
  industry.  The organization accelerates standards development and, as part of
  its ongoing partnership with the IEEE, its standards are contributed to the
  IEEE Standards Association for formal standardization and ongoing change
  control.  For more information, please visit [www.accellera.org][1].  Find out
  more about [membership][10]. Follow [@accellera][11] on Twitter or to comment,
  please use `#accellera`.  Accellera Global Sponsors are: Cadence; Mentor, A
  Siemens Business; and Synopsys.

_Accellera, Accellera Systems Initiative and SystemC are trademarks of
 Accellera Systems Initiative Inc. All other trademarks and trade names
 are the property of their respective owners._

[1]: https://accellera.org
[2]: https://ieeexplore.ieee.org/document/6134619/
[3]: https://accellera.org/activities/working-groups/systemc-language
[4]: https://accellera.org/activities/working-groups/systemc-tlm
[5]: https://accellera.org/activities/working-groups/systemc-datatypes
[6]: https://accellera.org/activities/working-groups/systemc-cci
[7]: https://accellera.org/activities/working-groups/systemc-ams
[8]: https://accellera.org/activities/working-groups/systemc-synthesis
[9]: https://accellera.org/activities/working-groups/systemc-verification
[10]: https://accellera.org/about/join/
[11]: https://twitter.com/accellera
[logo]: https://www.accellera.org/images/about/policies/logos/logo_systemc.gif


