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
        possibleMoves[n].score = weights[possibleMoves[n].x][possibleMoves[n].y] * WEIGHT_FACTOR;
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

        checkMobility();
        checkScores();
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

bool ReversiAI::checkScores()
{
    double averageScore = 0;

    //std::cout << "Scores: \n";

    //find the average score
    for( int n = 0; n < bestMoves.size(); n++ )
    {
        //std::cout << bestMoves[n].score << ", ";
        averageScore += bestMoves[n].score;
    }

    averageScore = averageScore / bestMoves.size();


    //std::cout << "\nAverage Score: " << averageScore << "\n";
    //std::cout << "Moves pruned due to score: \n";

    //remove all below average
    for( int n = 0; n < bestMoves.size(); n++ )
    {
        if( bestMoves[n].score <= averageScore )
        {
            //std::cout << "(" << (*(bestMoves.begin() + n ) ).x << ", " <<
            //    (*(bestMoves.begin() + n ) ).y << "), ";

            bestMoves.erase( bestMoves.begin() + n );

            n--;
        }
    }
    //std::cout << "\n";

    return true;
}

//checks how a move affects opponents mobility next turn
bool ReversiAI::checkMobility()
{
    for( int n = 0; n < bestMoves.size(); n++ )
    {
        //Get game state next turn with current move
        Game nextTurn = *game;
        nextTurn.makeMove( bestMoves[n].x, bestMoves[n].y );

        bestMoves[n].score -= nextTurn.getNumMoves() * MOBILITY_FACTOR;
        //std::cout << nextTurn.getNumMoves() << ", ";
    }
    //std::cout << "\n";
}
