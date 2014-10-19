#include "gameLogic.h"
#include <iostream>

void printMoves( Game & g );
void printBoard( const Game & g );

int main()
{
    Game game;

    /*
    if( game.isValidMove( 3, 5 ) )
        std::cout << "Yes\n";
    else
        std::cout << "No\n";
    */

    printBoard( game );

    bool quit = false;
    while( !quit )
    {
        printBoard( game );
        printMoves( game );
        int x; int y;
        std::cout << "Enter x: ";
        std::cin >> x;
        std::cout << "Enter y: ";
        std::cin >> y;

        if( ( x == -1 ) || (y == -1 ) )
            break;

        game.makeMove ( x, y );

        std::cout << "\nScore:\n"
            << "Player 1: " << game.getScore( 1 ) << "\n"
            << "Player 2: " << game.getScore( 2 ) << "\n";

        if( game.isBoardFilled() )
            quit = true;
    }
}

void printMoves( Game & g )
{
    std::vector< Vector2i > moves;
    moves = g.getMoves();

    std::cout << "Possible moves: \n";

    for( int n = 0; n < moves.size(); n++ )
    {
        std::cout << moves[n].x << ", " << moves[n].y << "\n";
    }
}

void printBoard( const Game & g )
{
    std::cout << "Current Board: \n\n";

    for( int n = 0; n < g.h; n++ )
    {
        for( int i = 0; i < g.w; i++ )
        {
            std::cout << g.board[n][i] << " ";
        }

        std::cout << "\n";
    }
    std::cout << "==========================\n";
}
