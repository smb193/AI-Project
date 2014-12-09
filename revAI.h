#ifndef REVAI_H
#define	REVAI_H

#include "gameLogic.h"
#include <string>
#include <time.h>
#include <iostream>

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

const double WEIGHT_FACTOR = 1;
const double MOBILITY_FACTOR = 5;


class ReversiAI
{
    private:
        //used in makeMove and pruning functions
        std::vector<Vector2i> possibleMoves;
        std::vector<Vector2i> bestMoves;

        bool inOpening;

        //pruning functions
        bool checkCorners();
        bool checkOpenings();
        bool checkScores();
        bool checkMobility();
        Vector2i greedyApproach();

    public:
        Game * game;

        //0 = random move, 1 = greedy, 2 = hard
        int difficulty;

        ReversiAI( Game * g, int diff = 0 );

        Vector2i takeTurn();
};

#endif
