#include <algorithm>
#include <cci_inspection>
#include <cstdint>
#include <cstring>
#include <iostream>

using ::cci::experimental::inspection::CAP_READWRITE;
using ::cci::experimental::inspection::capabilities;
using ::cci::experimental::inspection::discovery_if;
using ::cci::experimental::inspection::item_if;

class answer {
public:
  answer() : m_data({42, 42, 42}) {};
  ~answer() = default;

protected:
  std::vector<uint8_t> m_data;
};

class answer_item : public item_if, public answer {
public:
  answer_item(std::string_view nm) : item_if(), answer(), m_name(nm) {};
  ~answer_item() = default;

  std::string_view get_name() const { return m_name; };

  capabilities get_capabilities() const { return CAP_READWRITE; }

  size_t get_size() const { return m_data.size(); };

  size_t peek(std::vector<uint8_t> &data, size_t start) const {
    if (start >= get_size())
      return 0;
    const size_t n = get_size() - start;
    data.resize(n);
    std::memcpy(data.data(), m_data.data() + start, n);
    return n;
  }

  size_t poke(const std::vector<uint8_t> &data, size_t start) {
    if (start >= get_size())
      return 0;
    const size_t n = std::min(get_size() - start, data.size());
    std::memcpy(m_data.data() + start, data.data(), n);
    return n;
  }

private:
  std::string m_name;
};

int main() {
  discovery_if *portal = ::cci::experimental::inspection::get_discovery_if(
      CCI_INSPECTION_API_VERSION);

  std::shared_ptr<item_if> item0 = std::make_shared<answer_item>("answer");
  if (!portal->register_item(item0, "top.h2g2.deepthought")) {
    std::cerr << "Error registering item!" << std::endl;
    return 1;
  }

  auto deepthought = portal->get_items("top.h2g2.deepthought");
  auto item_iter = std::find_if(deepthought.begin(), deepthought.end(),
                                [](const std::shared_ptr<item_if> &item) {
                                  return item->get_name() == "answer";
                                });
  if (item_iter == deepthought.end()) {
    std::cerr << "Cannot find item 'answer' in 'top.h2g2.deepthought'"
              << std::endl;
    return 1;
  }
  auto answer = item_iter->get();

  std::vector<uint8_t> data;
  std::vector<uint8_t> cmp = {42, 42, 42};
  if (answer->peek(data, 0) != cmp.size() || data != cmp) {
    std::cerr << "Peek 1: Comparison failed!" << std::endl;
    return 1;
  }

  data[0] = 43;
  if (answer->poke(data, 1) != 2) {
    std::cerr << "Poke failed!" << std::endl;
    return 1;
  }

  data.clear();
  cmp = {43, 42};
  if (answer->peek(data, 1) != cmp.size() || data != cmp) {
    std::cerr << "Peek 2: Comparison failed!" << std::endl;
    return 1;
  }

  return 0;
}
