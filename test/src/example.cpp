#include <cci_configuration>

#include "climate_control.h"
#include "input.h"

int sc_main(int argc, char** argv) {
    cci::cci_register_broker(new cci_utils::broker("Config broker"));

    climate_control climate_control("climate_control");
    input input("input");

    sc_core::sc_start(30, sc_core::SC_SEC);

    return EXIT_SUCCESS;
}
