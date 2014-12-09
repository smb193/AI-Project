#include "revAI.h"

ReversiAI::ReversiAI( Game * g, int diff )
    : game(g), difficulty(diff)
{
    //should be moved somewhere else later
    srand( time(NULL) );
	isFlipped = false;
	isRotate = false;
}

Vector2i ReversiAI::takeTurn()
{
    //get list of available moves
    possibleMoves = game->getMoves();

	//apply weights
	for (int n = 0; n < possibleMoves.size(); n++)
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
		checkCorners();
    }

    //advanced heuristics
	if (difficulty >= 2)
	{
		if (inOpening && !checkOpenings())
		{
			inOpening = false;
			isFlipped = false;
			isRotate = false;
		}
		if(!inOpening) {
			checkMobility();
			checkScores();
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
	std::cout << "Picking random move\n\n";
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
		if (greedCount == tempGreed) {
			if (rand() % 2)
				greedMove = bestMoves[i];
		}
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
	std::vector<Vector2i> possible; //Vector to store book possibilities; May not prune at all.
	std::vector<Vector2i> madeMoves = game->allMoves; //Gets list of moves made.
	if (isRotate) { //Need to convert made moves in order to follow opening books
		if (isFlipped) { //is board both flipped and rotated?
			for (int i = 0; i < madeMoves.size(); i++) {
				int temp = madeMoves[i].x;
				madeMoves[i].x = madeMoves[i].y;
				madeMoves[i].y = temp;
			}
		}
		else { //is board only rotated?
			for (int i = 0; i < madeMoves.size(); i++) {
				madeMoves[i].x = 7 - madeMoves[i].x;
				madeMoves[i].y = 7 - madeMoves[i].y;
			}
		}
	}
	else if (isFlipped) { //is board only flipped?
		for (int i = 0; i < madeMoves.size(); i++) {
			int temp = madeMoves[i].x;
			madeMoves[i].x = 7 - madeMoves[i].y;
			madeMoves[i].y = 7 - temp;
		}
	}
	
	if (madeMoves.empty()) { //checks if computer plays first
		possible.push_back(Vector2i(2, 4));
		bestMoves = possible;
		isFlipped = false;
		isRotate = false;
		return true;
	}

	else if (madeMoves.size() == 1) { //checks if computer plays second
		if (madeMoves[0].x == 2 && madeMoves[0].y == 4) {
			isFlipped = false;
			isRotate = false;
			possible.push_back(Vector2i(4, 5));
		}
		else if (madeMoves[0].x == 5 && madeMoves[0].y == 3) {
			isFlipped = false;
			isRotate = true;
			possible.push_back(Vector2i(3, 2));
		}
		else if (madeMoves[0].x == 3 && madeMoves[0].y == 5) {
			isFlipped = true;
			isRotate = false;
			possible.push_back(Vector2i(2, 3));
		}
		else if (madeMoves[0].x == 4 && madeMoves[0].y == 2) {
			isFlipped = true;
			isRotate = true;
			possible.push_back(Vector2i(5, 4));
		}
		bestMoves = possible;
		return true;
	}

	if (madeMoves.size() > 1) { //Board is converted, so simply follow opening books (if able)
		int i = madeMoves.size() - 1;
		if (i < 6 && (madeMoves[i].x == Diag1[i].x && madeMoves[i].y == Diag1[i].y)) 
			possible.push_back(Diag1[i + 1]);
		if (i < 8 && (madeMoves[i].x == Diag2[i].x && madeMoves[i].y == Diag2[i].y)) 
			possible.push_back(Diag2[i + 1]);
		if (i < 2 && (madeMoves[i].x == Parallel[i].x && madeMoves[i].y == Parallel[i].y))
			possible.push_back(Parallel[i + 1]);
		if (i < 8 && (madeMoves[i].x == Perpend1a[i].x && madeMoves[i].y == Perpend1a[i].y))
			possible.push_back(Perpend1a[i + 1]);
		if (i < 8 && (madeMoves[i].x == Perpend1b[i].x && madeMoves[i].y == Perpend1b[i].y))
			possible.push_back(Perpend1b[i + 1]);
		if (i < 4 && (madeMoves[i].x == Perpend2[i].x && madeMoves[i].y == Perpend2[i].y))
			possible.push_back(Perpend2[i + 1]);
		if (!possible.empty()) {
			if (isRotate) {
				if (isFlipped) {
					for (int j = 0; j < possible.size(); j++) {
						int temp = possible[j].x;
						possible[j].x = possible[j].y;
						possible[j].y = temp;
					}
				}
				else {
					for (int j = 0; j < possible.size(); j++) {
						possible[j].x = 7 - possible[j].x;
						possible[j].y = 7 - possible[j].y;
					}
				}
			}
			else if (isFlipped) {
				for (int j = 0; j < possible.size(); j++) {
					int temp = possible[j].x;
					possible[j].x = 7 - possible[j].y;
					possible[j].y = 7 - temp;
				}
			}
			bestMoves = possible;
			return true;
		}
	}

    return false;
}

bool ReversiAI::checkScores()
{
	double averageScore = 0;

	//find the average score
	for (int n = 0; n < bestMoves.size(); n++)
	{
		averageScore += bestMoves[n].score;
	}

	averageScore = averageScore / bestMoves.size();

	//remove all below average
	for (int n = 0; n < bestMoves.size(); n++)
	{
		if (bestMoves[n].score <= averageScore)
		{
			bestMoves.erase(bestMoves.begin() + n);

			n--;
		}
	}

	return true;
}

//checks how a move affects opponents mobility next turn
bool ReversiAI::checkMobility()
{
	for (int n = 0; n < bestMoves.size(); n++)
	{
		//Get game state next turn with current move
		Game nextTurn = *game;
		nextTurn.makeMove( bestMoves[n].x, bestMoves[n].y );

		bestMoves[n].score -= nextTurn.getNumMoves() * MOBILITY_FACTOR;
	}
	return true;
}