#ifndef GAMELOGIC_H
#define	GAMELOGIC_H

#include <vector>
#include <algorithm>

//A 2d point, for passing to and from Game
struct Vector2i
{
	int x;
	int y;

	//only to be used by the AI
	int score;

	Vector2i(int xi, int yi) : x(xi), y(yi), score(0){}

	Vector2i( const Vector2i & other )
        : x( other.x ), y(other.y), score(other.score) {}
};

class Game
{
	public:
		//board[x][y], 0 = empty, 1+ = players
		std::vector< std::vector <int> > board;
		int h;
		int w;
		std::vector<Vector2i> allMoves;

		int currentPlayer; //1 = white, 10 = black
		int players;

        //arguments are not ready to be used, only use default
		Game( int numPlayers = 2, int height = 8, int width = 8 );

		Game(const Game & other);

		bool isBoardFilled();
		//only uses current player to check
		bool isValidMove( int x, int y );
		//checks from (x,y) to see if there are any moves in the given direction
		bool checkDirection( int x, int y, int vx, int vy );
		bool makeMove( int x, int y );
		void resetGame();

        //setters
		bool setCurrentPlayer( int player );

        //getters
		int getScore( int player );
		std::vector< Vector2i > getMoves();
		std::vector< Vector2i > getConvertedTiles( int x, int y );
		int getNumConverted( int x, int y );
		int getNumMoves();

		//called to display scores and current player's available moves.
		void printScoresMoves(); 
};

#endif
