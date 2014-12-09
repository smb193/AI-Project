#ifndef REVAI_H
#define	REVAI_H

#include "gameLogic.h"
#include <string>
#include <time.h>

const int weights[8][8] =
    {
        {99, -8 ,  8, -6,  6,  8, -8 , 99},
        {-8, -24, -4, -3, -3, -4, -24, -8},
        { 8, -4 ,  7,  4,  4,  7, -4 ,  8},
        { 6, -3 ,  4,  0,  0,  4, -3 ,  6},
        { 6, -3 ,  4,  0,  0,  4, -3 ,  6},
        { 8, -4 ,  7,  4,  4,  7, -4 , -8},
        {-8, -24, -4, -3, -3, -4, -24, 99},
        {99, -8 ,  8,  6,  6,  8, -8 , 99}
    };

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
        bool checkWeights();
        Vector2i greedyApproach();
};

#endif
