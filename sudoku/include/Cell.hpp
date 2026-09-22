#ifndef CASE_H
#define CASE_H

enum class NumberOrigin { Given, Player };

struct Cell
{
    unsigned int value = 0;
    NumberOrigin play = NumberOrigin::Given;
};

#endif //CASE_H
