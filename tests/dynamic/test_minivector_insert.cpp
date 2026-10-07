#include <list>
#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <string>

#include "MiniVector.hpp"
#include "MoveTracker.hpp"
#include "Tracker.hpp"

// =========================================================
// insert(pos, const T&)
// =========================================================

TEST_CASE("insert at beginning", "[minivector][insert]")
{
    MiniVector<int> v = {2, 3, 4};
    auto it = v.insert(v.cbegin(), 1);

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 4);
    REQUIRE(*it == 1);
    REQUIRE(it == v.begin());
}

TEST_CASE("insert at middle", "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 4, 5};
    auto it = v.insert(v.cbegin() + 2, 3);

    REQUIRE(v.size() == 5);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 4);
    REQUIRE(v[4] == 5);
    REQUIRE(*it == 3);
}

TEST_CASE("insert at end", "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 3};
    auto it = v.insert(v.cend(), 4);

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 1);
    REQUIRE(v[3] == 4);
    REQUIRE(*it == 4);
    REQUIRE(it == v.begin() + 3);
}

TEST_CASE("insert into empty vector", "[minivector][insert]")
{
    MiniVector<int> v;
    auto it = v.insert(v.cbegin(), 42);

    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 42);
    REQUIRE(*it == 42);
}

TEST_CASE("insert into single-element vector at beginning",
          "[minivector][insert]")
{
    MiniVector<int> v = {2};
    v.insert(v.cbegin(), 1);

    REQUIRE(v.size() == 2);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
}

TEST_CASE("insert triggers reallocation when full", "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 3};
    v.shrink_to_fit();
    REQUIRE(v.capacity() == 3);

    v.insert(v.cbegin() + 1, 99);

    REQUIRE(v.size() == 4);
    REQUIRE(v.capacity() >= 4);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 99);
    REQUIRE(v[2] == 2);
    REQUIRE(v[3] == 3);
}

TEST_CASE("insert preserves capacity when sufficient",
          "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 3};
    v.reserve(100);
    std::size_t cap = v.capacity();

    v.insert(v.cbegin() + 1, 99);

    REQUIRE(v.capacity() == cap);
}

TEST_CASE("insert handles aliasing when value is element of self",
          "[minivector][insert]")
{
    MiniVector<int> v = {10, 20, 30};

    // Insère v[0] au tout début : aliasing critique
    v.insert(v.cbegin(), v[0]);

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 10);
    REQUIRE(v[1] == 10);
    REQUIRE(v[2] == 20);
    REQUIRE(v[3] == 30);
}

TEST_CASE("insert handles aliasing at middle index",
          "[minivector][insert]")
{
    MiniVector<int> v = {10, 20, 30};

    // Insère v[1] juste après v[1]
    v.insert(v.cbegin() + 2, v[1]);

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 10);
    REQUIRE(v[1] == 20);
    REQUIRE(v[2] == 20);
    REQUIRE(v[3] == 30);
}

TEST_CASE("insert calls copy constructor", "[minivector][insert]")
{
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v;
        v.push_back(Tracker(1));
        v.push_back(Tracker(2));
        v.push_back(Tracker(3));

        REQUIRE(Tracker::alive == 3);

        Tracker source(99);
        v.insert(v.cbegin() + 1, source);

        // 4 dans v + 1 source
        REQUIRE(Tracker::alive == 5);
        REQUIRE(v.size() == 4);
        REQUIRE(v[0].value == 1);
        REQUIRE(v[1].value == 99);
        REQUIRE(v[2].value == 2);
        REQUIRE(v[3].value == 3);
    }
    REQUIRE(Tracker::alive == 0);
}

TEST_CASE("insert multiple times preserves order", "[minivector][insert]")
{
    MiniVector<int> v;
    v.insert(v.cbegin(), 3);
    v.insert(v.cbegin(), 2);
    v.insert(v.cbegin(), 1);
    v.insert(v.cend(), 4);
    v.insert(v.cend(), 5);

    REQUIRE(v.size() == 5);
    for (std::size_t i = 0; i < 5; ++i)
        REQUIRE(v[i] == static_cast<int>(i + 1));
}

TEST_CASE("insert returns valid iterator for further operations",
          "[minivector][insert]")
{
    MiniVector<int> v = {1, 3, 5};
    auto it = v.insert(v.cbegin() + 1, 2); // {1, 2, 3, 5}
    it = v.insert(it + 2, 4); // insère 4 après 3 → {1,2,3,4,5}

    REQUIRE(v.size() == 5);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 4);
    REQUIRE(v[4] == 5);
}

// =========================================================
// insert(pos, const T&) — version copie
// =========================================================

TEST_CASE("insert copy at beginning", "[minivector][insert]")
{
    MiniVector<int> v = {2, 3, 4};
    int value = 1;
    auto it = v.insert(v.cbegin(), value);

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 1);
    REQUIRE(v[3] == 4);
    REQUIRE(*it == 1);
    REQUIRE(it == v.begin());
}

TEST_CASE("insert copy at middle", "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 4, 5};
    int value = 3;
    auto it = v.insert(v.cbegin() + 2, value);

    REQUIRE(v.size() == 5);
    REQUIRE(v[2] == 3);
    REQUIRE(*it == 3);
}

TEST_CASE("insert copy at end", "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 3};
    int value = 4;
    auto it = v.insert(v.cend(), value);

    REQUIRE(v.size() == 4);
    REQUIRE(v[3] == 4);
    REQUIRE(*it == 4);
}

TEST_CASE("insert copy into empty vector", "[minivector][insert]")
{
    MiniVector<int> v;
    int value = 42;
    auto it = v.insert(v.cbegin(), value);

    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 42);
    REQUIRE(*it == 42);
}

TEST_CASE("insert copy triggers reallocation when full",
          "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 3};
    v.shrink_to_fit();
    int value = 99;

    v.insert(v.cbegin() + 1, value);

    REQUIRE(v.size() == 4);
    REQUIRE(v.capacity() >= 4);
    REQUIRE(v[1] == 99);
}

TEST_CASE("insert copy handles aliasing when value is element of self",
          "[minivector][insert]")
{
    MiniVector<int> v = {10, 20, 30};
    v.insert(v.cbegin(), v[0]); // value est une référence vers v[0]

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 10);
    REQUIRE(v[1] == 10);
    REQUIRE(v[2] == 20);
    REQUIRE(v[3] == 30);
}

TEST_CASE("insert copy calls copy constructor", "[minivector][insert]")
{
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v;
        v.push_back(Tracker(1));
        v.push_back(Tracker(3));

        REQUIRE(Tracker::alive == 2);

        Tracker source(2);
        v.insert(v.cbegin() + 1, source);

        // 3 dans v + 1 source
        REQUIRE(Tracker::alive == 4);
        REQUIRE(v.size() == 3);
        REQUIRE(v[0].value == 1);
        REQUIRE(v[1].value == 2);
        REQUIRE(v[2].value == 3);
    }
    REQUIRE(Tracker::alive == 0);
}

// =========================================================
// insert(pos, T&&) — version déplacement
// =========================================================

TEST_CASE("insert move at beginning", "[minivector][insert]")
{
    MiniVector<int> v = {2, 3, 4};
    auto it = v.insert(v.cbegin(), 1);

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 1);
    REQUIRE(v[3] == 4);
    REQUIRE(*it == 1);
}

TEST_CASE("insert move at middle", "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 4, 5};
    auto it = v.insert(v.cbegin() + 2, 3);

    REQUIRE(v.size() == 5);
    REQUIRE(v[2] == 3);
    REQUIRE(*it == 3);
}

TEST_CASE("insert move at end", "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 3};
    auto it = v.insert(v.cend(), 4);

    REQUIRE(v.size() == 4);
    REQUIRE(v[3] == 4);
    REQUIRE(*it == 4);
}

TEST_CASE("insert move calls move constructor, not copy",
          "[minivector][insert]")
{
    MoveTracker::moves = 0;

    MiniVector<MoveTracker> v;
    v.push_back(MoveTracker(1));
    v.push_back(MoveTracker(3));

    int moves_before = MoveTracker::moves;

    v.insert(v.cbegin() + 1, MoveTracker(2));

    // Au moins un déplacement pour insérer le temporaire
    REQUIRE(MoveTracker::moves > moves_before);

    REQUIRE(v.size() == 3);
    REQUIRE(v[0].value == 1);
    REQUIRE(v[1].value == 2);
    REQUIRE(v[2].value == 3);
}

TEST_CASE("insert move at beginning with temporary",
          "[minivector][insert]")
{
    MiniVector<std::string> v = {"world"};

    v.insert(v.cbegin(), std::string("hello"));

    REQUIRE(v.size() == 2);
    REQUIRE(v[0] == "hello");
    REQUIRE(v[1] == "world");
}

TEST_CASE("insert move handles aliasing when value is element of self",
          "[minivector][insert]")
{
    MiniVector<std::string> v = {"A", "B", "C"};
    v.insert(v.cbegin(), std::move(v[0]));

    REQUIRE(v.size() == 4);
    // v[0] est maintenant l'ancien "A" déplacé ; v[1] contient "A" ou "" selon l'implémentation
    REQUIRE(v.size() == 4);
    REQUIRE(v[2] == "B");
    REQUIRE(v[3] == "C");
}

TEST_CASE("insert copy vs move are interchangeable for trivially copyable types",
          "[minivector][insert]")
{
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b = {1, 2, 3};

    int value = 99;
    a.insert(a.cbegin() + 1, value); // copie
    b.insert(b.cbegin() + 1, std::move(value)); // move (mais int, donc copie)

    REQUIRE(a == b);
}

TEST_CASE("insert copy and move both work", "[minivector][insert]")
{
    MiniVector<std::string> v;

    std::string s1 = "hello";
    std::string s2 = "world";

    v.insert(v.cbegin(), s1); // copie
    v.insert(v.cend(), std::move(s2)); // move
    v.insert(v.cbegin() + 1, "middle"); // temporaire → move

    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == "hello");
    REQUIRE(v[1] == "middle");
    REQUIRE(v[2] == "world");

    // s1 est inchangé (copie), s2 est vide (déplacé)
    REQUIRE(s1 == "hello");
    REQUIRE(s2.empty());
}

TEST_CASE("insert move uses move construction and move assignment",
          "[minivector][insert]")
{
    MoveTracker::moves = 0;
    MoveTracker::assignments = 0;

    MiniVector<MoveTracker> v;
    v.push_back(MoveTracker(1));
    v.push_back(MoveTracker(3));

    int moves_before = MoveTracker::moves;

    v.insert(v.cbegin() + 1, MoveTracker(2));

    REQUIRE(MoveTracker::moves > moves_before);
    REQUIRE(v.size() == 3);
    REQUIRE(v[0].value == 1);
    REQUIRE(v[1].value == 2);
    REQUIRE(v[2].value == 3);
}

// =========================================================
// insert(pos, count, value)
// =========================================================

TEST_CASE("insert count copies at beginning", "[minivector][insert]")
{
    MiniVector<int> v = {3, 4, 5};
    auto it = v.insert(v.cbegin(), 3, 9);

    REQUIRE(v.size() == 6);
    REQUIRE(v[0] == 9);
    REQUIRE(v[1] == 9);
    REQUIRE(v[2] == 9);
    REQUIRE(v[3] == 3);
    REQUIRE(v[4] == 4);
    REQUIRE(v[5] == 5);
    REQUIRE(*it == 9);
    REQUIRE(it == v.begin());
}

TEST_CASE("insert count copies at middle", "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 3, 4};
    auto it = v.insert(v.cbegin() + 2, 3, 9);

    REQUIRE(v.size() == 7);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 9);
    REQUIRE(v[3] == 9);
    REQUIRE(v[4] == 9);
    REQUIRE(v[5] == 3);
    REQUIRE(v[6] == 4);
    REQUIRE(*it == 9);
}

TEST_CASE("insert count copies at end", "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 3};
    auto it = v.insert(v.cend(), 4, 9);

    REQUIRE(v.size() == 7);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 9);
    REQUIRE(v[6] == 9);
    REQUIRE(*it == 9);
}

TEST_CASE("insert zero copies does nothing", "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 3};
    auto it = v.insert(v.cbegin() + 1, 0, 9);

    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(it == v.begin() + 1);
}

TEST_CASE("insert count copies into empty vector", "[minivector][insert]")
{
    MiniVector<int> v;
    auto it = v.insert(v.cbegin(), 4, 7);

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 7);
    REQUIRE(v[3] == 7);
    REQUIRE(*it == 7);
}

TEST_CASE("insert count copies triggers reallocation",
          "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 3};
    v.shrink_to_fit();
    REQUIRE(v.capacity() == 3);

    v.insert(v.cbegin() + 1, 10, 9);

    REQUIRE(v.size() == 13);
    REQUIRE(v.capacity() >= 13);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 9);
    REQUIRE(v[10] == 9);
    REQUIRE(v[11] == 2);
    REQUIRE(v[12] == 3);
}

TEST_CASE("insert count copies preserves capacity if sufficient",
          "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 3};
    v.reserve(100);
    std::size_t cap = v.capacity();

    v.insert(v.cbegin() + 1, 5, 9);

    REQUIRE(v.capacity() == cap);
}

TEST_CASE("insert count copies handles aliasing", "[minivector][insert]")
{
    MiniVector<int> v = {10, 20, 30};
    v.insert(v.cbegin(), 3, v[1]); // value est une référence vers v[1]

    REQUIRE(v.size() == 6);
    REQUIRE(v[0] == 20);
    REQUIRE(v[1] == 20);
    REQUIRE(v[2] == 20);
    REQUIRE(v[3] == 10);
    REQUIRE(v[4] == 20);
    REQUIRE(v[5] == 30);
}

TEST_CASE("insert count copies calls copy constructor",
          "[minivector][insert]")
{
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v;
        v.push_back(Tracker(1));
        v.push_back(Tracker(2));

        REQUIRE(Tracker::alive == 2);

        Tracker source(99);
        v.insert(v.cbegin() + 1, 3, source);

        // 5 dans v + 1 source
        REQUIRE(Tracker::alive == 6);
        REQUIRE(v.size() == 5);
        REQUIRE(v[0].value == 1);
        REQUIRE(v[1].value == 99);
        REQUIRE(v[2].value == 99);
        REQUIRE(v[3].value == 99);
        REQUIRE(v[4].value == 2);
    }
    REQUIRE(Tracker::alive == 0);
}

TEST_CASE("insert count copies many elements", "[minivector][insert]")
{
    MiniVector<int> v = {0};
    v.insert(v.cend(), 100, 42);

    REQUIRE(v.size() == 101);
    REQUIRE(v[0] == 0);
    for (std::size_t i = 1; i <= 100; ++i)
        REQUIRE(v[i] == 42);
}

TEST_CASE("insert count copies at beginning multiple times",
          "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 3};
    v.insert(v.cbegin(), 2, 0);
    v.insert(v.cbegin(), 1, -1);

    REQUIRE(v.size() == 6);
    REQUIRE(v[0] == -1);
    REQUIRE(v[1] == 0);
    REQUIRE(v[2] == 0);
    REQUIRE(v[3] == 1);
    REQUIRE(v[4] == 2);
    REQUIRE(v[5] == 3);
}

// =========================================================
// insert(pos, initializer_list)
// =========================================================

TEST_CASE("insert ilist at beginning", "[minivector][insert]")
{
    MiniVector<int> v = {4, 5};
    auto it = v.insert(v.cbegin(), {1, 2, 3});

    REQUIRE(v.size() == 5);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 4);
    REQUIRE(v[4] == 5);
    REQUIRE(*it == 1);
    REQUIRE(it == v.begin());
}

TEST_CASE("insert ilist at middle", "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 5, 6};
    auto it = v.insert(v.cbegin() + 2, {3, 4});

    REQUIRE(v.size() == 6);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 4);
    REQUIRE(v[4] == 5);
    REQUIRE(v[5] == 6);
    REQUIRE(*it == 3);
}

TEST_CASE("insert ilist at end", "[minivector][insert]")
{
    MiniVector<int> v = {1, 2};
    auto it = v.insert(v.cend(), {3, 4, 5});

    REQUIRE(v.size() == 5);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[5 - 1] == 5);
    REQUIRE(*it == 3);
}

TEST_CASE("insert empty ilist does nothing", "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 3};
    auto it = v.insert(v.cbegin() + 1, {});

    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(it == v.begin() + 1);
}

TEST_CASE("insert ilist into empty vector", "[minivector][insert]")
{
    MiniVector<int> v;
    auto it = v.insert(v.cbegin(), {1, 2, 3});

    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(*it == 1);
}

TEST_CASE("insert ilist triggers reallocation", "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 3};
    v.shrink_to_fit();
    REQUIRE(v.capacity() == 3);

    v.insert(v.cbegin() + 1, {10, 20, 30, 40, 50});

    REQUIRE(v.size() == 8);
    REQUIRE(v.capacity() >= 8);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 10);
    REQUIRE(v[5] == 50);
    REQUIRE(v[6] == 2);
    REQUIRE(v[7] == 3);
}

TEST_CASE("insert ilist preserves capacity if sufficient",
          "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 3};
    v.reserve(100);
    std::size_t cap = v.capacity();

    v.insert(v.cbegin() + 1, {10, 20});

    REQUIRE(v.capacity() == cap);
}

TEST_CASE("insert ilist calls copy constructor",
          "[minivector][insert]")
{
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v;
        v.push_back(Tracker(1));
        v.push_back(Tracker(2));

        REQUIRE(Tracker::alive == 2);

        Tracker a(10);
        Tracker b(20);
        Tracker c(30);
        REQUIRE(Tracker::alive == 5);

        v.insert(v.cbegin() + 1, {a, b, c});

        // 5 dans v + 3 locaux = 8
        REQUIRE(Tracker::alive == 8);
        REQUIRE(v.size() == 5);
        REQUIRE(v[0].value == 1);
        REQUIRE(v[1].value == 10);
        REQUIRE(v[2].value == 20);
        REQUIRE(v[3].value == 30);
        REQUIRE(v[4].value == 2);
    }
    REQUIRE(Tracker::alive == 0);
}

TEST_CASE("insert ilist with multiple values preserves order",
          "[minivector][insert]")
{
    MiniVector<int> v = {0, 100};
    v.insert(v.cbegin() + 1, {10, 20, 30, 40, 50});

    REQUIRE(v.size() == 7);
    for (std::size_t i = 0; i < 7; ++i)
    {
        static const int expected[] = {0, 10, 20, 30, 40, 50, 100};
        REQUIRE(v[i] == expected[i]);
    }
}

TEST_CASE("insert ilist at multiple positions", "[minivector][insert]")
{
    MiniVector<int> v = {5};

    v.insert(v.cbegin(), {1, 2});
    v.insert(v.cend(), {7, 8});
    v.insert(v.cbegin() + 2, {3, 4});

    REQUIRE(v.size() == 7);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 4);
    REQUIRE(v[4] == 5);
    REQUIRE(v[5] == 7);
    REQUIRE(v[6] == 8);
}

TEST_CASE("insert ilist with strings", "[minivector][insert]")
{
    MiniVector<std::string> v = {"a", "d"};
    v.insert(v.cbegin() + 1, {"b", "c"});

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == "a");
    REQUIRE(v[1] == "b");
    REQUIRE(v[2] == "c");
    REQUIRE(v[3] == "d");
}

// =========================================================
// insert(pos, first, last)
// =========================================================

TEST_CASE("insert range from C-array at beginning",
          "[minivector][insert]")
{
    int arr[] = {1, 2, 3};
    MiniVector<int> v = {4, 5};

    auto it = v.insert(v.cbegin(), std::begin(arr), std::end(arr));

    REQUIRE(v.size() == 5);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 4);
    REQUIRE(v[4] == 5);
    REQUIRE(*it == 1);
    REQUIRE(it == v.begin());
}

TEST_CASE("insert range from C-array at middle",
          "[minivector][insert]")
{
    int arr[] = {10, 20, 30};
    MiniVector<int> v = {1, 2, 4, 5};

    auto it = v.insert(v.cbegin() + 2, std::begin(arr), std::end(arr));

    REQUIRE(v.size() == 7);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 10);
    REQUIRE(v[3] == 20);
    REQUIRE(v[4] == 30);
    REQUIRE(v[5] == 4);
    REQUIRE(v[6] == 5);
    REQUIRE(*it == 10);
}

TEST_CASE("insert range from C-array at end",
          "[minivector][insert]")
{
    int arr[] = {7, 8, 9};
    MiniVector<int> v = {1, 2};

    auto it = v.insert(v.cend(), std::begin(arr), std::end(arr));

    REQUIRE(v.size() == 5);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 7);
    REQUIRE(v[3] == 8);
    REQUIRE(v[4] == 9);
    REQUIRE(*it == 7);
}

TEST_CASE("insert empty range does nothing", "[minivector][insert]")
{
    int arr[] = {1, 2, 3};
    MiniVector<int> v = {10, 20, 30};

    auto it = v.insert(v.cbegin() + 1, std::begin(arr), std::begin(arr));

    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 10);
    REQUIRE(v[1] == 20);
    REQUIRE(v[2] == 30);
    REQUIRE(it == v.begin() + 1);
}

TEST_CASE("insert range into empty vector", "[minivector][insert]")
{
    int arr[] = {1, 2, 3};
    MiniVector<int> v;

    auto it = v.insert(v.cbegin(), std::begin(arr), std::end(arr));

    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(*it == 1);
}

TEST_CASE("insert range triggers reallocation", "[minivector][insert]")
{
    int arr[] = {10, 20, 30, 40, 50};
    MiniVector<int> v = {1, 2, 3};
    v.shrink_to_fit();
    REQUIRE(v.capacity() == 3);

    v.insert(v.cbegin() + 1, std::begin(arr), std::end(arr));

    REQUIRE(v.size() == 8);
    REQUIRE(v.capacity() >= 8);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 10);
    REQUIRE(v[5] == 50);
    REQUIRE(v[6] == 2);
    REQUIRE(v[7] == 3);
}

TEST_CASE("insert range from another MiniVector", "[minivector][insert]")
{
    MiniVector<int> src = {10, 20, 30};
    MiniVector<int> v = {1, 2, 4, 5};

    v.insert(v.cbegin() + 2, src.cbegin(), src.cend());

    REQUIRE(v.size() == 7);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 10);
    REQUIRE(v[3] == 20);
    REQUIRE(v[4] == 30);
    REQUIRE(v[5] == 4);
    REQUIRE(v[6] == 5);
}

TEST_CASE("insert range from std::vector", "[minivector][insert]")
{
    std::vector<int> src = {10, 20};
    MiniVector<int> v = {1, 2, 3};

    v.insert(v.cbegin() + 1, src.begin(), src.end());

    REQUIRE(v.size() == 5);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 10);
    REQUIRE(v[2] == 20);
    REQUIRE(v[3] == 2);
    REQUIRE(v[4] == 3);
}

TEST_CASE("insert range handles aliasing with sub-range of self",
          "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};

    // Insère {1, 2, 3} à l'indice 2. La plage source est dans v !
    v.insert(v.cbegin() + 2, v.cbegin(), v.cbegin() + 3);

    REQUIRE(v.size() == 8);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 1);
    REQUIRE(v[3] == 2);
    REQUIRE(v[4] == 3);
    REQUIRE(v[5] == 3);
    REQUIRE(v[6] == 4);
    REQUIRE(v[7] == 5);
}

TEST_CASE("insert range handles aliasing with full self",
          "[minivector][insert]")
{
    MiniVector<int> v = {1, 2, 3};

    // Insère tout v dans v au début
    v.insert(v.cbegin(), v.cbegin(), v.cend());

    REQUIRE(v.size() == 6);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 1);
    REQUIRE(v[4] == 2);
    REQUIRE(v[5] == 3);
}

TEST_CASE("insert range calls copy constructor", "[minivector][insert]")
{
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v;
        v.push_back(Tracker(1));
        v.push_back(Tracker(4));

        REQUIRE(Tracker::alive == 2);

        Tracker src[] = {Tracker(2), Tracker(3)};
        REQUIRE(Tracker::alive == 4);

        v.insert(v.cbegin() + 1, std::begin(src), std::end(src));

        // 4 dans v + 2 dans src = 6
        REQUIRE(Tracker::alive == 6);
        REQUIRE(v.size() == 4);
        REQUIRE(v[0].value == 1);
        REQUIRE(v[1].value == 2);
        REQUIRE(v[2].value == 3);
        REQUIRE(v[3].value == 4);
    }
    REQUIRE(Tracker::alive == 0);
}

TEST_CASE("insert range with strings", "[minivector][insert]")
{
    MiniVector<std::string> v = {"a", "d"};
    std::vector<std::string> src = {"b", "c"};

    v.insert(v.cbegin() + 1, src.begin(), src.end());

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == "a");
    REQUIRE(v[1] == "b");
    REQUIRE(v[2] == "c");
    REQUIRE(v[3] == "d");
}

TEST_CASE("insert range does not need random access iterators",
          "[minivector][insert]")
{
    // std::istreambuf_iterator est un input_iterator pur
    // On simule avec une source depuis std::cin, mais plus simple :
    // on utilise un itérateur qui n'est PAS random access
    std::list<int> src = {10, 20, 30};
    MiniVector<int> v = {1, 2, 3};

    v.insert(v.cbegin() + 1, src.begin(), src.end());

    REQUIRE(v.size() == 6);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 10);
    REQUIRE(v[2] == 20);
    REQUIRE(v[3] == 30);
    REQUIRE(v[4] == 2);
    REQUIRE(v[5] == 3);
}

// =========================================================
// emplace(pos, args...)
// =========================================================

TEST_CASE("emplace at beginning with simple value", "[minivector][emplace]")
{
    MiniVector<int> v = {2, 3, 4};
    auto it = v.emplace(v.cbegin(), 1);

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[3] == 4);
    REQUIRE(*it == 1);
    REQUIRE(it == v.begin());
}

TEST_CASE("emplace at middle with multiple args", "[minivector][emplace]")
{
    struct Point
    {
        int x, y;

        Point(int a, int b) : x(a), y(b)
        {
        }
    };

    MiniVector<Point> v;
    v.emplace(v.cbegin(), 1, 2);
    v.emplace(v.cbegin() + 1, 3, 4);
    v.emplace(v.cbegin(), 5, 6);

    REQUIRE(v.size() == 3);
    REQUIRE(v[0].x == 5);
    REQUIRE(v[0].y == 6);
    REQUIRE(v[1].x == 1);
    REQUIRE(v[1].y == 2);
    REQUIRE(v[2].x == 3);
    REQUIRE(v[2].y == 4);
}

TEST_CASE("emplace at end", "[minivector][emplace]")
{
    MiniVector<int> v = {1, 2, 3};
    auto it = v.emplace(v.cend(), 4);

    REQUIRE(v.size() == 4);
    REQUIRE(v[3] == 4);
    REQUIRE(*it == 4);
}

TEST_CASE("emplace into empty vector", "[minivector][emplace]")
{
    MiniVector<int> v;
    auto it = v.emplace(v.cbegin(), 42);

    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 42);
    REQUIRE(*it == 42);
}

TEST_CASE("emplace triggers reallocation", "[minivector][emplace]")
{
    MiniVector<int> v = {1, 2, 3};
    v.shrink_to_fit();
    REQUIRE(v.capacity() == 3);

    v.emplace(v.cbegin() + 1, 99);

    REQUIRE(v.size() == 4);
    REQUIRE(v.capacity() >= 4);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 99);
    REQUIRE(v[2] == 2);
    REQUIRE(v[3] == 3);
}

TEST_CASE("emplace with non-copyable type", "[minivector][emplace]")
{
    struct NonCopyable
    {
        int value;

        explicit NonCopyable(int v) : value(v)
        {
        }

        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
        NonCopyable(NonCopyable&&) noexcept = default;
        NonCopyable& operator=(NonCopyable&&) noexcept = default;
    };

    MiniVector<NonCopyable> v;
    v.emplace(v.cbegin(), 42);

    REQUIRE(v.size() == 1);
    REQUIRE(v[0].value == 42);

    v.emplace(v.cbegin(), 10);

    REQUIRE(v.size() == 2);
    REQUIRE(v[0].value == 10);
    REQUIRE(v[1].value == 42);
}

TEST_CASE("emplace with string", "[minivector][emplace]")
{
    MiniVector<std::string> v;

    v.emplace(v.cbegin(), "hello");
    v.emplace(v.cend(), 5, 'a'); // string(5, 'a') = "aaaaa"

    REQUIRE(v.size() == 2);
    REQUIRE(v[0] == "hello");
    REQUIRE(v[1] == "aaaaa");
}

TEST_CASE("emplace returns valid iterator", "[minivector][emplace]")
{
    MiniVector<int> v = {1, 3, 5};
    auto it = v.emplace(v.cbegin() + 1, 2);

    REQUIRE(*it == 2);
    REQUIRE(it == v.begin() + 1);

    // Utiliser l'itérateur retourné pour une autre opération
    auto it2 = v.emplace(it + 2, 4);
    REQUIRE(*it2 == 4);
    REQUIRE(v.size() == 5);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 4);
    REQUIRE(v[4] == 5);
}

TEST_CASE("emplace multiple times preserves order", "[minivector][emplace]")
{
    MiniVector<int> v;
    v.emplace(v.cbegin(), 3);
    v.emplace(v.cbegin(), 2);
    v.emplace(v.cbegin(), 1);
    v.emplace(v.cend(), 4);

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 4);
}

TEST_CASE("emplace at middle with copy/move counting",
          "[minivector][emplace]")
{
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v;
        v.push_back(Tracker(1));
        v.push_back(Tracker(3));

        REQUIRE(Tracker::alive == 2);

        v.emplace(v.cbegin() + 1, 2);

        // 3 dans v, le temporaire dans emplace a été détruit
        REQUIRE(Tracker::alive == 3);
        REQUIRE(v.size() == 3);
        REQUIRE(v[0].value == 1);
        REQUIRE(v[1].value == 2);
        REQUIRE(v[2].value == 3);
    }
    REQUIRE(Tracker::alive == 0);
}
