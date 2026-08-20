#ifndef CASE_H
#define CASE_H

enum class NUMBER_ORIGIN { FIXED, PLAYER };

struct Case
{
    unsigned int value = 0;
    NUMBER_ORIGIN play = NUMBER_ORIGIN::FIXED;
};

#endif //CASE_H
