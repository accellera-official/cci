#ifndef TEMP_KNOB
#define TEMP_KNOB

#include <cci_inspection>
#include <cstring>
#include <string>

using ::cci::experimental::inspection::CAP_READWRITE;
using ::cci::experimental::inspection::capabilities;
using ::cci::experimental::inspection::item_if;

struct temp_knob {
  float target_temp;

  temp_knob(float init) : target_temp(init) {}
};

class temp_knob_item : public item_if {
public:
  temp_knob_item() = delete;
  temp_knob_item(std::string_view nm, std::shared_ptr<temp_knob> knob)
      : m_knob(knob), m_name(nm) {};
  ~temp_knob_item() = default;

  std::string_view get_name() const { return m_name; }

  capabilities get_capabilites() const { return CAP_READWRITE; }

  size_t get_size() const { return sizeof(float); }

  size_t peek(std::vector<uint8_t> &data, size_t start) const {
    if (start != 0 || data.size() != get_size())
      return 0;
    std::memcpy(data.data(), &m_knob->target_temp, get_size());
    return get_size();
  }

  size_t poke(const std::vector<uint8_t> &data, size_t start) {
    if (start != 0 || data.size() != get_size())
      return 0;
    std::memcpy(&m_knob->target_temp, data.data(), get_size());
    return get_size();
  }

private:
  std::string m_name;
  std::shared_ptr<temp_knob> m_knob;
};

#endif // TEMP_KNOB
