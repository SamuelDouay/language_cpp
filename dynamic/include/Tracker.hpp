#ifndef LANGUAGE_CPP_TRACKER_H
#define LANGUAGE_CPP_TRACKER_H

struct Tracker
{
    static int alive; // nombre d'objets vivants
    int value;
    explicit Tracker(const int v = 0) : value(v) { ++alive; }
    Tracker(const Tracker& other) : value(other.value) { ++alive; }
    Tracker(Tracker&& other) noexcept : value(other.value) { ++alive; }
    ~Tracker() { --alive; }

    Tracker& operator=(Tracker&& other) noexcept
    {
        if (this != &other)
            value = other.value;
        return *this;
    }

    Tracker& operator=(const Tracker& other)
    {
        if (this != &other)
            value = other.value;
        return *this;
    }
};

#endif //LANGUAGE_CPP_TRACKER_H
