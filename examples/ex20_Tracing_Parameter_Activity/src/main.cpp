#include <systemc.h>
#include "parameter_configurator.h"

int sc_main(int sc_argc, char* sc_argv[])
{
	parameter_configurator   param_cfgr("param_cfgr");

	sc_start(15.0, SC_NS);

	return EXIT_SUCCESS;
}
