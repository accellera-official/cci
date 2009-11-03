#!/bin/sh

cd /home/greensocs/www/autodocs/greencontrol
/usr/bin/svk up
cd docs/GreenControl
/usr/bin/doxygen gc_doxygen_brief.conf 
/usr/bin/doxygen gc_doxygen_detailed.conf

cd /home/greensocs/www/autodocs/greencontrol
/usr/bin/svn up
cd docs/GreenConfig
/usr/bin/doxygen gcnf_doxygen_brief.conf 
/usr/bin/doxygen gcnf_doxygen_detailed.conf

cd /home/greensocs/www/autodocs/greencontrol
/usr/bin/svk up
cd docs/GreenAV
/usr/bin/doxygen gav_doxygen_brief.conf 
/usr/bin/doxygen gav_doxygen_detailed.conf

