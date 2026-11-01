#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include "DynamicBitset.hpp"

TEST_CASE("swap exchanges contents", "[bitset]") {
    DynamicBitset a("101");
    DynamicBitset b("11001");

    a.swap(b);

    REQUIRE(a.size() == 5);
    REQUIRE(b.size() == 3);
    REQUIRE(a.to_string() == "11001");
    REQUIRE(b.to_string() == "101");
}

TEST_CASE("free swap works via ADL", "[bitset]") {
    DynamicBitset a("101");
    DynamicBitset b("110");

    using std::swap;
    swap(a, b);

    REQUIRE(a.to_string() == "110");
    REQUIRE(b.to_string() == "101");
}

TEST_CASE("operator<< prints the bitset", "[bitset]") {
    DynamicBitset bs("10110");
    std::ostringstream oss;

    oss << bs;

    REQUIRE(oss.str() == "10110");
}
