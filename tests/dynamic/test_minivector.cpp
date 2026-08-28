#include <catch2/catch_test_macros.hpp>
#include <utility>
#include "MiniVector.hpp"

struct Tracker
{
    static int alive; // nombre d'objets vivants
    int value;
    Tracker(int v = 0) : value(v) { ++alive; }
    Tracker(const Tracker& other) : value(other.value) { ++alive; }
    Tracker(Tracker&& other) noexcept : value(other.value) { ++alive; }
    ~Tracker() { --alive; }
};

struct MoveTracker
{
    static int moves;

    int value;

    explicit MoveTracker(int v)
        : value(v)
    {
    }

    MoveTracker(const MoveTracker&) = delete;

    MoveTracker& operator=(const MoveTracker&) = delete;

    MoveTracker(MoveTracker&& other) noexcept
        : value(other.value)
    {
        ++moves;
    }

    MoveTracker& operator=(MoveTracker&&) = delete;
};

int MoveTracker::moves = 0;

int Tracker::alive = 0;

TEST_CASE("MiniVector default constructor", "[minivector]")
{
    MiniVector<int> v;
    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);
}

TEST_CASE("MiniVector constructor with count and value", "[minivector]")
{
    MiniVector<int> v(5, 42);
    REQUIRE_FALSE(v.empty());
    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);
    for (std::size_t i = 0; i < v.size(); ++i)
        REQUIRE(v[i] == 42);
}

TEST_CASE("MiniVector initializer_list constructor", "[minivector]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};
    REQUIRE(v.size() == 5);
    REQUIRE(v[0] == 1);
    REQUIRE(v[4] == 5);
}

TEST_CASE("push_back adds elements and grows capacity", "[minivector]")
{
    MiniVector<int> v;
    const int n = 100;
    for (int i = 0; i < n; ++i)
        v.push_back(i);

    REQUIRE(v.size() == static_cast<std::size_t>(n));
    REQUIRE(v.capacity() >= v.size());
    for (int i = 0; i < n; ++i)
        REQUIRE(v[static_cast<std::size_t>(i)] == i);
}

TEST_CASE("pop_back removes last element", "[minivector]")
{
    MiniVector<int> v = {1, 2, 3};
    v.pop_back();
    REQUIRE(v.size() == 2);
    REQUIRE(v[1] == 2);
}

TEST_CASE("clear empties vector but keeps capacity", "[minivector]")
{
    MiniVector<int> v = {1, 2, 3};
    std::size_t cap = v.capacity();
    v.clear();
    REQUIRE(v.empty());
    REQUIRE(v.capacity() == cap);
}

TEST_CASE("at throws out_of_range on invalid index", "[minivector]")
{
    MiniVector<int> v = {10, 20, 30};
    REQUIRE_THROWS_AS(v.at(3), std::out_of_range);
    REQUIRE_THROWS_AS(v.at(100), std::out_of_range);
    REQUIRE_NOTHROW(v.at(0));
}

TEST_CASE("copy constructor deep copies", "[minivector]")
{
    MiniVector<int> original = {1, 2, 3};
    MiniVector<int> copy(original);

    REQUIRE(copy.size() == original.size());
    copy[0] = 99;
    REQUIRE(original[0] == 1); // original unchanged
}

TEST_CASE("assignment operator deep copies", "[minivector]")
{
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b;
    b = a;
    REQUIRE(b.size() == a.size());
    b[0] = -1;
    REQUIRE(a[0] == 1);
}

TEST_CASE("move constructor leaves source empty", "[minivector]")
{
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b(std::move(a));
    REQUIRE(b.size() == 3);
    REQUIRE(a.size() == 0); // after move, source should be empty
    REQUIRE(a.capacity() == 0);
}

TEST_CASE("move assignment leaves source empty", "[minivector]")
{
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b;
    b = std::move(a);
    REQUIRE(b.size() == 3);
    REQUIRE(a.size() == 0);
    REQUIRE(a.capacity() == 0);
}

TEST_CASE("iterators work with range-based for", "[minivector]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};
    int sum = 0;
    for (int x : v)
        sum += x;
    REQUIRE(sum == 15);
}

TEST_CASE("pop_back destroys the removed element", "[minivector]")
{
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v;
        v.push_back(Tracker(1));
        REQUIRE(Tracker::alive == 1);
        v.pop_back();
        REQUIRE(Tracker::alive == 0); // l'élément est détruit
    }
    REQUIRE(Tracker::alive == 0); // pas de fuite après destruction du vecteur
}

TEST_CASE("clear destroys all elements but keeps capacity", "[minivector]")
{
    Tracker::alive = 0;
    MiniVector<Tracker> v;
    v.push_back(Tracker(1));
    v.push_back(Tracker(2));
    v.push_back(Tracker(3));
    std::size_t cap = v.capacity();
    REQUIRE(Tracker::alive == 3);
    v.clear();
    REQUIRE(Tracker::alive == 0); // tous les éléments sont détruits
    REQUIRE(v.capacity() == cap); // capacité inchangée
}

TEST_CASE("assignment operator releases old memory", "[minivector]")
{
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b = {4, 5, 6, 7};

    b = a;

    REQUIRE(b.size() == 3);
    REQUIRE(b[0] == 1);
}

TEST_CASE("move assignment releases old memory", "[minivector]")
{
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b = {4, 5, 6, 7};
    b = std::move(a);
    REQUIRE(b.size() == 3);
    REQUIRE(b[0] == 1);
    REQUIRE(a.size() == 0);
}

TEST_CASE("begin/end on empty vector return same pointer", "[minivector]")
{
    MiniVector<int> v;
    REQUIRE(v.begin() == v.end());
    REQUIRE(v.begin() == nullptr);
}

TEST_CASE("reserve does not shrink capacity", "[minivector]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};
    std::size_t old_cap = v.capacity();
    v.reserve(2);
    REQUIRE(v.capacity() == old_cap);
    v.reserve(old_cap + 1);
    REQUIRE(v.capacity() >= old_cap + 1);
}

TEST_CASE("push_back with rvalue moves element", "[minivector]")
{
    Tracker::alive = 0;
    MiniVector<Tracker> v;
    v.push_back(Tracker(42));
    REQUIRE(Tracker::alive == 1);
    REQUIRE(v[0].value == 42);
}

TEST_CASE("reallocate moves existing elements", "[minivector]")
{
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

TEST_CASE("MiniVector works with non-assignable type", "[minivector]")
{
    MiniVector<Tracker> v;

    v.push_back(Tracker(10));
    v.push_back(Tracker(20));
    v.push_back(Tracker(30));

    REQUIRE(v.size() == 3);
    REQUIRE(v[0].value == 10);
    REQUIRE(v[1].value == 20);
    REQUIRE(v[2].value == 30);
}
