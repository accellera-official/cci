The following example ex7_Parameter_Information has been tested on Linux OS only

1.  Within the example ex7_Parameter_Information directory, find
    
    a.  Documentation/Presentation within "docs" sub-directory
    
    b.  Expected output within the "reports" sub-directory



2.  Objective: Setting and querying parameter information

    Report all state for interesting example parameters; i.e. name, description, value, value origin, value validity, locked status, etc.



3.  The example shows various APIs in setting and querying parameter information at various time intervals.  Time stamps have been 
    illustrated to maintain easy readability and understanding of the example.


-----------------------
Modifications Done :
Date : 28th April, 2011
Author : PVS Phaneendra
========================

1.  The preset value for the std::string type cci-parameter is now set within the "sc_main()" before the instantiation of the 
    instantiation of the owner and configurer SC_MODULE(s).  Earlier, the value was set within the constructor of the configurer class.

2.  Re-arranged the code a bit within the configurer class to showcase better handling of the APIs

3.  Removed the few commented instructions (redundant information)

4.  Updated presentation in accordance with the code


-----------------------
Modifications Done :
Date : 17th August, 2011
Author : PVS Phaneendra
========================

1.  Appended the code for checking the value originator information

