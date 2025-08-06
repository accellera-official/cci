#ifndef INPUT
#define INPUT

#include <cci_configuration>
#include <cci_inspection>
#include <cstdint>
#include <cstdlib>
#include <systemc>

using ::cci::experimental::inspection::discovery_if;
using ::cci::experimental::inspection::get_discovery_if;
using ::cci::experimental::inspection::item_if;

class input : public sc_core::sc_module {
public:
  input(const sc_core::sc_module_name &nm) : sc_core::sc_module(nm) {
    m_inspection_portal = get_discovery_if(CCI_INSPECTION_API_VERSION);
    if (!m_inspection_portal) {
      std::cerr << "Error: Could not get inspection discovery portal!"
                << std::endl;
      std::abort();
    }

    SC_HAS_PROCESS(input);

    SC_THREAD(update_accuracy_thread);
    SC_THREAD(update_temp_thread);
    SC_THREAD(print_temp_thread);
  }

  void update_accuracy_thread() {
    auto broker = cci::cci_get_broker();
    wait(15, sc_core::SC_SEC);
    cci::cci_param_handle accuracy_handle =
        broker.get_param_handle("climate_control.accuracy");
    if (!accuracy_handle.is_valid()) {
      std::cerr << "Error: accuracy configuration handler is not valid!"
                << std::endl;
      std::abort();
    }
    std::cout << "-> Decrease accuracy to 0.75" << std::endl;
    accuracy_handle.set_cci_value(cci::cci_value(0.75f));
  }

  void update_temp_thread() {
    item_if *knob_item = get_item("temp_knob");
    std::vector<uint8_t> data(knob_item->get_size() / sizeof(uint8_t), 0.0);
    float temp = 20.0;
    while (true) {
      wait(5, sc_core::SC_SEC);
      temp += 10;
      std::cout << "-> Raise target temp. to " << temp << " °C" << std::endl;
      std::memcpy(data.data(), &temp, knob_item->get_size());
      if (knob_item->poke(data, 0) != knob_item->get_size()) {
        std::cerr << "Error poking in temp_knob!" << std::endl;
        std::abort();
      }
    }
  }

  void print_temp_thread() {
    item_if *sensor_item = get_item("temp_sensor");
    std::vector<uint8_t> data(sensor_item->get_size() / sizeof(uint8_t), 0.0);
    float temp;
    while (true) {
      if (sensor_item->peek(data, 0) != sensor_item->get_size()) {
        std::cerr << "Error peeking in temp_sensor!" << std::endl;
        std::abort();
      }
      std::memcpy(&temp, data.data(), sizeof(float));
      std::cout << "Current temp.: " << temp << " °C" << std::endl;
      wait(1, sc_core::SC_SEC);
    }
  }

  item_if *get_item(std::string_view nm) {
    auto climate_items = m_inspection_portal->get_items("climate_control");
    auto item_iter = std::find_if(climate_items.begin(), climate_items.end(),
                                  [&nm](const std::shared_ptr<item_if> &item) {
                                    return item->get_name() == nm;
                                  });
    if (item_iter == climate_items.end()) {
      std::cerr << "Error: Could not find " << nm << " inspection item!"
                << std::endl;
      std::abort();
    }
    return item_iter->get();
  }

private:
  discovery_if *m_inspection_portal;
};

#endif // INPUT
