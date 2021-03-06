#include "gameLogic.h"
#include <iostream>

/*=====================
    Constructor
======================*/
Game::Game( int numPlayers, int height, int width )
	: players(numPlayers), h(height), w(width)
{
	std::vector< int > row;
	for( int n = 0; n < w; n++ )
	{
		row.push_back( 0 );
	}

	for( int n = 0; n < h; n++ )
	{
		board.push_back( row );
	}

	currentPlayer = 10;
}

Game::Game(const Game & other) :
players(other.players), h(other.h), w(other.w), board(other.board),
currentPlayer(other.currentPlayer)
{

}

/*=====================
    General Functions
======================*/

bool Game::isBoardFilled()
{
    for( int n = 0; n < h; n++ )
    {
        for( int i = 0; i < w; i++ )
        {
            if( board[n][i] == 0 )
                return false;
        }
    }

    return true;
}

//only uses current player to check
bool Game::isValidMove( int x, int y )
{
    //is it empty?
    if   ( ( x >= w ) || ( x < 0 )
        || ( y >= h ) || ( y < 0 )
        || ( board[x][y] != 0 ) )
        return false;

    //check y directions
    for( int vy = -1; vy < 2; vy++ )
    {
        if( vy == -1 )
        {
            if( y < 2 )
                continue;
        }
        if( vy == 1 )
        {
            if( y > h - 2 )
                continue;
        }

        //check x directions
        for( int vx = -1; vx < 2; vx++ )
        {

            if( vx == -1 )
            {
                if( x < 2 )
                    continue;
            }
            if( vx == 1 )
            {
                if( x > w - 2 )
                    continue;
            }

            //if adjacent tile is owned by opponent
            if( ( board[x + vx][y + vy] != currentPlayer) && ( board[x + vx][y + vy] != 0 ) )
            {
                if ( checkDirection( x, y, vx, vy ) )
                    return true;
            }
        }
    }

    return false;

}

//checks from (x,y) to see if there are any moves in the given direction
bool Game::checkDirection( int x, int y, int vx, int vy )
{

    while( ( ( x + vx ) < w ) && ( ( x + vx ) >= 0 )
        && ( ( y + vy ) < h ) && ( ( y + vy ) >= 0 ) )
    {
        x += vx;
        y += vy;

        if( board[x][y] == 0 )
            return false;
        if( board[x][y] == currentPlayer )
            return true;
    }

    return false;
}

bool Game::makeMove( int x, int y )
{
    if( !isValidMove( x, y ) )
        return false;

    //find tiles to flip
    std::vector< Vector2i > converted;
    converted = getConvertedTiles( x, y );

    //flip tiles
    board[x][y] = currentPlayer;
	allMoves.push_back(Vector2i(x, y));
    for( int n = 0; n < converted.size(); n++ )
    {
        board[ converted[n].x ][ converted[n].y ] = currentPlayer;
    }

    if( currentPlayer == 1 )
        currentPlayer = 10;
    else
        currentPlayer = 1;
	if (getMoves().size() == 0) {
		if (currentPlayer == 1)
			currentPlayer = 10;
		else
			currentPlayer = 1;
		std::cout << "\nNo available moves, player passes turn\n";
	}
	return true;
}

void Game::resetGame()
{
    for( int n = 0; n < h; n++)
    {
        for( int i = 0; i < w; i++ )
        {
            board[n][i] = 0;
        }
    }

    board[ ( h / 2 ) - 1 ][ ( w / 2 ) - 1 ] = 1;
    board[ ( h / 2 ) - 1 ][ ( w / 2 ) ] = 2;
    board[ ( h / 2 ) ][ ( w / 2 ) - 1 ] = 2;
    board[ ( h / 2 ) ][ ( w / 2 ) ] = 1;
}

/*=====================
    Setters
======================*/
bool Game::setCurrentPlayer( int p )
{
    if( ( p != 0 ) && (p <= players ) )
    {
        currentPlayer = p;
        return true;
    }

    return false;
}

/*=====================
    Getters
======================*/
int Game::getScore( int player )
{
    int score = 0;

    for( int n = 0; n < h; n++ )
    {
        for( int i = 0; i < w; i++ )
        {
            if( board[n][i] == player )
                score++;
        }
    }

    return score;
}

std::vector< Vector2i > Game::getMoves()
{
    std::vector< Vector2i > moves;

    for( int n = 0; n < h; n++ )
    {
        for( int i = 0; i < w; i++ )
        {
            if ( isValidMove( i, n ) )
                moves.push_back( Vector2i( i, n ) );
        }
    }

    return moves;
}

std::vector< Vector2i > Game::getConvertedTiles( int x, int y )
{
    std::vector<Vector2i> convertedTiles;

    for( int vy = -1; vy < 2; vy++ )
    {
        for( int vx = -1; vx < 2; vx++ )
        {
            int posX = x;
            int posY = y;

            //check adjacent tile in current direction
            if(    ( ( posX + vx ) < w ) && ( ( posX + vx ) >= 0 )
                && ( ( posY + vy ) < h ) && ( ( posY + vy ) >= 0 ) )
            {
                posX += vx;
                posY += vy;

                if( ( board[posX][posY] == currentPlayer ) || ( board[posX][posY] == 0 ) )
                    continue;
            }

            //check remaining tiles
            while( ( ( posX + vx ) < w ) && ( ( posX + vx ) >= 0 )
                && ( ( posY + vy ) < h ) && ( ( posY + vy ) >= 0 ) )
            {
                posX += vx;
                posY += vy;
                if( board[posX][posY] == 0 )
                {
                    break;
                }

                else if( board[posX][posY] == currentPlayer )
                {
                    //add all from (x, y) to (posX, posY)
                    int pos2X = x + vx;
                    int pos2Y = y + vy;
                    while( ( ( pos2X - posX ) != 0 ) || ( ( pos2Y - posY ) != 0 ) )
                    {
                        convertedTiles.push_back( Vector2i( pos2X, pos2Y ) );

                        pos2X += vx;
                        pos2Y += vy;
                    }
                }
            }
        }
    }

    /*
	std::cout << "Tiles converted: ";

    for( int n = 0; n < convertedTiles.size(); n++ )
    {
        std::cout << "(" << convertedTiles[n].x << ", " << convertedTiles[n].y << ") ";
    }

	std::cout << std::endl;
    */

    return convertedTiles;

}

int Game::getNumConverted( int x, int y )
{
    int numConverted = 0;

    for( int vy = -1; vy < 2; vy++ )
    {
        for( int vx = -1; vx < 2; vx++ )
        {
            int posX = x;
            int posY = y;

            //check adjacent tile in current direction
            if(    ( ( posX + vx ) < w ) && ( ( posX + vx ) >= 0 )
                && ( ( posY + vy ) < h ) && ( ( posY + vy ) >= 0 ) )
            {
                posX += vx;
                posY += vy;

                if( ( board[posX][posY] == currentPlayer ) || ( board[posX][posY] == 0 ) )
                    continue;
            }

            //check remaining tiles
            while( ( ( posX + vx ) < w ) && ( ( posX + vx ) >= 0 )
                && ( ( posY + vy ) < h ) && ( ( posY + vy ) >= 0 ) )
            {
                posX += vx;
                posY += vy;
                if( board[posX][posY] == 0 )
                {
                    break;
                }

                else if( board[posX][posY] == currentPlayer )
                {
                    //add all from (x, y) to (posX, posY)
                    int pos2X = x + vx;
                    int pos2Y = y + vy;
                    while( ( ( pos2X - posX ) != 0 ) || ( ( pos2Y - posY ) != 0 ) )
                    {
                        numConverted++;

                        pos2X += vx;
                        pos2Y += vy;
                    }
                }
            }
        }
    }
    return numConverted;

}

int Game::getNumMoves()
{
	int numMoves = 0;

	for (int n = 0; n < h; n++)
	{
		for (int i = 0; i < w; i++)
		{
			if (isValidMove(i, n))
				numMoves++;
		}
	}

	return numMoves;
}

void Game::printScoresMoves() {
	std::cout << "========SCORE========\n";
	std::cout << "Black: " << getScore(10) << "   ";
	std::cout << "White: " << getScore(1) << "\n\n";
	int ind = 0;

	std::vector< Vector2i >moves = getMoves();
	std::cout << "Turn: ";
	if (currentPlayer == 1)
		std::cout << "White - Moves: ";
	else
		std::cout << "Black - Moves: ";
	while (ind < moves.size()) {
		std::cout << "(" << moves[ind].x << ", " << moves[ind].y << ") ";
		ind++;
	}
	std::cout << "\n\n";
}