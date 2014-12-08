#ifndef REVAI_H
#define	REVAI_H

#include "gameLogic.h"
#include <string>
#include <time.h>

class ReversiAI
{
    private:
        //used in makeMove and pruning functions
        std::vector<Vector2i> possibleMoves;
        std::vector<Vector2i> bestMoves;

        bool inOpening;

    public:
        Game * game;

        //0 = random move, 1 = greedy, 2 = hard
        int difficulty;

        ReversiAI( Game * g, int diff = 0 );

        Vector2i takeTurn();

        //pruning functions
        bool checkCorners();
        bool checkOpenings();
        void greedyApproach();
};

#endif
