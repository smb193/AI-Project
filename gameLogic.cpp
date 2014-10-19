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

	currentPlayer = 1;

	board[ ( h / 2 ) - 1 ][ ( w / 2 ) - 1 ] = 1;
    board[ ( h / 2 ) - 1 ][ ( w / 2 ) ] = 2;
    board[ ( h / 2 ) ][ ( w / 2 ) - 1 ] = 2;
    board[ ( h / 2 ) ][ ( w / 2 ) ] = 1;
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
        || ( board[y][x] != 0 ) )
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
            if( ( board[y + vy][x + vx] != currentPlayer) && ( board[y + vy][x + vx] != 0 ) )
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

        if( board[y][x] == 0 )
            return false;
        if( board[y][x] == currentPlayer )
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
    board[y][x] = currentPlayer;
    for( int n = 0; n < converted.size(); n++ )
    {
        board[ converted[n].y ][ converted[n].x ] = currentPlayer;
    }

    if( currentPlayer < players )
        currentPlayer++;
    else
        currentPlayer = 1;
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

                if( ( board[posY][posX] == currentPlayer ) || ( board[posY][posX] == 0 ) )
                    continue;
            }

            //check remaining tiles
            while( ( ( posX + vx ) < w ) && ( ( posX + vx ) >= 0 )
                && ( ( posY + vy ) < h ) && ( ( posY + vy ) >= 0 ) )
            {
                posX += vx;
                posY += vy;
                if( board[posY][posX] == 0 )
                {
                    break;
                }

                else if( board[posY][posX] == currentPlayer )
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

    for( int n = 0; n < convertedTiles.size(); n++ )
    {
        std::cout << convertedTiles[n].x << ", " << convertedTiles[n].y << "\n";
    }

    return convertedTiles;

}
