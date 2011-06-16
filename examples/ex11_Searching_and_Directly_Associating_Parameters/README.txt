NOTE :  The following example ex11_Searching_and_Directly_Associating_Parameters has been tested on Linux OS only

1.  Within the example ex11_Searching_and_Directly_Associating_Parameters directory, find
  
    a.  Documentation/Presentation within "docs" sub-directory
    
    b.  Expected output within the "reports" sub-directory



2.  Objective:  To demonstrate the following :

    a.  Parameter Searches (R3), and 
    b.  Parameter Value Synchronization to address Different Names, Same Intended Meaning (UC9)
  
---------------------------
Modifications Done (if any):
Date :   9th June, 2011
Author : P V S Phaneendra
===========================

   1.  'get_param' broker API has been used as of now in order to keep the implementation
       simple and the eliminate all the warnings arising because of using different string
       patterns while searching for the desired parameters from the config_database


TODO List :
==========

  1.  Once the 'get_param_list' broker API is fixed (i.e., no warnings are issued), then
      the 'get_param_list' API will be used to retrieve the desired list of the cci_parameters
      within the model using string patterns
