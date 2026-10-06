#include <catch2/catch_test_macros.hpp>
#include "MiniVector.hpp"
#include "MoveTracker.hpp"
#include "Tracker.hpp"

// =========================================================
// swap — méthode membre
// =========================================================

TEST_CASE("member swap exchanges contents", "[minivector][swap]")
{
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b = {4, 5};

    a.swap(b);

    REQUIRE(a.size() == 2);
    REQUIRE(b.size() == 3);
    REQUIRE(a[0] == 4);
    REQUIRE(a[1] == 5);
    REQUIRE(b[0] == 1);
    REQUIRE(b[1] == 2);
    REQUIRE(b[2] == 3);
}

TEST_CASE("member swap exchanges capacities", "[minivector][swap]")
{
    MiniVector<int> a = {1, 2, 3};
    a.reserve(100);
    MiniVector<int> b = {4, 5};
    b.reserve(50);

    std::size_t cap_a = a.capacity();
    std::size_t cap_b = b.capacity();

    a.swap(b);

    REQUIRE(a.capacity() == cap_b);
    REQUIRE(b.capacity() == cap_a);
}

TEST_CASE("member swap with empty vector", "[minivector][swap]")
{
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b;

    a.swap(b);

    REQUIRE(a.empty());
    REQUIRE(b.size() == 3);
    REQUIRE(b[0] == 1);
    REQUIRE(b[2] == 3);
}

TEST_CASE("member swap between two empty vectors", "[minivector][swap]")
{
    MiniVector<int> a;
    MiniVector<int> b;

    a.swap(b);

    REQUIRE(a.empty());
    REQUIRE(b.empty());
}

TEST_CASE("member swap with self does nothing", "[minivector][swap]")
{
    MiniVector<int> a = {1, 2, 3};
    a.swap(a);

    REQUIRE(a.size() == 3);
    REQUIRE(a[0] == 1);
    REQUIRE(a[2] == 3);
}

TEST_CASE("member swap does not destroy elements", "[minivector][swap]")
{
    Tracker::alive = 0;
    {
        MiniVector<Tracker> a;
        a.emplace_back(1);
        a.emplace_back(2);
        MiniVector<Tracker> b;
        b.emplace_back(3);

        REQUIRE(Tracker::alive == 3);

        a.swap(b);

        // Aucun objet détruit, aucun construit
        REQUIRE(Tracker::alive == 3);
        REQUIRE(a.size() == 1);
        REQUIRE(b.size() == 2);
        REQUIRE(a[0].value == 3);
        REQUIRE(b[0].value == 1);
        REQUIRE(b[1].value == 2);
    }
    REQUIRE(Tracker::alive == 0);
}

// =========================================================
// swap — fonction libre (appelée via ADL)
// =========================================================

TEST_CASE("free swap exchanges contents", "[minivector][swap]")
{
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b = {4, 5};

    using std::swap; // fallback
    swap(a, b); // ADL trouve la version libre

    REQUIRE(a.size() == 2);
    REQUIRE(b.size() == 3);
    REQUIRE(a[0] == 4);
    REQUIRE(b[0] == 1);
    REQUIRE(b[2] == 3);
}

TEST_CASE("free swap exchanges capacities", "[minivector][swap]")
{
    MiniVector<int> a = {1, 2, 3};
    a.reserve(100);
    MiniVector<int> b = {4, 5};
    b.reserve(50);

    std::size_t cap_a = a.capacity();
    std::size_t cap_b = b.capacity();

    using std::swap;
    swap(a, b);

    REQUIRE(a.capacity() == cap_b);
    REQUIRE(b.capacity() == cap_a);
}

TEST_CASE("free swap works with std::swap fallback available",
          "[minivector][swap]")
{
    // Simule du code générique qui fait "using std::swap; swap(a, b);"
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b = {10, 20};

    using std::swap;
    swap(a, b);

    REQUIRE(a.size() == 2);
    REQUIRE(b.size() == 3);
    REQUIRE(a[0] == 10);
    REQUIRE(b[0] == 1);
}

TEST_CASE("free swap on empty vectors", "[minivector][swap]")
{
    MiniVector<int> a;
    MiniVector<int> b;

    using std::swap;
    swap(a, b);

    REQUIRE(a.empty());
    REQUIRE(b.empty());
}

TEST_CASE("free swap does not copy elements", "[minivector][swap]")
{
    MoveTracker::moves = 0;

    MiniVector<MoveTracker> a;
    a.push_back(MoveTracker(1));
    a.push_back(MoveTracker(2));

    MiniVector<MoveTracker> b;
    b.push_back(MoveTracker(3));

    int moves_before = MoveTracker::moves;

    using std::swap;
    swap(a, b);

    // swap ne doit faire AUCUN déplacement d'éléments (échange de pointeurs)
    REQUIRE(MoveTracker::moves == moves_before);

    REQUIRE(a.size() == 1);
    REQUIRE(b.size() == 2);
    REQUIRE(a[0].value == 3);
    REQUIRE(b[0].value == 1);
    REQUIRE(b[1].value == 2);
}

TEST_CASE("swap is noexcept", "[minivector][swap]")
{
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b = {4, 5};

    REQUIRE(noexcept(a.swap(b)));
    REQUIRE(noexcept(swap(a, b)));
}
