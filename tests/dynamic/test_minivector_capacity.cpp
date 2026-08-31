#include "MiniVector.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("max_size is valid", "[minivector][capacity]")
{
    MiniVector<int> v;

    REQUIRE(v.max_size() > 0);
    REQUIRE(v.max_size() >= v.capacity());
}

TEST_CASE("reserve rejects capacity greater than max_size",
          "[minivector][capacity]")
{
    MiniVector<int> v;

    REQUIRE_THROWS_AS(
        v.reserve(v.max_size() + 1),
        std::length_error
    );
}
