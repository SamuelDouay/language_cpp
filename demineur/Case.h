#ifndef CASE_H
#define CASE_H

struct Case
{
    bool isMine = false;
    bool isReveal = false;
    // bool isFlag;
    unsigned int nbMineVoisin = 0;
};

#endif //CASE_H
