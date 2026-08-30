#include <catch2/catch_test_macros.hpp>
#include "MiniVector.hpp"

TEST_CASE("push_back adds elements and grows capacity", "[minivector][modifier]") {
    MiniVector<int> v;
    const int n = 100;
    for (int i = 0; i < n; ++i)
        v.push_back(i);
    REQUIRE(v.size() == static_cast<std::size_t>(n));
    REQUIRE(v.capacity() >= v.size());
    for (int i = 0; i < n; ++i)
        REQUIRE(v[static_cast<std::size_t>(i)] == i);
}

TEST_CASE("pop_back removes last element", "[minivector][modifier]") {
    MiniVector<int> v = {1, 2, 3};
    v.pop_back();
    REQUIRE(v.size() == 2);
    REQUIRE(v[1] == 2);
}

TEST_CASE("clear empties vector but keeps capacity", "[minivector][modifier]") {
    MiniVector<int> v = {1, 2, 3};
    std::size_t cap = v.capacity();
    v.clear();
    REQUIRE(v.empty());
    REQUIRE(v.capacity() == cap);
}

TEST_CASE("reserve preserves existing elements", "[minivector][modifier]") {
    MiniVector<int> v = {1, 2, 3};
    v.reserve(100);
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() >= 100);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
}

TEST_CASE("reserve does not shrink capacity", "[minivector][modifier]") {
    MiniVector<int> v = {1, 2, 3, 4, 5};
    std::size_t old_cap = v.capacity();
    v.reserve(2);
    REQUIRE(v.capacity() == old_cap);
    v.reserve(old_cap + 1);
    REQUIRE(v.capacity() >= old_cap + 1);
}

TEST_CASE("swap exchanges contents and capacities", "[minivector][modifier]") {
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b = {4, 5};
    std::size_t capA = a.capacity();
    std::size_t capB = b.capacity();
    a.swap(b);
    REQUIRE(a.size() == 2);
    REQUIRE(b.size() == 3);
    REQUIRE(a.capacity() == capB);
    REQUIRE(b.capacity() == capA);
    REQUIRE(a[0] == 4);
    REQUIRE(a[1] == 5);
    REQUIRE(b[0] == 1);
    REQUIRE(b[1] == 2);
    REQUIRE(b[2] == 3);
}

TEST_CASE("emplace_back constructs element in place", "[minivector][modifier]") {
    struct Point {
        int x, y;

        Point(int a, int b) : x(a), y(b) {
        }
    };
    MiniVector<Point> v;
    v.emplace_back(1, 2);
    v.emplace_back(3, 4);
    REQUIRE(v.size() == 2);
    REQUIRE(v[0].x == 1);
    REQUIRE(v[0].y == 2);
    REQUIRE(v[1].x == 3);
    REQUIRE(v[1].y == 4);
}

TEST_CASE("emplace_back works with non-copyable type", "[minivector][modifier]") {
    struct NonCopyable {
        int value;

        explicit NonCopyable(int v) : value(v) {
        }

        NonCopyable(const NonCopyable &) = delete;

        NonCopyable & operator=(const NonCopyable &) = delete;

        NonCopyable(NonCopyable &&other) noexcept : value(other.value) {
        }

        NonCopyable & operator=(NonCopyable &&) = default;
    };
    MiniVector<NonCopyable> v;
    v.emplace_back(42);
    REQUIRE(v.size() == 1);
    REQUIRE(v[0].value == 42);
}

TEST_CASE("push_back handles self-reference during reallocation",
          "[minivector][modifier]") {
    MiniVector<int> v = {1, 2, 3};

    v.push_back(v[0]);

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 1);
}

TEST_CASE("emplace_back handles self-reference during reallocation",
          "[minivector][modifier]") {
    MiniVector<int> v = {1, 2, 3};

    v.emplace_back(v[0]);

    REQUIRE(v.size() == 4);
    REQUIRE(v[3] == 1);
}

TEST_CASE("reserve on empty vector allocates capacity",
          "[minivector][modifier]") {
    MiniVector<int> v;

    v.reserve(100);

    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() >= 100);
    REQUIRE(v.data() != nullptr);
}
