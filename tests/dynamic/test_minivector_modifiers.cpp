#include <catch2/catch_test_macros.hpp>
#include "MiniVector.hpp"
#include "Throwing.hpp"
#include "Tracker.hpp"

TEST_CASE("push_back adds elements and grows capacity", "[minivector][modifier]")
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

TEST_CASE("pop_back removes last element", "[minivector][modifier]")
{
    MiniVector<int> v = {1, 2, 3};
    v.pop_back();
    REQUIRE(v.size() == 2);
    REQUIRE(v[1] == 2);
}

TEST_CASE("clear empties vector but keeps capacity", "[minivector][modifier]")
{
    MiniVector<int> v = {1, 2, 3};
    std::size_t cap = v.capacity();
    v.clear();
    REQUIRE(v.empty());
    REQUIRE(v.capacity() == cap);
}

TEST_CASE("reserve preserves existing elements", "[minivector][modifier]")
{
    MiniVector<int> v = {1, 2, 3};
    v.reserve(100);
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() >= 100);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
}

TEST_CASE("reserve does not shrink capacity", "[minivector][modifier]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};
    std::size_t old_cap = v.capacity();
    v.reserve(2);
    REQUIRE(v.capacity() == old_cap);
    v.reserve(old_cap + 1);
    REQUIRE(v.capacity() >= old_cap + 1);
}

TEST_CASE("swap exchanges contents and capacities", "[minivector][modifier]")
{
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

TEST_CASE("emplace_back constructs element in place", "[minivector][modifier]")
{
    struct Point
    {
        int x, y;

        Point(int a, int b) : x(a), y(b)
        {
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

TEST_CASE("emplace_back works with non-copyable type", "[minivector][modifier]")
{
    struct NonCopyable
    {
        int value;

        explicit NonCopyable(int v) : value(v)
        {
        }

        NonCopyable(const NonCopyable&) = delete;

        NonCopyable& operator=(const NonCopyable&) = delete;

        NonCopyable(NonCopyable&& other) noexcept : value(other.value)
        {
        }

        NonCopyable& operator=(NonCopyable&&) = default;
    };
    MiniVector<NonCopyable> v;
    v.emplace_back(42);
    REQUIRE(v.size() == 1);
    REQUIRE(v[0].value == 42);
}

TEST_CASE("push_back handles self-reference during reallocation",
          "[minivector][modifier]")
{
    MiniVector<int> v = {1, 2, 3};

    v.push_back(v[0]);

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 1);
}

TEST_CASE("emplace_back handles self-reference during reallocation",
          "[minivector][modifier]")
{
    MiniVector<int> v = {1, 2, 3};

    v.emplace_back(v[0]);

    REQUIRE(v.size() == 4);
    REQUIRE(v[3] == 1);
}

TEST_CASE("reserve on empty vector allocates capacity",
          "[minivector][modifier]")
{
    MiniVector<int> v;

    v.reserve(100);

    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() >= 100);
    REQUIRE(v.data() != nullptr);
}

TEST_CASE("emplace_back handles multiple constructor arguments",
          "[minivector][modifier]")
{
    struct Point
    {
        int x;
        int y;

        Point(int x, int y) : x(x), y(y)
        {
        }
    };

    MiniVector<Point> v;

    v.emplace_back(10, 20);

    REQUIRE(v.size() == 1);
    REQUIRE(v[0].x == 10);
    REQUIRE(v[0].y == 20);
}

TEST_CASE("push_back rvalue handles self-reference during reallocation",
          "[minivector][modifier]")
{
    MiniVector<int> v = {1, 2, 3};

    v.push_back(std::move(v[0]));

    REQUIRE(v.size() == 4);
    REQUIRE(v[3] == 1);
}


TEST_CASE("emplace_back does not reallocate when capacity is available",
          "[minivector][modifier]")
{
    MiniVector<int> v;

    v.reserve(10);

    auto* old_data = v.data();

    v.emplace_back(42);

    REQUIRE(v.data() == old_data);
    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 42);
}

TEST_CASE("emplace_back forwards multiple arguments",
          "[minivector][modifier]")
{
    struct Point
    {
        int x;
        int y;
        int z;

        Point(int x, int y, int z)
            : x(x), y(y), z(z)
        {
        }
    };

    MiniVector<Point> v;

    v.emplace_back(1, 2, 3);

    REQUIRE(v.size() == 1);
    REQUIRE(v[0].x == 1);
    REQUIRE(v[0].y == 2);
    REQUIRE(v[0].z == 3);
}

// =========================================================
// erase(pos) — version single element
// =========================================================

TEST_CASE("erase(pos) removes element at given position", "[minivector][erase]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};
    auto it = v.erase(v.begin() + 2); // supprime 3

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 4);
    REQUIRE(v[3] == 5);
    REQUIRE(*it == 4); // itérateur vers l'élément suivant
}

TEST_CASE("erase(pos) on first element", "[minivector][erase]")
{
    MiniVector<int> v = {1, 2, 3};
    v.erase(v.begin());

    REQUIRE(v.size() == 2);
    REQUIRE(v[0] == 2);
    REQUIRE(v[1] == 3);
}

TEST_CASE("erase(pos) on last element", "[minivector][erase]")
{
    MiniVector<int> v = {1, 2, 3};
    auto it = v.erase(v.begin() + 2); // supprime 3

    REQUIRE(v.size() == 2);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(it == v.end()); // plus d'élément suivant
}

TEST_CASE("erase(pos) on single-element vector leaves it empty",
          "[minivector][erase]")
{
    MiniVector<int> v = {42};
    auto it = v.erase(v.begin());

    REQUIRE(v.empty());
    REQUIRE(it == v.end());
}

TEST_CASE("erase(pos) does not change capacity", "[minivector][erase]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};
    std::size_t cap = v.capacity();
    v.erase(v.begin() + 2);

    REQUIRE(v.capacity() == cap);
}

TEST_CASE("erase(pos) calls destructor of removed element",
          "[minivector][erase]")
{
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v;
        v.push_back(Tracker(1));
        v.push_back(Tracker(2));
        v.push_back(Tracker(3));

        REQUIRE(Tracker::alive == 3);

        v.erase(v.begin() + 1); // supprime Tracker(2)

        REQUIRE(Tracker::alive == 2);
        REQUIRE(v.size() == 2);
        REQUIRE(v[0].value == 1);
        REQUIRE(v[1].value == 3);
    }
    REQUIRE(Tracker::alive == 0);
}

TEST_CASE("erase(pos) in loop works correctly", "[minivector][erase]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};

    // Supprime tous les éléments pairs
    for (auto it = v.begin(); it != v.end();)
    {
        if (*it % 2 == 0)
            it = v.erase(it);
        else
            ++it;
    }

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 3);
    REQUIRE(v[2] == 5);
    REQUIRE(v[3] == 7);
}

// =========================================================
// erase(first, last) — version range
// =========================================================

TEST_CASE("erase(first, last) removes a sub-range in the middle",
          "[minivector][erase]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5, 6, 7};
    auto it = v.erase(v.begin() + 2, v.begin() + 5); // supprime 3,4,5

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 6);
    REQUIRE(v[3] == 7);
    REQUIRE(*it == 6);
}

TEST_CASE("erase(first, last) from begin", "[minivector][erase]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};
    auto it = v.erase(v.begin(), v.begin() + 3); // supprime 1,2,3

    REQUIRE(v.size() == 2);
    REQUIRE(v[0] == 4);
    REQUIRE(v[1] == 5);
    REQUIRE(it == v.begin());
}

TEST_CASE("erase(first, last) to end", "[minivector][erase]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};
    auto it = v.erase(v.begin() + 2, v.end()); // supprime 3,4,5

    REQUIRE(v.size() == 2);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(it == v.end());
}

TEST_CASE("erase(first, last) with entire range clears vector",
          "[minivector][erase]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};
    auto it = v.erase(v.begin(), v.end());

    REQUIRE(v.empty());
    REQUIRE(it == v.end());
}

TEST_CASE("erase(first, last) with empty range does nothing",
          "[minivector][erase]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};
    auto it = v.erase(v.begin() + 2, v.begin() + 2);

    REQUIRE(v.size() == 5);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 4);
    REQUIRE(v[4] == 5);
    REQUIRE(it == v.begin() + 2);
}

TEST_CASE("erase(first, last) does not change capacity",
          "[minivector][erase]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};
    std::size_t cap = v.capacity();
    v.erase(v.begin() + 1, v.begin() + 4);

    REQUIRE(v.capacity() == cap);
}

TEST_CASE("erase(first, last) calls destructors of removed elements",
          "[minivector][erase]")
{
    Tracker::alive = 0;
    {
        MiniVector<Tracker> v;
        v.push_back(Tracker(1));
        v.push_back(Tracker(2));
        v.push_back(Tracker(3));
        v.push_back(Tracker(4));
        v.push_back(Tracker(5));

        REQUIRE(Tracker::alive == 5);

        v.erase(v.begin() + 1, v.begin() + 4); // supprime 2,3,4

        REQUIRE(Tracker::alive == 2);
        REQUIRE(v.size() == 2);
        REQUIRE(v[0].value == 1);
        REQUIRE(v[1].value == 5);
    }
    REQUIRE(Tracker::alive == 0);
}

TEST_CASE("erase(first, last) on single element", "[minivector][erase]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};
    v.erase(v.begin() + 2, v.begin() + 3);

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 4);
    REQUIRE(v[3] == 5);
}

TEST_CASE("erase(first, last) in a loop", "[minivector][erase]")
{
    MiniVector<int> v = {1, 2, 2, 3, 2, 4, 2, 5};

    // Supprime tous les 2
    for (auto it = v.begin(); it != v.end();)
    {
        if (*it == 2)
        {
            // Trouve la fin de la plage de 2 consécutifs
            auto next = it;
            while (next != v.end() && *next == 2)
                ++next;
            it = v.erase(it, next);
        }
        else
        {
            ++it;
        }
    }

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 3);
    REQUIRE(v[2] == 4);
    REQUIRE(v[3] == 5);
}

TEST_CASE("erase(first, last) leaves iterators and references valid before the range",
          "[minivector][erase]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5};
    auto it_before = v.begin(); // pointe vers 1
    v.erase(v.begin() + 2, v.begin() + 4);

    REQUIRE(*it_before == 1); // toujours valide
    REQUIRE(v.size() == 3);
}

TEST_CASE("erase(pos) then erase(first, last) combined", "[minivector][erase]")
{
    MiniVector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    v.erase(v.begin()); // 2,3,4,5,6,7,8,9
    v.erase(v.begin() + 1, v.begin() + 4); // supprime 3,4,5

    REQUIRE(v.size() == 5);
    REQUIRE(v[0] == 2);
    REQUIRE(v[1] == 6);
    REQUIRE(v[2] == 7);
    REQUIRE(v[3] == 8);
    REQUIRE(v[4] == 9);
}

TEST_CASE("emplace_back grows vector correctly",
          "[minivector][modifier]")
{
    MiniVector<int> v;

    for (int i = 0; i < 100; ++i)
    {
        v.emplace_back(i);
    }

    REQUIRE(v.size() == 100);
    REQUIRE(v.capacity() >= v.size());

    for (int i = 0; i < 100; ++i)
    {
        REQUIRE(v[static_cast<std::size_t>(i)] == i);
    }
}


TEST_CASE("emplace_back preserves vector when construction throws",
          "[minivector][modifier][exception]")
{
    Throwing::constructions = 0;
    Throwing::throw_after = 100;

    MiniVector<Throwing> v;

    v.emplace_back(1);
    v.emplace_back(2);

    REQUIRE(v.size() == 2);
    REQUIRE(v[0].value == 1);
    REQUIRE(v[1].value == 2);

    Throwing::throw_after = Throwing::constructions;

    REQUIRE_THROWS_AS(
        v.emplace_back(3),
        std::runtime_error
    );

    REQUIRE(v.size() == 2);
    REQUIRE(v[0].value == 1);
    REQUIRE(v[1].value == 2);
}
