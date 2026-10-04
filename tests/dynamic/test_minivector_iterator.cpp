#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <ranges>
#include "MiniVector.hpp"

TEST_CASE("iterators work with range-based for", "[minivector][iterator]") {
    MiniVector<int> v = {1, 2, 3, 4, 5};
    int sum = 0;
    for (int x: v) {
        sum += x;
    }
    REQUIRE(sum == 15);
}

TEST_CASE("begin/end on empty vector return same pointer", "[minivector][iterator]") {
    MiniVector<int> v;
    REQUIRE(v.begin() == v.end());
    REQUIRE(v.begin() == nullptr);
}

TEST_CASE("const begin/end work", "[minivector][iterator]") {
    const MiniVector<int> v = {1, 2, 3};
    const int *b = v.begin();
    const int *e = v.end();
    REQUIRE(e - b == 3);
    REQUIRE(b[0] == 1);
    REQUIRE(b[2] == 3);
}

// =========================================================
// cbegin / cend
// =========================================================

TEST_CASE("cbegin/cend return const iterators", "[minivector][iterator]") {
    MiniVector<int> v = {1, 2, 3, 4, 5};

    auto b = v.cbegin();
    auto e = v.cend();

    REQUIRE(e - b == 5);
    REQUIRE(*b == 1);
    REQUIRE(*(e - 1) == 5);
}

TEST_CASE("cbegin/cend on const vector", "[minivector][iterator]") {
    const MiniVector<int> v = {10, 20, 30};

    const auto *b = v.cbegin();
    const auto *e = v.cend();

    REQUIRE(e - b == 3);
    REQUIRE(b[0] == 10);
    REQUIRE(b[2] == 30);
}

TEST_CASE("cbegin/cend on empty vector", "[minivector][iterator]") {
    MiniVector<int> const v;

    REQUIRE(v.cbegin() == v.cend());
}

// =========================================================
// rbegin / rend (parcours inversé)
// =========================================================

TEST_CASE("rbegin/rend traverse in reverse order", "[minivector][iterator]") {
    MiniVector<int> v = {1, 2, 3, 4, 5};

    std::vector<int> reversed;
    for (int const &it: std::views::reverse(v)) {
        reversed.push_back(it);
    }

    REQUIRE(reversed == std::vector<int>{5, 4, 3, 2, 1});
}

TEST_CASE("rbegin points to last element", "[minivector][iterator]") {
    MiniVector<int> v = {1, 2, 3};
    REQUIRE(*v.rbegin() == 3);
}

TEST_CASE("rbegin/rend on empty vector are equal", "[minivector][iterator]") {
    MiniVector<int> v;
    REQUIRE(v.rbegin() == v.rend());
}

TEST_CASE("rbegin/rend on const vector", "[minivector][iterator]") {
    const MiniVector<int> v = {1, 2, 3};

    auto it = v.rbegin();
    REQUIRE(*it == 3);
    ++it;
    REQUIRE(*it == 2);
    ++it;
    REQUIRE(*it == 1);
    ++it;
    REQUIRE(it == v.rend());
}

// =========================================================
// crbegin / crend
// =========================================================

TEST_CASE("crbegin/crend traverse in reverse order", "[minivector][iterator]") {
    MiniVector<int> const v = {1, 2, 3, 4, 5};

    std::vector<int> reversed;
    for (int it: std::views::reverse(v)) {
        reversed.push_back(it);
    }

    REQUIRE(reversed == std::vector<int>{5, 4, 3, 2, 1});
}

TEST_CASE("crbegin/crend on const vector", "[minivector][iterator]") {
    const MiniVector<int> v = {1, 2, 3};

    auto it = v.crbegin();
    REQUIRE(*it == 3);
    ++it;
    REQUIRE(*it == 2);
    ++it;
    REQUIRE(*it == 1);
    ++it;
    REQUIRE(it == v.crend());
}

TEST_CASE("crbegin/crend on empty vector are equal", "[minivector][iterator]") {
    const MiniVector<int> v;
    REQUIRE(v.crbegin() == v.crend());
}

// =========================================================
// Interaction const / non-const
// =========================================================

TEST_CASE("non-const vector can be modified through iterator", "[minivector][iterator]") {
    MiniVector<int> v = {1, 2, 3};

    for (int &it: v) {
        it *= 10;
    }

    REQUIRE(v[0] == 10);
    REQUIRE(v[1] == 20);
    REQUIRE(v[2] == 30);
}

TEST_CASE("iterators support std::distance", "[minivector][iterator]") {
    MiniVector<int> v = {1, 2, 3, 4, 5};

    REQUIRE(std::distance(v.begin(), v.end()) == 5);
    REQUIRE(std::distance(v.cbegin(), v.cend()) == 5);
    REQUIRE(std::distance(v.rbegin(), v.rend()) == 5);
}

TEST_CASE("iterators support std::find", "[minivector][iterator]") {
    MiniVector<int> v = {1, 2, 3, 4, 5};

    auto *it = std::ranges::find(v.begin(), v.end(), 3);
    REQUIRE(it != v.end());
    REQUIRE(*it == 3);
}

TEST_CASE("iterators support std::reverse", "[minivector][iterator]") {
    MiniVector<int> v = {1, 2, 3, 4, 5};

    std::ranges::reverse(v.begin(), v.end());

    REQUIRE(v[0] == 5);
    REQUIRE(v[1] == 4);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 2);
    REQUIRE(v[4] == 1);
}
