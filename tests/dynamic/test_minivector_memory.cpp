#include <catch2/catch_test_macros.hpp>
#include "MiniVector.hpp"
#include "MoveTracker.hpp"
#include "ThrowingCopy.hpp"
#include "Tracker.hpp"

// Les variables globales sont définies dans TestGlobals.cpp
// On les déclare extern ici (ou on inclut un header qui les déclare)

TEST_CASE("clear followed by push_back reconstructs objects", "[minivector][memory]") {
    Tracker::alive = 0;
    MiniVector<Tracker> v;
    v.push_back(Tracker(10));
    v.push_back(Tracker(20));
    REQUIRE(Tracker::alive == 2);
    v.clear();
    REQUIRE(Tracker::alive == 0);
    v.push_back(Tracker(30));
    REQUIRE(Tracker::alive == 1);
    REQUIRE(v.size() == 1);
    REQUIRE(v[0].value == 30);
}

TEST_CASE("reserve moves existing elements", "[minivector][memory]") {
    MoveTracker::moves = 0;
    MiniVector<MoveTracker> v;
    v.push_back(MoveTracker(1));
    v.push_back(MoveTracker(2));
    const int moves_before = MoveTracker::moves;
    v.reserve(100);
    REQUIRE(MoveTracker::moves > moves_before);
    REQUIRE(v[0].value == 1);
    REQUIRE(v[1].value == 2);
}

TEST_CASE("reallocate correctly destroys old objects", "[minivector][memory]") {
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v;
        v.push_back(Tracker(1));
        v.push_back(Tracker(2));
        v.push_back(Tracker(3));
        v.push_back(Tracker(4));
        v.push_back(Tracker(5));
        REQUIRE(Tracker::alive == 5);
        REQUIRE(v.size() == 5);
    }
    REQUIRE(Tracker::alive == 0);
}

TEST_CASE("pop_back destroys the removed element", "[minivector][memory]") {
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v;
        v.push_back(Tracker(1));
        REQUIRE(Tracker::alive == 1);
        v.pop_back();
        REQUIRE(Tracker::alive == 0);
    }
    REQUIRE(Tracker::alive == 0);
}

TEST_CASE("clear destroys all elements but keeps capacity", "[minivector][memory]") {
    Tracker::alive = 0;
    MiniVector<Tracker> v;
    v.push_back(Tracker(1));
    v.push_back(Tracker(2));
    v.push_back(Tracker(3));
    std::size_t cap = v.capacity();
    REQUIRE(Tracker::alive == 3);
    v.clear();
    REQUIRE(Tracker::alive == 0);
    REQUIRE(v.capacity() == cap);
}

TEST_CASE("push_back with rvalue moves element", "[minivector][memory]") {
    Tracker::alive = 0;
    MiniVector<Tracker> v;
    v.push_back(Tracker(42));
    REQUIRE(Tracker::alive == 1);
    REQUIRE(v[0].value == 42);
}

TEST_CASE("reallocate moves existing elements", "[minivector][memory]") {
    MoveTracker::moves = 0;
    MiniVector<MoveTracker> v;
    v.push_back(MoveTracker(1));
    v.push_back(MoveTracker(2));
    v.push_back(MoveTracker(3));
    REQUIRE(v[0].value == 1);
    REQUIRE(v[1].value == 2);
    REQUIRE(v[2].value == 3);
    REQUIRE(MoveTracker::moves > 0);
}

TEST_CASE("MiniVector works with non-assignable type", "[minivector][memory]") {
    MiniVector<Tracker> v;
    v.push_back(Tracker(10));
    v.push_back(Tracker(20));
    v.push_back(Tracker(30));
    REQUIRE(v.size() == 3);
    REQUIRE(v[0].value == 10);
    REQUIRE(v[1].value == 20);
    REQUIRE(v[2].value == 30);
}

TEST_CASE("swap does not destroy elements", "[minivector][memory]") {
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v1;
        v1.emplace_back(1);
        v1.emplace_back(2);
        MiniVector<Tracker> v2;
        v2.emplace_back(3);
        REQUIRE(Tracker::alive == 3);
        v1.swap(v2);
        REQUIRE(Tracker::alive == 3);
        REQUIRE(v1.size() == 1);
        REQUIRE(v2.size() == 2);
        REQUIRE(v1[0].value == 3);
        REQUIRE(v2[0].value == 1);
        REQUIRE(v2[1].value == 2);
    }
    REQUIRE(Tracker::alive == 0);
}

TEST_CASE("exception safety during reallocation", "[minivector][memory]") {
    ThrowingCopy::copy_count = 0;
    ThrowingCopy::throw_after = 2;
    MiniVector<ThrowingCopy> v;
    v.push_back(ThrowingCopy(1));
    REQUIRE_THROWS_AS(v.push_back(ThrowingCopy(2)), std::runtime_error);
    REQUIRE(v.size() == 1);
    REQUIRE(v[0].value == 1);
}
