README.txt
----------

gs_param_imlementation

Example CCI-Parameter implementation using GreenConfig parameters.


This creates a library with symbols for different types of CCI-Parameter implementations:

- The default make creates a subset of the std parameter types NOT
  including the template types sc_big(u)int<w>

- The shell script make_full_big_lib.sh creates the same library
  but including the template types sc_big(u)int<w>.
  This enlarges the library to about 700 MB.


--
Christian Schršder
christian.schroeder@greensocs.com