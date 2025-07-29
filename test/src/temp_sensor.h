#ifndef TEMP_SENSOR
#define TEMP_SENSOR

#include <cci_inspection>
#include <cstring>
#include <string>

using ::cci::experimental::inspection::CAP_READ;
using ::cci::experimental::inspection::capabilities;
using ::cci::experimental::inspection::item_if;

struct temp_sensor {
  float curr_temp;

  temp_sensor(float init) : curr_temp(init) {}
};

class temp_sensor_item : public item_if {
public:
  temp_sensor_item() = delete;
  temp_sensor_item(std::string_view nm, std::shared_ptr<temp_sensor> sensor)
      : m_sensor(sensor), m_name(nm) {};
  ~temp_sensor_item() = default;

  std::string_view get_name() const override { return m_name; }

  capabilities get_capabilities() const override { return CAP_READ; }

  size_t get_size() const override { return sizeof(float); }

  size_t peek(std::vector<uint8_t> &data, size_t start) const override {
    if (start != 0 || data.size() != get_size())
      return 0;
    std::memcpy(data.data(), &m_sensor->curr_temp, get_size());
    return get_size();
  }

  size_t poke(const std::vector<uint8_t> &data, size_t start) override {
    return 0;
  }

private:
  std::string m_name;
  std::shared_ptr<temp_sensor> m_sensor;
};

#endif // TEMP_SENSOR
