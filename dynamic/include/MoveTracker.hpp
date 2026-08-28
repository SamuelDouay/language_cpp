#ifndef LANGUAGE_CPP_MOVETRACKER_H
#define LANGUAGE_CPP_MOVETRACKER_H

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

#endif //LANGUAGE_CPP_MOVETRACKER_H
