#ifndef CASE_H
#define CASE_H

enum class CaseState {Hidden, Revealed, Flagged};

struct Case
{
    CaseState state = CaseState::Hidden;
    bool isMine = false;
    unsigned int nbNearbyMines = 0;
};

#endif //CASE_H
