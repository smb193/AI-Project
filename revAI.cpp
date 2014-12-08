#include "revAI.h"

ReversiAI::ReversiAI( Game * g, int diff )
    : game(g), difficulty(diff)
{
    //should be moved somewhere else later
    srand( time(NULL) );
}

Vector2i ReversiAI::takeTurn()
{
    //get list of available moves
    possibleMoves = game->getMoves();

	if (possibleMoves.empty()) {
		return Vector2i(-1, -1);
	}

    bestMoves = possibleMoves; //list to prune
    inOpening = true;

    //check for corners
    if( difficulty >= 1 )
    {
		if (checkCorners()) 
		{
		
		}
    }

    //advanced heuristics
    if( difficulty >= 2 )
    {
        if( inOpening && !checkOpenings() )
        {
            inOpening = false;
        }
    }

    //simple heuristics
    if( difficulty == 1 )
    {
        return greedyApproach();
    }

    //pick random remaining move
    if( !( bestMoves.empty() ) )
    {
        Vector2i finalMove = bestMoves[ rand() % bestMoves.size() ];
        game->makeMove( finalMove.x, finalMove.y );

        return finalMove;
    }

    //pick random move
    Vector2i finalMove = possibleMoves[ rand() % possibleMoves.size()];
    game->makeMove(finalMove.x, finalMove.y );

    return finalMove;
}

/*=====================
    Pruning functions
======================*/

//look for the moves that will convert the most pieces
Vector2i ReversiAI::greedyApproach()
{
	Vector2i greedMove(0, 0);
	int greedCount = 0;
	int tempGreed;
	for (int i = 0; i < bestMoves.size(); i++) {
		tempGreed = game->getConvertedTiles(bestMoves[i].x, bestMoves[i].y).size();
		if (greedCount < tempGreed) {
			greedCount = tempGreed;
			greedMove = bestMoves[i];
		}
		/*if (greedCount == tempGreed) {
			if (rand() % 2)
				greedMove = bestMoves[i];
		}*/
	}
	game->makeMove(greedMove.x, greedMove.y);
	return greedMove;
}

//if corners are available, throw out everything except them
bool ReversiAI::checkCorners()
{
    return false;
}

//follow an opening if possible
bool ReversiAI::checkOpenings()
{
    return false;
}
