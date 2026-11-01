#include <catch2/catch_test_macros.hpp>
#include "DynamicBitset.hpp"

TEST_CASE("test throws out_of_range on invalid index", "[bitset]") {
    DynamicBitset bs(4);
    REQUIRE_THROWS_AS(bs.test(4), std::out_of_range);
    REQUIRE_THROWS_AS(bs.test(100), std::out_of_range);
}
