#include <algorithm>
#include <cci_inspection>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <ostream>
#include <vector>

using ::cci::experimental::inspection::CAP_READ;
using ::cci::experimental::inspection::capabilities;
using ::cci::experimental::inspection::discovery_if;
using ::cci::experimental::inspection::item_if;

const constexpr uint32_t init_temp_val = 20;
const constexpr uint32_t new_temp_val = 25;

inline uint32_t read_uint32_t(const std::vector<uint8_t> &data) {
  return *reinterpret_cast<const uint32_t *>(data.data());
}

class temperature_sensor {
public:
  temperature_sensor(uint32_t init) : m_val(init) {};
  ~temperature_sensor() = default;

  uint32_t get_temp() const { return m_val; }
  void set_temp(const uint32_t new_temp) { m_val = new_temp; }

private:
  uint32_t m_val;
};

class sensor_adapter : public item_if {
public:
  sensor_adapter(std::string_view nm,
                 std::shared_ptr<temperature_sensor> sensor)
      : item_if(), m_name(nm), m_sensor(sensor) {};
  ~sensor_adapter() = default;

  std::string_view get_name() const { return m_name; }

  capabilities get_capabilities() const { return CAP_READ; }

  size_t get_size() const { return sizeof(m_sensor->get_temp()); }

  size_t peek(std::vector<uint8_t> &data, size_t start) const {
    if (start != 0)
      return 0;
    const uint32_t value = m_sensor->get_temp();
    data.reserve(sizeof(value));
    std::memcpy(data.data(), &value, sizeof(value));
    return sizeof(value);
  }

  size_t poke(const std::vector<uint8_t> &data, size_t start) { return 0; }

private:
  std::string m_name;
  std::shared_ptr<temperature_sensor> m_sensor;
};

int main() {
  std::shared_ptr<temperature_sensor> sensor =
      std::make_shared<temperature_sensor>(init_temp_val);

  discovery_if *portal = ::cci::experimental::inspection::get_discovery_if(
      CCI_INSPECTION_API_VERSION);
  std::shared_ptr<item_if> item0 =
      std::make_shared<sensor_adapter>("temp", sensor);
  if (!portal->register_item(item0, "car.climate")) {
    std::cerr << "Error registering item!" << std::endl;
    return 1;
  }

  auto climate = portal->get_items("car.climate");
  auto item_iter = std::find_if(climate.begin(), climate.end(),
                                [](const std::shared_ptr<item_if> &item) {
                                  return item->get_name() == "temp";
                                });
  if (item_iter == climate.end()) {
    std::cerr << "Cannot find item 'temp' in 'car.climate'" << std::endl;
    return 1;
  }
  auto sensor_item = item_iter->get();

  std::vector<uint8_t> data;
  if (sensor_item->peek(data, 0) != sizeof(uint32_t)) {
    std::cerr << "Error reading initial temperature value" << std::endl;
    return 1;
  }
  uint32_t temp = read_uint32_t(data);
  if (temp != init_temp_val) {
    std::cerr << "Unexpected temperature value read" << std::endl;
    return 1;
  } else {
    std::cout << "Current temperature: " << temp << "C" << std::endl;
  }

  sensor->set_temp(new_temp_val);
  std::cout << "Setting temperature to " << new_temp_val << "C" << std::endl;

  data.clear();
  if (sensor_item->peek(data, 0) != sizeof(uint32_t)) {
    std::cerr << "Error reading new temperature value" << std::endl;
    return 1;
  }
  temp = read_uint32_t(data);
  if (temp != new_temp_val) {
    std::cerr << "Unexpected temperature value read" << std::endl;
    return 1;
  } else {
    std::cout << "Current temperature: " << temp << "C" << std::endl;
  }

  return 0;
}
