#ifndef REVAI_H
#define	REVAI_H

#include "gameLogic.h"
#include <string>
#include <time.h>
#include <iostream>

const int weights[8][8] =
{
	{ 99, -8, 8, -6, 6, 8, -8, 99 },
	{ -8, -24, -4, -3, -3, -4, -24, -8 },
	{ 8, -4, 7, 4, 4, 7, -4, 8 },
	{ 6, -3, 4, 0, 0, 4, -3, 6 },
	{ 6, -3, 4, 0, 0, 4, -3, 6 },
	{ 8, -4, 7, 4, 4, 7, -4, -8 },
	{ -8, -24, -4, -3, -3, -4, -24, 99 },
	{ 99, -8, 8, 6, 6, 8, -8, 99 }
};

const double WEIGHT_FACTOR = 1;
const double MOBILITY_FACTOR = 5;

const std::vector<Vector2i> Diag1 = 
	{ Vector2i(2, 4), Vector2i(2, 5), Vector2i(3, 5), //C4c3D3
	  Vector2i(2, 3), Vector2i(1, 4), Vector2i(3, 6), //c5B4d2
	  Vector2i(3, 2) };								  //D6

const std::vector<Vector2i> Diag2 =
	{ Vector2i(2, 4), Vector2i(2, 5), Vector2i(3, 5), //C4c3D3
	  Vector2i(2, 3), Vector2i(3, 2), Vector2i(5, 4), //c5D6f4
	  Vector2i(5, 3), Vector2i(3, 6), Vector2i(1, 3)};//F5d2B5

const std::vector<Vector2i> Parallel =
	{ Vector2i(2, 4), Vector2i(2, 3), Vector2i(3, 2) }; //C4c5D6

const std::vector<Vector2i> Perpend1a =
	{ Vector2i(2, 4), Vector2i(4, 5), Vector2i(5, 2), //C4e3F6
	  Vector2i(4, 2), Vector2i(5, 3), Vector2i(2, 3), //e6F5c5
	  Vector2i(5, 4), Vector2i(6, 2), Vector2i(5, 1), //F4g6F7
	};

const std::vector<Vector2i> Perpend1b =
	{ Vector2i(2, 4), Vector2i(4, 5), Vector2i(5, 3), //C4e3F5
	  Vector2i(4, 2), Vector2i(5, 2), Vector2i(2, 3), //e6F6c5
	  Vector2i(5, 4), Vector2i(6, 2), Vector2i(5, 1)};//F4g6F7

const std::vector<Vector2i> Perpend2 =
	{ Vector2i(2, 4), Vector2i(4, 5), Vector2i(5, 3), //C4e3F5
	  Vector2i(1, 4), Vector2i(5, 5) }; //b4F3

class ReversiAI
{
    private:
        //used in makeMove and pruning functions
        std::vector<Vector2i> possibleMoves;
        std::vector<Vector2i> bestMoves;

        bool inOpening;
		bool isFlipped;
		bool isRotate;

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
