GreenSocs README file
---------------------
GreenControl project
  including sub projects
    GreenConfig
    GreenAV
    ReportMessages

GreenControl is a header-only framework.

Documentation
-------------

Project website: http://www.greensocs.com/en/projects/GreenControl

Directory "./docs" contains User Manuals and Tutorials for 
GreenControl as well as for the sub projects.

Include Files
-------------

Most header files that are sensible to include by the user are 
located at the top-level. Only experimental or insignificant 
features are not available at top-level.

Directory structure
-------------------
.        User include files
./core    GreenControl Core files
./debian  Files for GSPM (GreenSocs Package Management)
./docs    Documentation including for sub projects
             doxygen generation files
             User Manuals
             Tutorials
./examples       Examples, see README.txt there
./gav            GreenAV sub project
./gcnf           GreenConfig sub project
./gcnf_watcher   GreenConfig Logger
./reportmsg      Report Messages sub project