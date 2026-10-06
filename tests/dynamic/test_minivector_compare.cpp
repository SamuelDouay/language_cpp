#include <catch2/catch_test_macros.hpp>
#include "MiniVector.hpp"

// =========================================================
// operator==
// =========================================================

TEST_CASE("operator== returns true for identical vectors", "[minivector][compare]")
{
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b = {1, 2, 3};

    REQUIRE(a == b);
}

TEST_CASE("operator== returns true for two empty vectors", "[minivector][compare]")
{
    MiniVector<int> a;
    MiniVector<int> b;

    REQUIRE(a == b);
}

TEST_CASE("operator== returns false for different sizes", "[minivector][compare]")
{
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b = {1, 2};

    REQUIRE_FALSE(a == b);
    REQUIRE_FALSE(b == a);
}

TEST_CASE("operator== returns false for same size but different elements",
          "[minivector][compare]")
{
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b = {1, 5, 3};

    REQUIRE_FALSE(a == b);
}

TEST_CASE("operator== on self returns true", "[minivector][compare]")
{
    MiniVector<int> a = {1, 2, 3};
    REQUIRE(a == a);
}

TEST_CASE("operator== differs only in last element", "[minivector][compare]")
{
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b = {1, 2, 4};

    REQUIRE_FALSE(a == b);
}

// =========================================================
// operator<
// =========================================================

TEST_CASE("operator< returns false for equal vectors", "[minivector][compare]")
{
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b = {1, 2, 3};

    REQUIRE_FALSE(a < b);
    REQUIRE_FALSE(b < a);
}

TEST_CASE("operator< compares first differing element", "[minivector][compare]")
{
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b = {1, 2, 4};

    REQUIRE(a < b);
    REQUIRE_FALSE(b < a);
}

TEST_CASE("operator< differs on first element", "[minivector][compare]")
{
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b = {2, 2, 3};

    REQUIRE(a < b);
    REQUIRE_FALSE(b < a);
}

TEST_CASE("operator< with shorter prefix is smaller", "[minivector][compare]")
{
    MiniVector<int> a = {1, 2};
    MiniVector<int> b = {1, 2, 3};

    REQUIRE(a < b);
    REQUIRE_FALSE(b < a);
}

TEST_CASE("operator< with longer vector that has smaller first element",
          "[minivector][compare]")
{
    MiniVector<int> a = {2};
    MiniVector<int> b = {1, 2, 3};

    // a[0] = 2 > b[0] = 1, donc a > b et b < a
    REQUIRE_FALSE(a < b);
    REQUIRE(b < a);
}

TEST_CASE("operator< on empty vs non-empty", "[minivector][compare]")
{
    MiniVector<int> a;
    MiniVector<int> b = {1};

    REQUIRE(a < b);
    REQUIRE_FALSE(b < a);
}

TEST_CASE("operator< on two empty vectors", "[minivector][compare]")
{
    MiniVector<int> a;
    MiniVector<int> b;

    REQUIRE_FALSE(a < b);
    REQUIRE_FALSE(b < a);
}

TEST_CASE("operator< strict weak ordering consistency", "[minivector][compare]")
{
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b = {1, 2, 4};
    MiniVector<int> c = {1, 3, 0};

    // Transitivité
    REQUIRE(a < b);
    REQUIRE(b < c);
    REQUIRE(a < c);

    // Irréflexivité
    REQUIRE_FALSE(a < a);
    REQUIRE_FALSE(b < b);
    REQUIRE_FALSE(c < c);

    // Antisymétrie
    REQUIRE_FALSE(b < a);
    REQUIRE_FALSE(c < b);
    REQUIRE_FALSE(c < a);
}

TEST_CASE("operator< works with strings", "[minivector][compare]")
{
    MiniVector<std::string> a = {"apple", "banana"};
    MiniVector<std::string> b = {"apple", "cherry"};

    REQUIRE(a < b);
    REQUIRE_FALSE(b < a);
}

TEST_CASE("operator< with capacity greater than size", "[minivector][compare]")
{
    MiniVector<int> a = {1, 2, 3};
    a.reserve(100);
    MiniVector<int> b = {1, 2, 3};
    b.reserve(50);

    // La capacité ne doit pas affecter la comparaison
    REQUIRE_FALSE(a < b);
    REQUIRE_FALSE(b < a);
    REQUIRE(a == b);
}

TEST_CASE("operator< lexicographic with equal prefix of different sizes",
          "[minivector][compare]")
{
    MiniVector<int> a = {1, 2, 3, 4, 5};
    MiniVector<int> b = {1, 2, 3};

    // b est un préfixe de a, donc b < a
    REQUIRE_FALSE(a < b);
    REQUIRE(b < a);
}

TEST_CASE("C++20 auto-generates >=, <=, >, !=", "[minivector][compare]")
{
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b = {1, 2, 4};

    REQUIRE(a < b);
    REQUIRE(a <= b);
    REQUIRE(b > a);
    REQUIRE(b >= a);
    REQUIRE(a != b);
    REQUIRE_FALSE(a > b);
    REQUIRE_FALSE(b <= a);
}