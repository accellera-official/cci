#include <systemc.h>
#include "observer.h"
#include "parameter_owner.h"

int sc_main(int sc_argc, char* sc_argv[])
{
	observer                 observer_inst;
	parameter_owner          param_owner("param_owner");

	sc_start(15.0, SC_NS);

	return EXIT_SUCCESS;
}
