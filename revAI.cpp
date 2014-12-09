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

    //apply weights
    for( int n = 0; n < possibleMoves.size(); n++ )
    {
        possibleMoves[n].score = weights[possibleMoves[n].x][possibleMoves[n].y];
    }

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
    std::vector<Vector2i> availableCorners;

    for( int n = 0; n < bestMoves.size(); n++ )
    {
        if( ( ( bestMoves[n].x == 0 ) && ( bestMoves[n].y == 0 ) ) ||
            ( ( bestMoves[n].x == 0 ) && ( bestMoves[n].y == 7 ) ) ||
            ( ( bestMoves[n].x == 7 ) && ( bestMoves[n].y == 0 ) ) ||
            ( ( bestMoves[n].x == 7 ) && ( bestMoves[n].y == 7 ) ) )
        {
            availableCorners.push_back( bestMoves[n] );

        }
    }

    if( !availableCorners.empty() )
        bestMoves = availableCorners;

    return false;
}

//follow an opening if possible
bool ReversiAI::checkOpenings()
{
    return false;
}

bool ReversiAI::checkWeights()
{
    int averageScore = 0;

    //find the average score
    for( int n = 0; n < bestMoves.size(); n++ )
    {
        averageScore += bestMoves[n].score;
    }

    averageScore /= bestMoves.size();

    //remove all below average
    for( int n = 0; n < bestMoves.size(); n++ )
    {
        if( bestMoves[n].score < averageScore )
        {
            bestMoves.erase( bestMoves.begin() + n );

            n--;
        }
    }

    return true;
}
