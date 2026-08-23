#ifndef CASE_H
#define CASE_H

enum class NumberOrigin { FIXED, PLAYER };

struct Cell
{
    unsigned int value = 0;
    NumberOrigin play = NumberOrigin::FIXED;
};

#endif //CASE_H
