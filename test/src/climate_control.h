#ifndef CLIMATE_CONTROL
#define CLIMATE_CONTROL

#include <cci_configuration>
#include <cci_inspection>
#include <systemc>

#include <ctime>
#include <iostream>

#include "temp_knob.h"
#include "temp_sensor.h"

using ::cci::experimental::inspection::discovery_if;
using ::cci::experimental::inspection::item_if;

class climate_control : public sc_core::sc_module {
public:
  climate_control(const sc_core::sc_module_name &nm)
      : sc_core::sc_module(nm), m_accuracy("accuracy", 0.25f),
        m_temp_sensor(std::make_shared<temp_sensor>(20.0)),
        m_temp_knob(std::make_shared<temp_knob>(20.0)) {

    std::cout << "Start values:" << std::endl
              << "  Sensor accurary : " << m_accuracy << std::endl
              << "  Sensor temp.    : " << m_temp_sensor->curr_temp << " °C"
              << std::endl
              << "  Desired temp.   : " << m_temp_knob->target_temp << " °C"
              << std::endl
              << std::endl;

    discovery_if *inspection_portal =
        cci::experimental::inspection::get_discovery_if(
            CCI_INSPECTION_API_VERSION);
    if (!inspection_portal) {
      std::cerr << "Error: Could not get inspection discovery portal!"
                << std::endl;
      std::abort();
    }

    std::shared_ptr<item_if> sensor_item =
        std::make_shared<temp_sensor_item>("temp_sensor", m_temp_sensor);
    std::shared_ptr<item_if> knob_item =
        std::make_shared<temp_knob_item>("temp_knob", m_temp_knob);

    if (!(inspection_portal->register_item(sensor_item, name()) &&
          inspection_portal->register_item(knob_item, name()))) {
      std::cerr << "Error: Could not register inspection item to portal!"
                << std::endl;
      std::abort();
    }

    SC_HAS_PROCESS(climate_control);

    SC_THREAD(update_temp);
  }

  void update_temp() {
    std::srand(std::time(0));
    while (true) {
      m_temp_sensor->curr_temp =
          m_temp_knob->target_temp +
          m_accuracy * (float)((std::rand() % 3) * 2 - 1);
      wait(1, sc_core::SC_SEC);
    }
  }

private:
  cci::cci_param<float> m_accuracy;
  std::shared_ptr<temp_sensor> m_temp_sensor;
  std::shared_ptr<temp_knob> m_temp_knob;
};

#endif // CLIMATE_CONTROL
