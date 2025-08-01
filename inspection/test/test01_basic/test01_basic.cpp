#include <cci_inspection>
#include <catch2/catch_test_macros.hpp>
using ::cci::experimental::inspection::capabilities;
using ::cci::experimental::inspection::CAP_NONE;
using ::cci::experimental::inspection::item_if;
using ::cci::experimental::inspection::discovery_if;

class test_item : public item_if
{
public:
    test_item(std::string_view nm):
        ::cci::experimental::inspection::item_if(),
        m_name(nm) {};
    test_item() = delete;
    ~test_item() = default;

    virtual std::string_view get_name() const { return m_name; };

    virtual capabilities get_capabilities() const { return CAP_NONE; }

    virtual size_t get_size() const { return 0; };
    virtual size_t peek(std::vector<uint8_t>& data, size_t start) const { return 0; } ;
    virtual size_t poke(const std::vector<uint8_t>& data, size_t start) { return 0; };

private:
    std::string m_name;
};

TEST_CASE("Basic Test", "[discovery]") {
    using discovery_if = ::cci::experimental::inspection::discovery_if;
    using item_if = ::cci::experimental::inspection::item_if;

    discovery_if* portal = ::cci::experimental::inspection::get_discovery_if(CCI_INSPECTION_API_VERSION);

    std::shared_ptr<item_if> item0( new test_item("item0"));
    std::shared_ptr<item_if> item1( new test_item( "item1"));
    std::shared_ptr<item_if> item2( new test_item( "item2"));
    std::shared_ptr<item_if> item3( new test_item( "item3"));

    auto b = portal->register_item(item0, "top.level1.level2");
    REQUIRE(b);
    b = portal->register_item(item1, "top.level1.level2");
    REQUIRE(b);

    auto i = portal->get_items("top.level1.level2");
    REQUIRE(i.size() == 2);

    b = portal->unregister_item(item0, "top.level1.level2");
    REQUIRE(b);
    i = portal->get_items("top.level1.level2");
    REQUIRE(i.size() == 1);

    b = portal->register_item(item0, "top.level1.level2");
    REQUIRE(b);
    i = portal->get_items("top.level1.level2");
    REQUIRE(i.size() == 2);

    b = portal->register_item(item2, "top.level1");
    REQUIRE(b);
    b = portal->register_item(item3, "top");
    REQUIRE(b);
    b = portal->register_item(item3, "top");
    REQUIRE(b);

    auto l1 = portal->get_items("top.level1");
    REQUIRE(l1.size() == 1);

    CHECK(portal->get_name() == "CCI Discovery Interface");
}
