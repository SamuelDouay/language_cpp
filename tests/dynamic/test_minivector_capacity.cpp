#include "MiniVector.hpp"
#include "MoveTracker.hpp"
#include "ThrowingCopy.hpp"
#include "Tracker.hpp"
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

// =========================================================
// resize(count) — version par défaut
// =========================================================

TEST_CASE("resize to larger size adds default-constructed elements",
          "[minivector][resize]")
{
    MiniVector<int> v = {1, 2, 3};
    v.resize(5);

    REQUIRE(v.size() == 5);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 0); // default-constructed int
    REQUIRE(v[4] == 0);
}

TEST_CASE("resize to smaller size removes trailing elements",
          "[minivector][resize]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};
    v.resize(3);

    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
}

TEST_CASE("resize to same size does nothing",
          "[minivector][resize]")
{
    MiniVector<int> v = {1, 2, 3};
    v.resize(3);

    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
}

TEST_CASE("resize to zero clears the vector",
          "[minivector][resize]")
{
    MiniVector<int> v = {1, 2, 3};
    v.resize(0);

    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);
}

TEST_CASE("resize on empty vector grows correctly",
          "[minivector][resize]")
{
    MiniVector<int> v;
    v.resize(4);

    REQUIRE(v.size() == 4);
    for (std::size_t i = 0; i < v.size(); ++i)
        REQUIRE(v[i] == 0);
}

TEST_CASE("resize does not shrink capacity",
          "[minivector][resize]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};
    std::size_t old_cap = v.capacity();
    v.resize(2);

    REQUIRE(v.size() == 2);
    REQUIRE(v.capacity() == old_cap);
}

TEST_CASE("resize calls destructors on removed elements",
          "[minivector][resize]")
{
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v;
        v.push_back(Tracker(1));
        v.push_back(Tracker(2));
        v.push_back(Tracker(3));
        v.push_back(Tracker(4));

        REQUIRE(Tracker::alive == 4);

        v.resize(2);

        REQUIRE(Tracker::alive == 2); // deux éléments détruits
        REQUIRE(v.size() == 2);
    }
    REQUIRE(Tracker::alive == 0);
}

TEST_CASE("resize calls default constructor on added elements",
          "[minivector][resize]")
{
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v;
        v.push_back(Tracker(1));
        v.push_back(Tracker(2));

        REQUIRE(Tracker::alive == 2);

        v.resize(5);

        REQUIRE(Tracker::alive == 5); // trois éléments construits
        REQUIRE(v.size() == 5);
    }
    REQUIRE(Tracker::alive == 0);
}

// =========================================================
// resize(count, value) — version avec valeur
// =========================================================

TEST_CASE("resize with value fills new elements with that value",
          "[minivector][resize]")
{
    MiniVector<int> v = {1, 2, 3};
    v.resize(6, 42);

    REQUIRE(v.size() == 6);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 42);
    REQUIRE(v[4] == 42);
    REQUIRE(v[5] == 42);
}

TEST_CASE("resize with value shrinks correctly",
          "[minivector][resize]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};
    v.resize(2, 42);

    REQUIRE(v.size() == 2);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
}

TEST_CASE("resize with value on empty vector",
          "[minivector][resize]")
{
    MiniVector<int> v;
    v.resize(3, 7);

    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == 7);
    REQUIRE(v[2] == 7);
}

TEST_CASE("resize with value does not shrink capacity",
          "[minivector][resize]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};
    std::size_t old_cap = v.capacity();
    v.resize(2, 99);

    REQUIRE(v.capacity() == old_cap);
}

TEST_CASE("resize with value calls destructors on removed elements",
          "[minivector][resize]")
{
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v;
        v.push_back(Tracker(1));
        v.push_back(Tracker(2));
        v.push_back(Tracker(3));
        v.push_back(Tracker(4));

        REQUIRE(Tracker::alive == 4);

        Tracker filler(99);
        v.resize(2, filler);

        REQUIRE(v.size() == 2);
        // 2 trackers dans v + 1 filler local
        REQUIRE(Tracker::alive == 3);
    }
    REQUIRE(Tracker::alive == 0);
}

TEST_CASE("resize with value calls copy constructor on added elements",
          "[minivector][resize]")
{
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v;
        v.push_back(Tracker(1));

        Tracker filler(42);
        // 1 dans v + 1 filler
        REQUIRE(Tracker::alive == 2);

        v.resize(4, filler);

        // 4 dans v + 1 filler
        REQUIRE(Tracker::alive == 5);
        REQUIRE(v.size() == 4);
        REQUIRE(v[0].value == 1);
        REQUIRE(v[1].value == 42);
        REQUIRE(v[2].value == 42);
        REQUIRE(v[3].value == 42);
    }
    REQUIRE(Tracker::alive == 0);
}

// =========================================================
// resize — cas limites et stabilité
// =========================================================

TEST_CASE("resize repeatedly is idempotent",
          "[minivector][resize]")
{
    MiniVector<int> v = {1, 2, 3};

    v.resize(5);
    v.resize(5);
    v.resize(5);

    REQUIRE(v.size() == 5);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 0);
    REQUIRE(v[4] == 0);
}

TEST_CASE("resize grow then shrink then grow works",
          "[minivector][resize]")
{
    MiniVector<int> v;

    v.resize(5, 7);
    REQUIRE(v.size() == 5);

    v.resize(2);
    REQUIRE(v.size() == 2);

    v.resize(4, 9);
    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == 7);
    REQUIRE(v[2] == 9);
    REQUIRE(v[3] == 9);
}

TEST_CASE("resize large does not overflow",
          "[minivector][resize]")
{
    MiniVector<int> v;
    v.resize(1000, 1);

    REQUIRE(v.size() == 1000);
    for (std::size_t i = 0; i < v.size(); ++i)
        REQUIRE(v[i] == 1);
}

// =========================================================
// assign(count, value)
// =========================================================

TEST_CASE("assign(count, value) on empty vector", "[minivector][assign]")
{
    MiniVector<int> v;
    v.assign(3, 42);

    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 42);
    REQUIRE(v[1] == 42);
    REQUIRE(v[2] == 42);
}

TEST_CASE("assign(count, value) replaces existing content", "[minivector][assign]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};
    v.assign(2, 7);

    REQUIRE(v.size() == 2);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == 7);
}

TEST_CASE("assign(count, value) with count greater than capacity",
          "[minivector][assign]")
{
    MiniVector<int> v = {1, 2, 3};
    v.assign(100, 9);

    REQUIRE(v.size() == 100);
    for (std::size_t i = 0; i < v.size(); ++i)
        REQUIRE(v[i] == 9);
}

TEST_CASE("assign(count, value) with count zero clears the vector",
          "[minivector][assign]")
{
    MiniVector<int> v = {1, 2, 3};
    v.assign(0, 42);

    REQUIRE(v.empty());
}

TEST_CASE("assign(count, value) handles aliasing when value is an element of self",
          "[minivector][assign]")
{
    MiniVector<int> v = {10, 20, 30};
    v.assign(4, v[1]); // value référence v[1] == 20

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 20);
    REQUIRE(v[1] == 20);
    REQUIRE(v[2] == 20);
    REQUIRE(v[3] == 20);
}

TEST_CASE("assign(count, value) destroys old elements",
          "[minivector][assign]")
{
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v;
        v.push_back(Tracker(1));
        v.push_back(Tracker(2));
        v.push_back(Tracker(3));

        REQUIRE(Tracker::alive == 3);

        Tracker filler(99);
        v.assign(2, filler);

        // 2 dans v + 1 filler
        REQUIRE(Tracker::alive == 3);
        REQUIRE(v.size() == 2);
        REQUIRE(v[0].value == 99);
        REQUIRE(v[1].value == 99);
    }
    REQUIRE(Tracker::alive == 0);
}

// =========================================================
// assign(initializer_list)
// =========================================================

TEST_CASE("assign(ilist) on empty vector", "[minivector][assign]")
{
    MiniVector<int> v;
    v.assign({1, 2, 3});

    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
}

TEST_CASE("assign(ilist) replaces existing content", "[minivector][assign]")
{
    MiniVector<int> v = {10, 20, 30, 40, 50};
    v.assign({5, 6});

    REQUIRE(v.size() == 2);
    REQUIRE(v[0] == 5);
    REQUIRE(v[1] == 6);
}

TEST_CASE("assign(ilist) with empty list clears the vector",
          "[minivector][assign]")
{
    MiniVector<int> v = {1, 2, 3};
    v.assign({});

    REQUIRE(v.empty());
}

TEST_CASE("assign(ilist) with size greater than capacity",
          "[minivector][assign]")
{
    MiniVector<int> v;
    v.assign({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    REQUIRE(v.size() == 10);
    for (std::size_t i = 0; i < 10; ++i)
        REQUIRE(v[i] == static_cast<int>(i + 1));
}

TEST_CASE("assign(ilist) destroys old elements",
          "[minivector][assign]")
{
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v;
        v.push_back(Tracker(1));
        v.push_back(Tracker(2));
        v.push_back(Tracker(3));

        REQUIRE(Tracker::alive == 3);

        Tracker a(10);
        Tracker b(20);
        v.assign({a, b});

        // 2 dans v + 2 trackers locaux
        REQUIRE(Tracker::alive == 4);
        REQUIRE(v.size() == 2);
        REQUIRE(v[0].value == 10);
        REQUIRE(v[1].value == 20);
    }
    REQUIRE(Tracker::alive == 0);
}

// =========================================================
// assign(first, last)
// =========================================================

TEST_CASE("assign(range) from C-array", "[minivector][assign]")
{
    int arr[] = {1, 2, 3, 4};
    MiniVector<int> v = {99, 99};
    v.assign(std::begin(arr), std::end(arr));

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 4);
}

TEST_CASE("assign(range) from another MiniVector", "[minivector][assign]")
{
    MiniVector<int> source = {10, 20, 30};
    MiniVector<int> v = {1, 2, 3, 4, 5};
    v.assign(source.begin(), source.end());

    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 10);
    REQUIRE(v[1] == 20);
    REQUIRE(v[2] == 30);
}

TEST_CASE("assign(range) from a sub-range of self handles aliasing",
          "[minivector][assign]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};
    // On veut copier les éléments {2, 3, 4} dans v
    v.assign(v.begin() + 1, v.begin() + 4);

    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 2);
    REQUIRE(v[1] == 3);
    REQUIRE(v[2] == 4);
}

TEST_CASE("assign(range) with empty range clears the vector",
          "[minivector][assign]")
{
    MiniVector<int> v = {1, 2, 3};
    v.assign(v.begin(), v.begin());

    REQUIRE(v.empty());
}

TEST_CASE("assign(range) from std::vector", "[minivector][assign]")
{
    std::vector<int> src = {7, 8, 9};
    MiniVector<int> v;
    v.assign(src.begin(), src.end());

    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == 8);
    REQUIRE(v[2] == 9);
}

TEST_CASE("assign(range) preserves strong guarantee on exception",
          "[minivector][assign]")
{
    ThrowingCopy::copy_count = 0;
    ThrowingCopy::throw_after = 100;

    MiniVector<ThrowingCopy> v;
    v.push_back(ThrowingCopy(1));
    v.push_back(ThrowingCopy(2));

    ThrowingCopy::copy_count = 0;
    ThrowingCopy::throw_after = 2;


    ThrowingCopy src[] = {ThrowingCopy(10), ThrowingCopy(20), ThrowingCopy(30), ThrowingCopy(40)};

    REQUIRE_THROWS_AS(
        v.assign(std::begin(src), std::end(src)),
        std::runtime_error
    );

    // Le vecteur original doit rester intact (garantie forte)
    REQUIRE(v.size() == 2);
    REQUIRE(v[0].value == 1);
    REQUIRE(v[1].value == 2);
}

TEST_CASE("assign(range) destroys old elements",
          "[minivector][assign]")
{
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v;
        v.push_back(Tracker(1));
        v.push_back(Tracker(2));
        v.push_back(Tracker(3));

        REQUIRE(Tracker::alive == 3);

        Tracker src[] = {Tracker(10), Tracker(20)};
        // 3 dans v + 2 dans src
        REQUIRE(Tracker::alive == 5);

        v.assign(std::begin(src), std::end(src));

        // 2 dans v + 2 dans src
        REQUIRE(Tracker::alive == 4);
        REQUIRE(v.size() == 2);
        REQUIRE(v[0].value == 10);
        REQUIRE(v[1].value == 20);
    }
    REQUIRE(Tracker::alive == 0);
}

// =========================================================
// shrink_to_fit
// =========================================================

TEST_CASE("shrink_to_fit reduces capacity to size", "[minivector][shrink]")
{
    MiniVector<int> v;
    v.reserve(100);
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    REQUIRE(v.capacity() >= 100);
    REQUIRE(v.size() == 3);

    v.shrink_to_fit();

    REQUIRE(v.capacity() == 3);
    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
}

TEST_CASE("shrink_to_fit on vector with capacity == size does nothing",
          "[minivector][shrink]")
{
    MiniVector<int> v = {1, 2, 3};
    std::size_t cap_before = v.capacity();

    v.shrink_to_fit();

    REQUIRE(v.capacity() == cap_before);
    REQUIRE(v.size() == 3);
}

TEST_CASE("shrink_to_fit on empty vector with no capacity",
          "[minivector][shrink]")
{
    MiniVector<int> v;
    REQUIRE(v.capacity() == 0);
    REQUIRE(v.data() == nullptr);

    v.shrink_to_fit();

    REQUIRE(v.capacity() == 0);
    REQUIRE(v.size() == 0);
    REQUIRE(v.data() == nullptr);
}

TEST_CASE("shrink_to_fit on empty vector with capacity releases memory",
          "[minivector][shrink]")
{
    MiniVector<int> v;
    v.reserve(100);
    REQUIRE(v.capacity() == 100);
    REQUIRE(v.data() != nullptr);

    v.shrink_to_fit();

    REQUIRE(v.capacity() == 0);
    REQUIRE(v.size() == 0);
    REQUIRE(v.data() == nullptr);
    REQUIRE(v.empty());
}

TEST_CASE("shrink_to_fit preserves elements",
          "[minivector][shrink]")
{
    MiniVector<int> v = {10, 20, 30, 40, 50};
    v.reserve(200);

    v.shrink_to_fit();

    REQUIRE(v.size() == 5);
    REQUIRE(v[0] == 10);
    REQUIRE(v[1] == 20);
    REQUIRE(v[2] == 30);
    REQUIRE(v[3] == 40);
    REQUIRE(v[4] == 50);
}

TEST_CASE("shrink_to_fit calls move constructors, not copies",
          "[minivector][shrink]")
{
    MoveTracker::moves = 0;

    MiniVector<MoveTracker> v;
    v.reserve(100);
    v.push_back(MoveTracker(1));
    v.push_back(MoveTracker(2));
    v.push_back(MoveTracker(3));

    const int moves_before = MoveTracker::moves;

    v.shrink_to_fit();

    REQUIRE(MoveTracker::moves > moves_before);
    REQUIRE(v.size() == 3);
    REQUIRE(v[0].value == 1);
    REQUIRE(v[1].value == 2);
    REQUIRE(v[2].value == 3);
}

TEST_CASE("shrink_to_fit destroys old elements correctly",
          "[minivector][shrink]")
{
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v;
        v.reserve(100);
        v.push_back(Tracker(1));
        v.push_back(Tracker(2));
        v.push_back(Tracker(3));

        // 3 dans v + 3 temporaires détruits lors des push_back
        REQUIRE(Tracker::alive == 3);

        v.shrink_to_fit();

        // Toujours 3 objets vivants (pas de fuite ni double destruction)
        REQUIRE(Tracker::alive == 3);
        REQUIRE(v.size() == 3);
    }
    REQUIRE(Tracker::alive == 0);
}

TEST_CASE("shrink_to_fit can be called multiple times",
          "[minivector][shrink]")
{
    MiniVector<int> v = {1, 2, 3};
    v.reserve(200);

    v.shrink_to_fit();
    std::size_t cap_after_first = v.capacity();

    v.shrink_to_fit();

    REQUIRE(v.capacity() == cap_after_first);
    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 1);
    REQUIRE(v[2] == 3);
}

TEST_CASE("shrink_to_fit then push_back triggers reallocation",
          "[minivector][shrink]")
{
    MiniVector<int> v = {1, 2, 3};
    v.reserve(100);
    v.shrink_to_fit();

    REQUIRE(v.capacity() == 3);

    v.push_back(4);

    REQUIRE(v.size() == 4);
    REQUIRE(v.capacity() >= 4);
    REQUIRE(v[3] == 4);
}

TEST_CASE("shrink_to_fit on vector of one element",
          "[minivector][shrink]")
{
    MiniVector<int> v;
    v.reserve(50);
    v.push_back(42);

    v.shrink_to_fit();

    REQUIRE(v.size() == 1);
    REQUIRE(v.capacity() == 1);
    REQUIRE(v[0] == 42);
}
