#ifndef LANGUAGE_CPP_MOVETRACKER_H
#define LANGUAGE_CPP_MOVETRACKER_H

struct MoveTracker
{
    static int moves; // constructeur de déplacement
    static int assignments; // affectation par déplacement

    int value;

    explicit MoveTracker(int v) : value(v)
    {
    }

    MoveTracker(const MoveTracker&) = delete;
    MoveTracker& operator=(const MoveTracker&) = delete;

    MoveTracker(MoveTracker&& other) noexcept : value(other.value)
    {
        ++moves;
    }

    MoveTracker& operator=(MoveTracker&& other) noexcept
    {
        if (this != &other)
            value = other.value;
        ++assignments;
        return *this;
    }
};

#endif //LANGUAGE_CPP_MOVETRACKER_H
